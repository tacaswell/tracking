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


#include "sql_handler.h"
#include "md_store.h"

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
using utilities::Md_store;
using utilities::SQL_handler;


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
  
   
  string pram_file; //= "frame";
  
  float neighbor_range ;
  
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
  
  int iden_key, write_comp_num,dset_key;
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("iden_key")&&
       comp_prams.contains_key("dset_key")))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
  try
  {
    comp_prams.get_value("iden_key",iden_key);
    comp_prams.get_value("dset_key",dset_key);
   
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
     

  // set up sql stuff
  SQL_handler sql;
  sql.open_connection(db_path);


  // claim the entry in the DB and lock it before doing anything

  sql.start_comp(dset_key,write_comp_num,utilities::F_PHI6);


  cout<<"neighbor_range: "<<neighbor_range<<endl;
  
  

  // make hcase

  
    
  
  //nonsense to get the map set up
  map<utilities::D_TYPE, int> contents;
  D_TYPE tmp[] = {utilities::D_XPOS,
		  utilities::D_YPOS,
		  utilities::D_DX,
		  utilities::D_DY,
		  utilities::D_R2,
		  utilities::D_E
  };
  set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+6);
  

  
    
      
  // set up the input wrapper
  Wrapper_i_hdf wh(in_file,data_types,iden_key);
  

  
  
  
    
    

  Filter_basic filt;
  if(app_prams.contains_key("e_cut") &&
     app_prams.contains_key("rg_cut") &&
     app_prams.contains_key("shift_cut"))
  {
    float e_cut,rg_cut,shift_cut;
    app_prams.get_value("e_cut",e_cut);
    app_prams.get_value("rg_cut",rg_cut);
    app_prams.get_value("shift_cut",shift_cut);
    filt.init(e_cut,rg_cut,shift_cut);
    
    
  }
  else
    filt.init(in_file,iden_key);
  Md_store filt_md = filt.get_parameters();
  
   

  
    

  
    

  
      
  Md_store md_store;
  md_store.add_elements(app_prams.get_store());    
  md_store.add_elements(comp_prams.get_store());    
  md_store.add_elements(files.get_store());    
  md_store.add_elements(&filt_md);
  md_store.add_element("comp_key",write_comp_num);
  

  sql.add_mdata(md_store);
  
  hash_case hcase;
  hcase.init(wh,filt,neighbor_range);
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

  
  // shove in to db

  sql.commit();
  
  // clean up the data base
  sql.close_connection();


  return 0;
  


}
