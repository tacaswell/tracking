//Copyright 2008-2010 Thomas A Caswell
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
//
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.



#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"


#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot

#include "track_shelf.h"


#include <unistd.h>
#include <vector>
#include "read_config.h"


using std::string;
using std::vector;


using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::vector;
using std::map;

using std::logic_error;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;


using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;

using utilities::Read_config;




using namespace tracking;


static string APP_NAME = "tracking :: ";

int main(int argc, char * const argv[])
{
  
  
  Track_shelf tracks;
  
  		   
  string data_file ;
  string out_file ;
  string pram_file ;


  
      
  int optchar;
  bool found_i=false,found_o= false,found_c = false;

  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'i':
      data_file = string(optarg);
      found_i = true;
      break;
    case 'o':
      out_file = string(optarg);
      found_o = true;
      break;
    case 'c':
      pram_file = string(optarg);
      found_c = true;
      break;
      
    case '?':
    default:
      cout<<"-i input file name"<<endl;
      cout<<"-o output file name"<<endl;
      cout<<"-c parameter file name"<<endl;
      break;
    }
  }

  if(!(found_i && found_o && found_c))
  {
    cerr<<"input failed"<<endl;
    cout<<"-i input filename"<<endl;
    cout<<"-o output filename"<<endl;
    cout<<"-c configuration filename"<<endl;
    return -1;
  }
  
  cout<<"file to read in: "<<data_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;

    
  float box_side_len;
  float search_range;
  int min_track_length;

  
  Read_config app_prams(pram_file,"tracking");
  if(!(app_prams.contains_key("box_side_len")&&
       app_prams.contains_key("search_range")))
  {
    cerr<<APP_NAME + " parameter file does not contain enough parameters"<<endl;
    return -1;
  }
  try
  {
    app_prams.get_value("box_side_len",box_side_len);
    app_prams.get_value("search_range",search_range);
    if(app_prams.contains_key("min_track_length"))
      app_prams.get_value("min_track_length",min_track_length);
    else
      min_track_length = 3;
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  

  int read_comp_num, write_comp_num;
  Read_config comp_prams(pram_file,"comp");
  if(!(comp_prams.contains_key("read_comp")&&
       comp_prams.contains_key("write_comp")))
  {
    cerr<<APP_NAME + " parameter file does not contain both read and write comp nums"<<endl;
    return -1;
  }

  try
  {
    comp_prams.get_value("read_comp",read_comp_num);
    comp_prams.get_value("write_comp",write_comp_num);

  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  cout<<"file to read in: "<<data_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  cout<<"Parameters: "<<endl;
  cout<<"  box_side_len: "<<box_side_len<<endl;
  cout<<"  search_range: "<<search_range<<endl;
  cout<<"  min_track_length: "<<min_track_length<<endl;
  cout<<"comps: "<<endl;
  cout<<"  read_comp_num "<<read_comp_num<<endl;
  cout<<"  write_comp_num "<<write_comp_num<<endl;
  



    
  
  try
  {
    

    //nonsense to get the map set up
    map<utilities::D_TYPE, int> contents;
    utilities::D_TYPE tmp[] = {utilities::D_XPOS,
			       utilities::D_YPOS,
			       utilities::D_DX,
			       utilities::D_DY,
			       utilities::D_R2,
			       utilities::D_E};
    set<D_TYPE> data_types = set<D_TYPE>(tmp,tmp+6);
    
      
    // set up the input wrapper
    Wrapper_i_hdf wh(data_file,data_types,read_comp_num,0,100);
    
    // fill the master_box
    Master_box box;
    Filter_basic filt(data_file,read_comp_num);
    //Filter_trivial filt;
    box.init(wh,filt);
    
 
    
    cout<<wh.get_dims()<<endl;
    
    hash_case hcase(box,wh.get_dims(),box_side_len,wh.get_num_frames());
    cout<<"hash case filled"<<endl;
    
    Track_shelf tracks;
    
    hcase.link(search_range,tracks);
    cout<<"after searching found "<<tracks.get_track_count()<<" tracks"<<endl;
    

    
    tracks.remove_short_tracks(min_track_length);
    cout<<"after trimming found "<<tracks.get_track_count()<<" tracks"<<endl;
    // make wrapper
    
    D_TYPE tmp2[] = {utilities::D_XPOS,
    		     utilities::D_YPOS,
    };
    set<D_TYPE> data_types2 = set<D_TYPE>(tmp2, tmp2+2);
    
    
    set<D_TYPE> d2;
    d2.insert(utilities::D_NEXT_INDX);
    d2.insert(utilities::D_PREV_INDX);
    d2.insert(utilities::D_TRACKID);
    
    
    Wrapper_o_hdf hdf_w(out_file,d2,write_comp_num,
			Wrapper_o_hdf::APPEND_FILE,
			"frame");
    
    

    cout<<"made output wrapper"<<endl;
    
    
    try
    {
      hdf_w.initialize_wrapper();
      cout<<"inited wrapper"<<endl;
      hcase.output_to_wrapper(hdf_w,false);
      hdf_w.add_meta_data_list(app_prams,d2);
      hdf_w.finalize_wrapper();
    }
    catch(const char * err)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<err<<endl;
      return -1;
    }
    catch(std::exception & e)
    {
      cout<<"caught std excetp"<<endl;
      cout<<e.what()<<endl;
    }
    
    catch(...)
    {
      std::cerr<<"unknown error type"<<endl;
      return -1;
    }
    
    
    
    // cout<<"made wrapper"<<endl;
    // hdf_w.initialize_wrapper();
    // // set meta data on wrapper
    // hdf_w.add_meta_data(string("search_range"),search_range,true);
    // hdf_w.add_meta_data(string("min_track_length"),min_track_length,true);
    // // set the tracks to the wrapper
    // cout<<"set meta-data"<<endl;
    // tracks.output_to_wrapper(hdf_w);
    // cout<<"wrote tracks"<<endl;
    // // close wrapper
    // hdf_w.finalize_wrapper();
    // cout<<"done"<<endl;
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  } 
  catch( char const * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  } 
  catch(const string & err)
  {
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  }
  
  catch(...)
  {
    std::cerr<<"uncaught error"<<endl;
    return -1;
  }
  
    
  
  return 0;
}
