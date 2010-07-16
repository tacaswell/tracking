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
#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"

#include "read_config.h"

#include "enum_utils.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::logic_error;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Tuple;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;

using utilities::Read_config;

using utilities::format_name;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_gofr;

static string APP_NAME = "gofr :: ";


int main(int argc, char * argv[])
{

  		   
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

  // parsing of parameters


  float max_range;
  int nbins;
  string grp_name;

  Read_config app_prams(pram_file,"gofr");
  if(!(app_prams.contains_key("max_range") &&
       app_prams.contains_key("nbins") &&
       app_prams.contains_key("grp_name")))
    throw logic_error(APP_NAME + " parameter file does not contain enough parameters");
  try
  {
    app_prams.get_value("max_range",max_range);
    app_prams.get_value("nbins",nbins);
    app_prams.get_value("grp_name",grp_name);

  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  int read_comp_num, write_comp_num,dset_num;
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("read_comp")&&
       comp_prams.contains_key("write_comp")))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
  try
  {
    comp_prams.get_value("read_comp",read_comp_num);
    comp_prams.get_value("write_comp",write_comp_num);
    comp_prams.get_value("dset",dset_num);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  


  try
  {
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
		    utilities::D_DX,
		    utilities::D_DY,
		    utilities::D_I,
		    utilities::D_R2,
		    utilities::D_E
		    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+7);

  
    
    Wrapper_i_hdf wh(data_file,data_types,read_comp_num);

    

    Master_box box;
    Filter_basic filt(data_file,read_comp_num);
    //    Filter_trivial filt;
    

    box.init(wh,filt);
    

    
    cout<<"total number of particles is: "<<box.size()<<endl;;
    
    Tuple<float,2> dims = wh.get_dims();
    hash_case hcase(box,dims,max_range/2,wh.get_num_frames());

    cout<<"hash case filled"<<endl;
    
    Corr_gofr gofr(nbins,max_range,write_comp_num,dset_num,read_comp_num);
    hcase.compute_corr(gofr);
    cout<<"computed g(r)"<<endl;
    
    //    gofr.display();

      
    Generic_wrapper_hdf hdf_out(out_file,true);
    gofr.out_to_wrapper(hdf_out,format_name(grp_name,write_comp_num));
    cout<<"wrote out g(r)"<<endl;

    
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  } 
  catch(std::exception & e)
  {
    cerr<<"died on exception"<<endl;
    cerr<<'\t'<<e.what()<<endl;
    return -1;
  }
  
  return 0;
}

