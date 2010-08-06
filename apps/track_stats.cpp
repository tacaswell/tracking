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
#include "histogram.h"


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
using utilities::Histogram;


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

  
  int steps,hist_bins;
  float box_side_len,search_range,hist_range;
  
  // get tracking parameters 
  Read_config app_prams(pram_file,"track_stats");
  if(!(app_prams.contains_key("box_side_len")&&
       app_prams.contains_key("search_range")&&
       app_prams.contains_key("hist_range")&&
       app_prams.contains_key("hist_bins")&&
       app_prams.contains_key("steps")))
  {
    cerr<<APP_NAME + " parameter file does not contain enough parameters"<<endl;
    return -1;
  }
  try
  {
    app_prams.get_value("box_side_len",box_side_len);
    app_prams.get_value("search_range",search_range);
    app_prams.get_value("steps",steps);
    app_prams.get_value("hist_bins",hist_bins);
    app_prams.get_value("hist_range",hist_range);
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
  cout<<"  steps: "<<steps<<endl;
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
    


    // 
    

    Md_store md_store;
    md_store.add_elements(app_prams.get_store());
    md_store.add_elements(comp_prams.get_store());    
    
    
    
    
    Histogram trk_len_hist(wh.get_num_frames()+1,0,wh.get_num_frames()+1);

    
    
    tracks.track_length_histogram(trk_len_hist);
    
    
    string value_str = "bin_value";
    string edge_str = "bin_edges";
    string g_name;
    

    Generic_wrapper_hdf hdf_out(out_file,true);
    hdf_out.open_wrapper();
    g_name =  format_name("track_length",write_comp_num);
    trk_len_hist.output_to_wrapper(&hdf_out,g_name,value_str,edge_str,&md_store);
    hdf_out.open_group(format_name("disp_sq_hist",write_comp_num));
    hdf_out.close_group();
    
    for(int j = 0;j<steps;++j)
    {
      Histogram tmp_hist = Histogram(hist_bins,0,hist_range);
      tracks.disp_sq_hist(j+1,tmp_hist);
      g_name =  format_name("disp_sq_hist",write_comp_num) +format_name("/step",j+1) ;
      tmp_hist.output_to_wrapper(&hdf_out,g_name,value_str,edge_str,&md_store);
    }
    
    hdf_out.close_wrapper();
    
    
    
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
