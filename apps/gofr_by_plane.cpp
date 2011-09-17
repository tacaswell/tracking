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
#include "corr_case.h"

#include "read_config.h"
#include "cl_parse.h"

#include "enum_utils.h"


#include "sql_handler.h"
#include "md_store.h"

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
using utilities::CL_parse;
using utilities::Read_config;

using utilities::Md_store;
using utilities::SQL_handler;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_case;

static string APP_NAME = "gofr_by_plane :: ";


int main(int argc, char * argv[])
{

  		   
  string pram_file;
  
  
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
  

  
  // parsing of parameters


  float max_range;
  int nbins,comp_count;
  string grp_name;
  Read_config app_prams(pram_file,"gofr_by_plane");
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
  
  int read_comp_key , write_comp_key, dset_key ;
  Read_config comp_prams(pram_file,"comps");
  try
  {
    comp_prams.get_value("iden_key",read_comp_key);
    comp_prams.get_value("dset_key",dset_key);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
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

  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;


  
  // set up sql stuff
  SQL_handler sql;
  sql.open_connection(db_path);


  // claim the entry in the DB and lock it before doing anything

  sql.start_comp(dset_key,write_comp_key,utilities::F_GOFR_BY_PLANE);


  
  D_TYPE tmp[] = {utilities::D_XPOS,
		  utilities::D_YPOS,
		  utilities::D_DX,
		  utilities::D_DY,
		  utilities::D_I,
		  utilities::D_R2,
		  utilities::D_E
  };
  set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+7);

  
    
  Wrapper_i_hdf wh;
  bool suc = wh.initialize(in_file,data_types,read_comp_key);
    


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
    filt.init(in_file,read_comp_key);
  Md_store filt_md = filt.get_parameters();
  
  
  Md_store md_store;
  md_store.add_elements(app_prams.get_store());    
  md_store.add_elements(comp_prams.get_store());    
  md_store.add_elements(files.get_store());    
  md_store.add_elements(&filt_md);
  md_store.add_element("comp_key",write_comp_key);
  
  

  
  
    
  Master_box box;
  box.init(wh,filt);
    

    
  cout<<"total number of particles is: "<<box.size()<<endl;;
    
  Tuple<float,2> dims = wh.get_dims();
  hash_case hcase;
  hcase.init(box,dims,max_range/2,wh.get_num_frames());

  cout<<"hash case filled"<<endl;

  
  if(app_prams.contains_key("comp_count"))
    app_prams.get_value("comp_count",comp_count);
  else if(app_prams.contains_key("frames_per_comp"))
  {
    int frames_per_comp;
    app_prams.get_value("frames_per_comp",frames_per_comp);
    comp_count = hcase.get_num_frames()/frames_per_comp;
    
  }
  else
    comp_count = wh.get_num_frames();
  




  Corr_case gofr_c((tracking::Corr_gofr*)NULL,comp_count,
		   max_range,nbins,
		   write_comp_key,
		   dset_key,
		   read_comp_key);
  unsigned int step = hcase.compute_corr(gofr_c);
  cout<<"computed g(r)"<<endl;
  md_store.add_element("comp_count",comp_count);
  md_store.add_element("frames_per_comp",step);
  
  //    gofr.display();


  // shove in to db, but don't commit
  sql.add_mdata(md_store);

  // save the data
  Generic_wrapper_hdf hdf_out(out_file,true);
  gofr_c.out_to_wrapper(hdf_out,utilities::format_name(grp_name,write_comp_key),&md_store);
  cout<<"wrote out g(r)"<<endl;



  // commit the md to the data base
  sql.commit();
  
  // clean up the data base
  sql.close_connection();

  
  return 0;
}

