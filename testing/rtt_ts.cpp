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


#include "hash_case.h"
#include "track_shelf.h"
  
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"


#include "generic_wrapper_hdf.h"
#include "master_box_t.h"

#include "read_config.h"
#include "cl_parse.h"
#include "md_store.h"


using std::endl;
using std::cout;
using std::cerr;
using std::string;
using std::map;
using std::pair;
using std::set;

using std::logic_error;
using std::exception;


using utilities::Read_config;
using utilities::CL_parse;
using utilities::Md_store;
using utilities::D_TYPE;

using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;
using utilities::Generic_wrapper_hdf;
using utilities::Filter_trivial;

using tracking::Track_shelf;
using tracking::hash_case;
using tracking::Master_box;

static string APP_NAME = "rtt_hc :: ";

int main(int argc, char * const argv[])
{

  		   
  string in_file ;
  string out_file ;
  string pram_file ;

    
  
  try
  {
    CL_parse cl(argc,argv);
    cl.parse();
    cl.get_fin(in_file);
    cl.get_fout(out_file);
    cl.get_fpram(pram_file);
  }
  catch(std::invalid_argument &e )
  {
    cout<<e.what()<<endl;
    return -1;
  }

  
  // parse out the standard logistic parameters
  int read_comp_num, write_comp_num,dset;
  Read_config comp_prams(pram_file,"comps");
  try
  {
    comp_prams.get_value("read_comp",read_comp_num);
    comp_prams.get_value("write_comp",write_comp_num);
    comp_prams.get_value("dset",dset);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  // parse out the parameters that this application needs
  float search_range;
  Read_config app_prams(pram_file,"tracking");
  try
  {
    app_prams.get_value("search_range",search_range);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the apps parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }


  
  // parse out the parameters that this application needs
  int read_track_comp_num;
  Read_config trk_prams(pram_file,"read_track");
  try
  {
    trk_prams.get_value("read_track_comp_num",read_track_comp_num);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the apps parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  
  //nonsense to get the map set up
  set<pair<utilities::D_TYPE,int> > contents;
  contents.insert(pair<D_TYPE, int> (utilities::D_XPOS,read_comp_num));
  contents.insert(pair<D_TYPE, int> (utilities::D_YPOS,read_comp_num));
  contents.insert(pair<D_TYPE, int> (utilities::D_NEXT_INDX,read_track_comp_num));
  contents.insert(pair<D_TYPE, int> (utilities::D_PREV_INDX,read_track_comp_num));
  contents.insert(pair<D_TYPE, int> (utilities::D_TRACKID,read_track_comp_num));
  

  for(set<pair<D_TYPE, int> >::const_iterator it = contents.begin();
	it != contents.end(); ++it)
	cout<<(*it).first<<'\t'<<(*it).second<<endl;
	

  
  // set up the input wrapper
  Wrapper_i_hdf wh = Wrapper_i_hdf();
  wh.initialize(in_file,contents,0,0,true);

  std::clock_t start = std::clock();
  Filter_trivial filt;
  Master_box box;
  box.init(wh,filt);
  
  
  // fill the hash case
  hash_case hcase;
  hcase.init(box,wh.get_dims(),search_range,wh.get_num_frames());
  Track_shelf tracks;
  tracks.init(box);
  
  std::cout<< ( ( std::clock() - start ) / (double)CLOCKS_PER_SEC ) <<endl;

  cout<<"hash case filled"<<endl;
  
  
  // set<D_TYPE> d2;
  // d2.insert(utilities::D_NEXT_INDX);
  // d2.insert(utilities::D_PREV_INDX);
  // d2.insert(utilities::D_TRACKID);
    
  // // set up output wrapper
  // Wrapper_o_hdf particle_data_wrapper(out_file,d2,write_comp_num,
  // 				      Wrapper_o_hdf::APPEND_FILE,
  // 				      "frame");
    
  // try
  // {
  //   particle_data_wrapper.initialize_wrapper();
  //   cout<<"inited wrapper"<<endl;
  //   hcase.output_to_wrapper(particle_data_wrapper);
  //   particle_data_wrapper.add_meta_data_list(app_prams,d2);
  //   particle_data_wrapper.finalize_wrapper();
  // }
  // catch(exception & err)
  // {
  //   std::cerr<<"caught on error outputting to particle data: "<<err.what()<<endl;
  //   return -1;
      
  // }
    
  // cout<<"finished outputting to particle data"<<endl;
    
  // int dtime = hcase.get_avg_dtime();


  // Md_store md_store;
  // md_store.add_elements(app_prams.get_store());    
  // md_store.add_elements(comp_prams.get_store());    
  // md_store.add_element("dtime",dtime);
  // cout<<"set up md_store"<<endl;
    
  // try
  // {
  //   Generic_wrapper_hdf track_data_wrapper(out_file,true);
  //   tracks.output_to_wrapper(track_data_wrapper,&md_store);
  // }
  // catch(exception & err)
  // {
  //   std::cerr<<"caught on error outputting track data: "<<err.what()<<endl;
  //   return -1;
      
  // }
  
  return -1;
  

}
