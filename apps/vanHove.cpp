//Copyright 2010 Thomas A Caswell
//tcaswell@uchicago.edu
//http://jfi.uchicago.edu/~tcaswell
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 3 of the License, or (at
//your option) any later version.
//
//This program is distributed in the hope that it will be useful, but
//WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, see <http://www.gnu.org/licenses>.

#include <string>
#include <iostream>
#include <ctime>

#include "track_shelf.h"
#include "hash_case.h"  
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"


#include "generic_wrapper_hdf.h"
#include "master_box_t.h"

#include "read_config.h"
#include "cl_parse.h"
#include "md_store.h"

#include "ta_vanHove.h"
#include "sql_handler.h"

using std::endl;
using std::cout;
using std::cerr;
using std::string;
using std::map;
using std::pair;
using std::set;

using std::logic_error;
using std::exception;
using std::invalid_argument;


using utilities::Read_config;
using utilities::CL_parse;
using utilities::Md_store;
using utilities::D_TYPE;
using utilities::SQL_handler;

using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;
using utilities::Generic_wrapper_hdf;
using utilities::Filter_trivial;

using tracking::Track_shelf;
using tracking::hash_case;

using tracking::Master_box;
using tracking::TA_vanHove;


static string APP_NAME = "vanHove :: ";

int main(int argc, char * const argv[])
{

  
  string pram_file ;

    
  
  try
  {
    CL_parse cl(argc,argv);
    cl.parse();
    cl.get_fpram(pram_file);
  }
  catch(std::invalid_argument &e )
  {
    cout<<e.what()<<endl;
    return -1;
  }
  
  // parse out the standard logistic parameters
  int iden_key,track_key,dset_key;
  Read_config comp_prams(pram_file,"comps");
  try
  {
    comp_prams.get_value("iden_key",iden_key);
    comp_prams.get_value("track_key",track_key);
    comp_prams.get_value("dset_key",dset_key);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }


  // parse out the parameters that this application needs
  unsigned min_track_length,max_step,nbins;
  float max_range;
  
  Read_config app_prams(pram_file,"vanHove");
  try
  {
    app_prams.get_value("min_track_length",min_track_length);
    app_prams.get_value("max_step",max_step);
    app_prams.get_value("max_range",max_range);
    app_prams.get_value("nbins",nbins);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the apps parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  // parse out the file names
  string db_path;
  string in_file ;
  string out_file ;

  Read_config files(pram_file,"files");
  try
  {
    files.get_value("db_path",db_path);
    files.get_value("fin",in_file);
    files.get_value("fout",out_file);

  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the file parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  
  if( min_track_length <2)
    throw invalid_argument("the minimum track length must be at least 2 ");
  
  		  
		    
  //nonsense to get the map set up
  set<pair<utilities::D_TYPE,int> > contents;
  contents.insert(pair<D_TYPE, int> (utilities::D_XPOS,iden_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_YPOS,iden_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_NEXT_INDX,track_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_PREV_INDX,track_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_TRACKID,track_key));


  
  // set up sql stuff
  SQL_handler sql;
  sql.open_connection(db_path);

  
  // set up the input wrapper
  Wrapper_i_hdf wh;
  bool suc = wh.initialize(in_file,contents,0,0,true);

  std::clock_t start = std::clock();
  // set up all of the needed objects
  Filter_trivial filt;
  Master_box box;
  Track_shelf tracks;
  hash_case hcase;
  // initialize everything
  try
  {
    box.init(wh,filt);
    hcase.init(box,wh.get_dims(),50,wh.get_num_frames());
    tracks.init(box);
  }
  catch(exception & e)
  {
    cerr<<"error initializing master box and tracks"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  
  // md stuff
  int dtime = hcase.get_avg_dtime();
  float temperature = hcase.get_avg_temp();

  // set up md store to pass to output 
  Md_store md_store;
  md_store.add_elements(app_prams.get_store());
  md_store.add_elements(comp_prams.get_store());    
  md_store.add_element("dtime",dtime);
  md_store.add_element("temperature",temperature);
  md_store.add_element("fin",in_file.c_str());
  md_store.add_element("fout",out_file.c_str());
    
  
  cout<<"track case filled in "<<( ( std::clock() - start ) / (double)CLOCKS_PER_SEC ) <<"s"<<endl;
  
  // trim tracks and compute mean displacement
  tracks.remove_short_tracks(min_track_length);
  hcase.compute_mean_disp();
  

  
     
  // data base stuff
  int comp_key;
  sql.start_comp(dset_key,comp_key,utilities::F_VANHOVE);

  md_store.add_element("comp_key",comp_key);
  
  // compute the van Hove distributions
  TA_vanHove vanHove(max_step,nbins,max_range);
  tracks.compute_corrected_TA(vanHove);
  




  // make output wrapper and shove data in to it
  try
  {
    Generic_wrapper_hdf hdf_out(out_file,Generic_wrapper_hdf::F_DISK_RDWR);  
    vanHove.output_to_wrapper(hdf_out,md_store);
  }
  catch(exception & e)
  {
    cerr<<"error outputting the histograms"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  // close the db connection cleanly
  sql.close_connection();

  
  return 0;
  

}
