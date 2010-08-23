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

#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "hash_shelf.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"
#include "read_config.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot

#include "cl_parse.h"
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::map;
using std::vector;
using std::logic_error;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;


using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;
using utilities::Read_config;
using utilities::CL_parse;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Hash_shelf;
using tracking::Corr_gofr;

const static string APP_NAME = "phi6";


int main(int argc,  char *  argv[])
{
  
   
  string in_file; //= "test.h5";	
  string out_file; //= "test.h5";	
  string pram_file; //= "frame";
  
  float neighbor_range ;
  
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
  

  // add check to make sure the input and output are the same file
    
  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  cout<<"pram file: "<<pram_file<<endl;


  
  Read_config app_prams(pram_file,"phi6");
  if(!(app_prams.contains_key("neighbor_range")))
     throw logic_error(APP_NAME + " parameter file does not contain enough parameters");
  try
  {
    app_prams.get_value("neighbor_range",neighbor_range);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  int read_comp_num, write_comp_num;
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("read_comp")&&
       comp_prams.contains_key("write_comp")))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
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
  
     

  cout<<"neighbor_range: "<<neighbor_range<<endl;
  
  

  // make hcase

  try
  {
    
  
    //nonsense to get the map set up
    map<utilities::D_TYPE, int> contents;
    utilities::D_TYPE tmp[] = {utilities::D_XPOS,
  			       utilities::D_YPOS};
    set<D_TYPE> data_types = set<D_TYPE>(tmp,tmp+2);
    
      
    // set up the input wrapper
    Wrapper_i_hdf wh(in_file,data_types,read_comp_num);
  

    // fill the master_box
    Master_box box;
    //  Filter_basic filt(h5_file);
    Filter_trivial filt;
    box.init(wh,filt);
    cout<<"master_box contains "<<box.size()<<" particles"<<endl;
  
    hash_case hcase;
    hcase.init(box,wh.get_dims(),neighbor_range,wh.get_num_frames());
    cout<<"hash case filled"<<endl;
    
    
      
    particle::set_neighborhood_range(neighbor_range);
  
    hcase.pass_fun_to_shelf(&Hash_shelf::fill_in_neighborhood);
    hcase.pass_fun_to_part(&particle::fill_phi_6);


    // output everything back to the hdf file

    cout<<"finished computation"<<endl;
    

    set<D_TYPE> d2;
    d2.insert(utilities::D_S_ORDER_PARAMETER);
    d2.insert(utilities::D_N_SIZE);

    
    Wrapper_o_hdf hdf_w(out_file,d2,write_comp_num,
			Wrapper_o_hdf::APPEND_FILE,
			"frame");
    
    

    cout<<"made output wrapper"<<endl;
    
    
    try
    {
      hdf_w.initialize_wrapper();
      cout<<"inited wrapper"<<endl;
      hcase.output_to_wrapper(hdf_w);
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
