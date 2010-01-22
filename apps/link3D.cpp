//Copyright 2009 Thomas A Caswell
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
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting


#include <iostream>

#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"
#include "pair.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"


#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot

#include "track_shelf.h"
#include "track_box.h"

#include <unistd.h>
#include <vector>
#include "read_config.h"



using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::vector;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Pair;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;
using utilities::Triple;
using utilities::Read_config;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_gofr;
using tracking::Track_shelf;
using tracking::Track_box;
static string base_proc_path = "/home/tcaswell/colloids/processed/";

int main(int argc, char * argv[])
{
  
  
  string proc_file,out_file;
  
  
  int pixel_per_box;
  float search_range ;
  int min_track_length;

  {
    
    int optchar;
    bool found_i=false,found_o= false,found_f=false;

    while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
    {
      switch(optchar)
      {
      case 'i':
	proc_file = string(optarg);
	found_i = true;
	break;
      case 'o':
	out_file = string(optarg);
	found_o = true;
	break;
      case 'c':
	{
	  vector<string> names;
	  names.push_back("box_side_len");
	  names.push_back("search_range");
	  names.push_back("min_trk_len");
	  Read_config rc(string(optarg),names,"link3D");
	  if(!rc.get_val("box_side_len",pixel_per_box))
	  {
	    cerr<<"box_side_len not found"<<endl;
	    return -1;
	  }
	  if(!rc.get_val("search_range",search_range))
	  {
	    cerr<<"search_range not found"<<endl;
	    return -1;
	  }
	  if(!rc.get_val("min_trk_len",min_track_length))
	  {
	    min_track_length = 3;
	
	  }
	  found_f = true;
	  break;
	}
      case '?':
      default:
	cout<<"-i input filename"<<endl;
	cout<<"-o output filename"<<endl;
	cout<<"-c configuration filename"<<endl;
	break;
      }
    }

    if(!(found_i && found_o && found_f))
    {
      cerr<<"input failed"<<endl;
      cout<<"-i input filename"<<endl;
      cout<<"-o output filename"<<endl;
      cout<<"-c configuration filename"<<endl;
      return -1;
    }
  
    cout<<"file to read in: "<<proc_file<<endl;
    cout<<"file that will be written to: "<<out_file<<endl;
    cout<<"Parameters: "<<endl;
    cout<<"  pixel_per_box: "<<pixel_per_box<<endl;
    cout<<"  search_range: "<<search_range<<endl;
    cout<<"  min_track_length: "<<min_track_length<<endl;



  }
  
  
  try
  {
   
    
    // set up data types to import form the input
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
		    utilities::D_DX,
		    utilities::D_DY,
		    utilities::D_I,
		    utilities::D_R2,
		    utilities::D_MULT,
		    utilities::D_E,
		    utilities::D_ZPOS
		    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+9);

  
    // set up the input wrapper
    Wrapper_i_hdf wh(proc_file,data_types);


    // fill the master_box
    Master_box box;
    Filter_basic filt(proc_file);
    //    Filter_trivial filt;
    box.init(wh,filt);
    
 
    Pair dims = wh.get_dims();
    cout<<dims<<endl;
    
    hash_case hcase(box,dims,pixel_per_box,wh.get_num_frames());
    cout<<"hash case filled"<<endl;
    
    Track_shelf tracks;
    
    hcase.link(search_range,tracks);

    
    tracks.remove_short_tracks(min_track_length);
    
    Track_shelf final_tracks;
    tracks.split_to_parts(final_tracks);
    

    cout<<tracks.get_track_count()<<endl;
    cout<<final_tracks.get_track_count()<<endl;
    
    D_TYPE tmp2[] = {utilities::D_XPOS,
    		     utilities::D_YPOS,
    		     utilities::D_I,
    		     utilities::D_ZPOS
    };
    set<D_TYPE> data_types2 = set<D_TYPE>(tmp2, tmp2+4);

    
    //tracks.pass_fun_to_track(&Track_box::plot_intensity);
    Wrapper_o_hdf hdf_w(out_file,data_types2,"frame",true);
    cout<<"made wrapper"<<endl;
    

    hdf_w.set_compress(false);
    float scale_tmp = wh.get_xy_scale();
    
    cout<<"scale_tmp: "<<scale_tmp<<endl;;
    

    Triple scale_t(scale_tmp,scale_tmp,1.0f);
    cout<<"scale triple :"<<scale_t<<endl;
    
    Triple dim(dims[0]*scale_tmp, dims[1]*scale_tmp,wh.get_num_frames()*.2);
    
    final_tracks.output_link_to_wrapper(hdf_w,scale_t,dim);
    
 
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 
  catch( char const * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 


  return 0;
  

}
