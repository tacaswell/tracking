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
//
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.



#include "master_box_t.h"
#include "hash_case.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"
#include "generic_wrapper_hdf.h"
#include "track_shelf.h"
#include <unistd.h>
#include <vector>
#include "read_config.h"
#include "cl_parse.h"
#include "counted_vector.h"
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
using utilities::CL_parse;
using utilities::Counted_vector;
using utilities::format_name;

using utilities::Md_store;


using namespace tracking;


static string APP_NAME = "tracking :: ";

int main(int argc, char * const argv[])
{
  
  
  Track_shelf tracks;
  
  		   
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

  
  int msd_steps;

  // get msd parameters
  Read_config app_prams_msd(pram_file,"msd");
  if(!(app_prams_msd.contains_key("msd_steps")))
  {
    cerr<<APP_NAME + " parameter file does not contain enough parameters"<<endl;
    return -1;
  }
  try
  {
    app_prams_msd.get_value("msd_steps",msd_steps);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  float box_side_len, search_range;
  int min_track_length;
  
  
  // get tracking parameters 
  Read_config app_prams_trk(pram_file,"msd");
  if(!(app_prams_trk.contains_key("box_side_len")&&
       app_prams_trk.contains_key("search_range")))
  {
    cerr<<APP_NAME + " parameter file does not contain enough parameters"<<endl;
    return -1;
  }
  try
  {
    app_prams_trk.get_value("box_side_len",box_side_len);
    app_prams_trk.get_value("search_range",search_range);
    if(app_prams_trk.contains_key("min_track_length"))
      app_prams_trk.get_value("min_track_length",min_track_length);
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
  Read_config comp_prams(pram_file,"comps");
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

  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  cout<<"Parameters: "<<endl;
  cout<<"  box_side_len: "<<box_side_len<<endl;
  cout<<"  search_range: "<<search_range<<endl;
  cout<<"  min_track_length: "<<min_track_length<<endl;
  cout<<"  msd_steps: "<<msd_steps<<endl;
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
    Wrapper_i_hdf wh(in_file,data_types,read_comp_num);
    
    // fill the master_box
    Master_box box;
    Filter_basic filt(in_file,read_comp_num);
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
    
    
    hcase.compute_mean_disp();

    Counted_vector md(msd_steps);
    Counted_vector msd(msd_steps);
    Counted_vector msd_sq(msd_steps);
    
    
    
    
    tracks.msd_corrected(md,msd,msd_sq);

    
    md.average_data();
    msd.average_data();
    msd_sq.average_data();
    

    string data_str = "data";
    string count_str = "count";
    string g_name;
    
    Generic_wrapper_hdf hdf_out(out_file,true);

    float dtime = hcase.get_avg_dtime();
    

    Md_store md_store;
    md_store.add_elements(app_prams_msd.get_store());
    md_store.add_elements(app_prams_trk.get_store());    
    md_store.add_elements(comp_prams.get_store());    
    md_store.add_element("dtime",dtime);
    
    g_name = format_name("mean_disp",write_comp_num);
    
    
    md.output_to_wrapper(&hdf_out,
			 g_name,
			 data_str,
			 count_str,
			 &md_store);
    g_name = format_name("mean_squared_disp",write_comp_num);
    msd.output_to_wrapper(&hdf_out,
			  g_name,
			 data_str,
			  count_str,
			  &md_store);
    g_name = format_name("msd_squared",write_comp_num);    
    md.output_to_wrapper(&hdf_out,
			 g_name,
			 data_str,
			 count_str,
			 &md_store);
    
    cout<<"wrote out g(r)"<<endl;

    
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
  catch(const std::exception & err)
  {
    std::cerr<<"caught on error: ";
    
    std::cerr<<err.what()<<endl;
    return -1;
  }
  
  catch(...)
  {
    std::cerr<<"uncaught error"<<endl;
    return -1;
  }
  
    
  
  return 0;
}
