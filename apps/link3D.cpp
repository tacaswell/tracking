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

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"




#include "track_shelf.h"


#include <unistd.h>
#include <vector>
#include "read_config.h"
#include <stdexcept>


using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::vector;
using std::logic_error;

using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Md_store;
using utilities::Tuple;
using utilities::Filter_ers;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;

using utilities::Read_config;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_gofr;
using tracking::Track_shelf;
using tracking::Track_box;


using std::runtime_error;


const static string APP_NAME = "link3D :: ";

int main(int argc, char * argv[])
{
  
  
  string in_file,out_file,pram_file;
  

  
        
  int optchar;
  bool found_i=false,found_o= false,found_c = false;
    
  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'i':
      in_file = string(optarg);
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
  
  // if(!(found_i && found_o && found_c))
  // {
  //   cerr<<"input failed"<<endl;
  //   cout<<"-i input filename"<<endl;
  //   cout<<"-o output filename"<<endl;
  //   cout<<"-c configuration filename"<<endl;
  //   return -1;
  // }
  
  string db_path;
  
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

  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  

    
  float box_side_len;
  int write_comp_number,iden_key,dset_key;
  float search_range ;
  int min_track_length;
  
  
  Read_config app_prams(pram_file,"link3D");
  if(!(app_prams.contains_key("box_side_len") &&
       app_prams.contains_key("search_range") &&
       app_prams.contains_key("min_trk_len")))
    throw logic_error(APP_NAME + " parameter file does not contain enough parameters");
  try
  {
    app_prams.get_value("box_side_len",box_side_len);
    app_prams.get_value("search_range",search_range);
    app_prams.get_value("min_trk_len",min_track_length);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("iden_key")&&
       comp_prams.contains_key("link_key")&&
       comp_prams.contains_key("dset_key")
       ))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
  try
  {
    comp_prams.get_value("iden_key",iden_key);
    comp_prams.get_value("link_key",write_comp_number);
    comp_prams.get_value("dset_key",dset_key);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
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
		    utilities::D_E,
		    utilities::D_ZPOS
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+8);

  
    // set up the input wrapper
    Wrapper_i_hdf wh;
    bool suc = wh.initialize(in_file,data_types,iden_key);


    // fill the master_box
    Master_box box;
    Filter_ers filt;
    if(app_prams.contains_key("e_cut") &&
       app_prams.contains_key("rg_cut") &&
       app_prams.contains_key("shift_cut"))
    {
      float e_cut,rg_cut,shift_cut;
      app_prams.get_value("e_cut",e_cut);
      app_prams.get_value("rg_cut",rg_cut);
      app_prams.get_value("shift_cut",shift_cut);
      filt.init(*(app_prams.get_store()));
    }
    else
    {

      vector<string> pram_lst(3,"");
      pram_lst.at(0) = "e_cut";
      pram_lst.at(1) = "rg_cut";
      pram_lst.at(2) = "shift_cut";
      Md_store filt_md = utilities::extract_prams(in_file,iden_key,pram_lst);
      filt.init(filt_md);
   
   
    }




    
    //    Filter_trivial filt;
    box.init(wh,filt);
    
 
    Tuple<float,2> dims = wh.get_dims();
    cout<<dims<<endl;
    
    hash_case hcase;
    hcase.init(box,dims,box_side_len,wh.get_num_frames());
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
    Wrapper_o_hdf hdf_w(out_file,data_types2,write_comp_number,Wrapper_o_hdf::NEW_FILE,"frame");
    cout<<"made wrapper"<<endl;
    
    
    float xy_scale,z_spacing;
    if(app_prams.contains_key("xy_scale"))
      app_prams.get_value("xy_scale",xy_scale);
    else
      xy_scale = wh.get_xy_scale();
   
  
    if(app_prams.contains_key("z_spacing"))
      app_prams.get_value("z_spacing",z_spacing);
    else
      z_spacing = .2;
    
  
    

    cout<<"xy_scale: "<<xy_scale<<endl;;
    cout<<"z_spacing: "<<z_spacing<<endl;;
    
    // the third component needs to be one because we do not need to
    // scale the value of the z-planes
    Tuple<float,3> scale_t(xy_scale,xy_scale,1.0f);
    cout<<"scale triple :"<<scale_t<<endl;
    
    Tuple<float,3> dim(dims[0]*xy_scale, dims[1]*xy_scale,wh.get_num_frames()*z_spacing);
    hdf_w.initialize_wrapper();
    final_tracks.output_link_to_wrapper(hdf_w,scale_t,dim);
    hdf_w.add_meta_data_list(app_prams,data_types2);
    hdf_w.finalize_wrapper();
 
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
  catch(...)
  {
    std::cerr<<"uncaught error"<<endl;
    return -1;
  }
  

  return 0;
  

}
