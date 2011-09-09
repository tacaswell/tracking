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

#include "particle_track.h"
#include "hash_shelf.h"



#include "generic_wrapper_hdf.h"
#include "master_box_t.h"

#include "read_config.h"
#include "cl_parse.h"
#include "md_store.h"

#include "ta_msd.h"
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
using tracking::particle;
using tracking::hash_case;
using tracking::Hash_shelf;



using tracking::Master_box;
using tracking::TA_msd;


static string APP_NAME = "msd_sweep :: ";

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
  unsigned trk_len_min,trk_len_step,steps;
  int n_min,n_max;
  float n_range;
  
  
  Read_config app_prams(pram_file,"msd_sweep");
  try
  {
    app_prams.get_value("trk_len_min",trk_len_min);

    if(app_prams.contains_key("trk_len_step") &&
       app_prams.contains_key("steps"))
    {
      app_prams.get_value("trk_len_step",trk_len_step);
      app_prams.get_value("steps",steps);
    }
    else
    {
      steps = 0;
      trk_len_step = 0;
    }
    
    // see if arguments for neighborhood size cuts are passed in
    if(app_prams.contains_key("n_min"))
      app_prams.get_value("n_min",n_min);
    else
      n_min= -1;
    
    if(app_prams.contains_key("n_max"))
      app_prams.get_value("n_max",n_max);
    else
      n_max= -1;    

    if(!(n_min < 0 && n_max < 0))
      app_prams.get_value("n_range",n_range);
    else
      n_range = 0;
    
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

  
  if( trk_len_min <2)
    throw invalid_argument("the minimum track length must be at least 2 ");
  
		  
		    
  //nonsense to get the map set up
  set<pair<utilities::D_TYPE,int> > contents;
  contents.insert(pair<D_TYPE, int> (utilities::D_XPOS,iden_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_YPOS,iden_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_NEXT_INDX,track_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_PREV_INDX,track_key));
  contents.insert(pair<D_TYPE, int> (utilities::D_TRACKID,track_key));
  
  
  
  // set up the input wrapper
  Wrapper_i_hdf wh(in_file,contents,0,0,true);

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
    
  cout<<"track case filled in "<<( ( std::clock() - start ) / (double)CLOCKS_PER_SEC ) <<"s"<<endl;
  // make sure that all tracks 
  tracks.remove_short_tracks(2);
  cout<<"removed short tracks"<<endl;

  // if we are going to make cuts based on 
  if(!(n_min < 0 && n_max < 0))
  {
    particle::set_neighborhood_range(n_range);
    hcase.pass_fun_to_shelf(&Hash_shelf::fill_in_neighborhood);
  }
  
    
    
  // find the mean displacement per frame.  I am going to assume that
  // this should not change drastically based on the cutting to
  // tracks, but that may not be a good assumption.
  hcase.compute_mean_disp();


  
  // random md
  int dtime = hcase.get_avg_dtime();
  float temperature = hcase.get_avg_temp();
  

  
  
  // set up sql stuff
  SQL_handler sql;
  sql.open_connection(db_path);
  

  

  // open output file
  Generic_wrapper_hdf hdf_out(out_file,true);  
  hdf_out.open_wrapper();
  

  
  
  // do loop
  for(unsigned int count = 0; count < steps; ++count)
  {

    
    // data base stuff
    int comp_key;
    sql.start_comp(dset_key,comp_key,utilities::F_MSD);

    unsigned int len = trk_len_min + count * trk_len_step;
    // trim tracks and compute mean displacement
    tracks.remove_short_tracks(len);
    
    Md_store msd_md_store;
    msd_md_store.add_elements(comp_prams.get_store());
    msd_md_store.add_element("comp_key",comp_key);
    msd_md_store.add_element("msd_steps",len-1 );
    msd_md_store.add_element("min_track_length",len );
    msd_md_store.add_element("dtime",dtime);
    msd_md_store.add_element("temperature",temperature);
    msd_md_store.add_element("fin",in_file.c_str());
    msd_md_store.add_element("fout",out_file.c_str());
    if(! ( n_min < 0 && n_max < 0 ) )
      msd_md_store.add_element("n_range",n_range);
    if(n_min >= 0)
      msd_md_store.add_element("n_min",n_min);
    if(n_max >= 0)
      msd_md_store.add_element("n_max",n_max);
    
    // make ta_msd object
    TA_msd msd(len-1);
    // compute msd
    if(n_min < 0 && n_max < 0)
      // if the neighborhood cut sizes are default, don't bother checking
      tracks.compute_corrected_TA(msd);
    else
      // if either cut is not negative
      tracks.compute_corrected_TA_ncuts(msd,n_min,n_max);
    // output to the file
    msd.output_to_wrapper(hdf_out,msd_md_store);

    // add md to sql db
    sql.add_mdata(msd_md_store);
    sql.commit();



  }
  
  
  // close the file cleanly
  hdf_out.close_wrapper();

  // close the db connection cleanly
  sql.close_connection();

  
  
  

  
  return 0;
  

}
