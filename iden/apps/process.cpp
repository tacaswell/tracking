//Copyright 2009-2010 Thomas A Caswell
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
#include <stdexcept>
#include "part_def.h"

#include "wrapper_i_plu.h"
#include "iden.h"
#include "params1.h"

#include "master_box_t.h"
#include "particle_base.h"
#include "hash_case.h"
#include "tuple.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "attr_list_hdf.h"
#include "read_config.h"

#include "image_stack.h"
#include "image_series.h"

#include "H5Cpp.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

using std::cout;
using std::cerr;
using std::endl;
using std::set;
using std::string;
using std::map;

using std::logic_error;
using std::runtime_error;


using utilities::Wrapper_i_plu;
using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Read_config;

using utilities::Tuple;

using utilities::Attr_list_hdf;

using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::V_TYPE;

using utilities::Image_stack;
using utilities::Image_series;
using utilities::Image_base;

using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;

using iden::Iden;
using iden::Params;

using H5::H5File;
using H5::Group;
using H5::Attribute;
using H5::PredType;
using H5::Exception;


int main(int argc, char * const argv[])
{
  
  		   
  string data_file ;
  string proc_file ;
  string pram_file ;
  
  
    
  int optchar;
  bool found_o= false,found_c = false;
    
  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'o':
      proc_file = string(optarg);
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

  if(!(found_o && found_c))
  {
    cerr<<"input failed"<<endl;
    cout<<"-i input filename"<<endl;
    cout<<"-o output filename"<<endl;
    cout<<"-c configuration filename"<<endl;
    return -1;
  }
  
  cout<<"file to read in: "<<data_file<<endl;
  cout<<"file that will be written to: "<<proc_file<<endl;
    
  float hwhm,thresh,top_cut;
  int feature_rad,dilation_rad, mask_rad;
  
    
  int comp_num;
  string grp_name;
  
  
  // read parameters out of the input xml that have to do with the 
  // particle location
  Read_config iden_prams(pram_file,"iden");
  if(!(iden_prams.contains_key("threshold") &&
       iden_prams.contains_key("p_rad") &&
       iden_prams.contains_key("hwhm") &&
       iden_prams.contains_key("d_rad") &&
       iden_prams.contains_key("top_cut") &&
       iden_prams.contains_key("mask_rad")))
    throw logic_error("process:: parameter file does not contain enough parameters");
  try
  {
    iden_prams.get_value("threshold",thresh);
    iden_prams.get_value("p_rad",feature_rad);
    iden_prams.get_value("hwhm",hwhm);
    iden_prams.get_value("d_rad",dilation_rad);
    iden_prams.get_value("mask_rad",mask_rad);
    iden_prams.get_value("top_cut",top_cut);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }


  // build the parameters object for shoving into the iden object
  Params p(feature_rad,hwhm,dilation_rad,thresh,mask_rad,top_cut);
  p.PrintOutParameters(std::cout);

  
  // read parameters from xml that have do to with logistics
  Read_config comp_prams(pram_file,"comp");
  if(!(comp_prams.contains_key("number")))
    throw logic_error("process: parameter file does not have enough parameters for the logistics");
  if(!comp_prams.get_value("number",comp_num))
    throw logic_error("process: failure to parse computation number");
    
  
  // read file name from the xml file
  Image_base * img_src;
  if(comp_prams.contains_key("base_name"))
  {
    string base_name;
    comp_prams.get_value("base_name",base_name);
    Image_series * tmp = new Image_series();
    tmp->init(base_name);
    img_src = tmp;
  }
  else if(comp_prams.contains_key("file_name"))
  {
    string file_name;
    comp_prams.get_value("file_name",file_name);
    Image_stack * tmp = new Image_stack(file_name);
    img_src = (Image_base *) tmp;
    tmp->initialize();
    
  }
  else
    throw runtime_error("did not provide a file name in the xml file");
  
  Iden iden(p);
  iden.set_image_src(img_src);

  

    
    
  Wrapper_i_plu *  wp = iden.fill_wrapper(10,0);
  cout<<"number of entries in wrapper: "<<wp->get_num_entries()<<endl;


  
  
  Tuple<float,2> dims = wp->get_dims();
  
    

  Master_box box;
  
  Filter_trivial filt;
    
  box.init(*wp,filt);



    
  cout<<"total number of particles is: "<<box.size()<<endl;;
    


    
    
  hash_case hcase;
  hcase.init(box,dims,20,wp->get_num_frames());

  bool error_flg = false;
  try
  {
    //Wrapper_o_hdf hdf_w("25-0_mid_0.h5",wh.get_data_types());
    set<D_TYPE> d_types = wp->get_data_types();
    
    Wrapper_o_hdf hdf_w(proc_file,d_types,comp_num,Wrapper_o_hdf::NEW_FILE);
    hdf_w.initialize_wrapper();
    hcase.output_to_wrapper(hdf_w);
    hdf_w.add_meta_data_list(iden_prams,d_types);

    hdf_w.finalize_wrapper();
  }
  catch(const char * err)
  {
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    error_flg = true;
  }
  catch(Exception & e)
  {
    std::cerr<<"caught on error: ";
    e.printError();
    
    error_flg = true;
  }
  catch(std::exception & e)
  {
    std::cerr<<"caught on std error: "<<endl
	     <<e.what()<<endl;
    

  }
  catch(...)
  {
    std::cerr<<"unknown error type"<<endl;
    error_flg = true;
  }

  // clean up wrapper
  delete wp;
  wp = NULL;
  
  ((Image_stack*)img_src)->deinitialize();
  
  
  delete img_src;
  img_src = NULL;
  

  if(error_flg)
    return -1;
  return 0;
  
  
}

