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
using utilities::Md_store;
using utilities::SQL_handler;

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


/**
   \page gofr_doc g(r) HDF Documentation


   THIS IS SLIGHTLY OUT OF DATE

   This program takes in an hdf file of particle locations, computes 
   \f$g(r)\f$, and writes the results out to an hdf file.

   The program expects three input arguments
   <dl>
     <dt>-i ifname</dt>
     <dd>the full path of the input file</dd>
     <dt>-o ofname</dt>
     <dd>the full path of the output files</dd>
     <dt>-c cfname</dt>
     <dd>the full path of the xml configuration file</dd>
   </dl>
   
   Required function parameters in the xml file are
   <dl>
     <dt>nbins  </dt>
     <dd><em>int</em> The number of bins to use</dd>
     <dt>max_range </dt>
     <dd><em>float</em> The maximum radius to compute g(r) to.  This will also have
       an effet on the statistics of g(r) as only particles
       at least <kbd> max_range</kbd> away from the edge can be used.
     </dd>
     <dt>grp_name </dt>
     <dd><em>string</em> the name of the top level group to write the results to.  This may 
       be removed</dd>
   </dl>

   Required logistics parameters in the xml file are
   <dl>
     <dt>read_comp </dt>
     <dd><em>int</em> the computation key of the data to read in</dd>
     <dt>write_comp </dt>
     <dd><em>int</em>  the computation key of the data to be written out</dd>
     <dt>dset </dt>
     <dd><em>int</em> the dset number of the data being worked on</dd>
   </dl>
   All of these numbers refer to the data base scheme, documented else
   where.
       

 */

int main(int argc, char * argv[])
{

  		   
  string pram_file ;
  
  
    
  int optchar;
  bool found_c = false;
    
  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'c':
      pram_file = string(optarg);
      found_c = true;
      break;
    case 'i':
    case 'o':
    case 'd':
      break;
    case '?':
    default:
      cout<<"-c parameter file name"<<endl;
      break;
    }
  }

  if(!found_c)
  {
    cerr<<"input failed"<<endl;
    cout<<"-c configuration filename"<<endl;
    return -1;
  }
  
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
  
  int read_comp_num, write_comp_num,dset_key;
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("iden_key")&&
       comp_prams.contains_key("dset_key")))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
  try
  {
    comp_prams.get_value("iden_key",read_comp_num);
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

  sql.start_comp(dset_key,write_comp_num,utilities::F_GOFR);


  
  // set up md_store
  

  D_TYPE tmp[] = {utilities::D_XPOS,
		  utilities::D_YPOS,
		  utilities::D_DX,
		  utilities::D_DY,
		  utilities::D_I,
		  utilities::D_R2,
		  utilities::D_E
  };
  set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+7);
  
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
    filt.init(in_file,read_comp_num);
  Md_store filt_md = filt.get_parameters();
  //    Filter_trivial filt;
  
  Md_store md_store;
  md_store.add_elements(app_prams.get_store());    
  md_store.add_elements(comp_prams.get_store());    
  md_store.add_elements(files.get_store());    
  md_store.add_elements(&filt_md);
  md_store.add_element("comp_key",write_comp_num);
  
  

  
  Wrapper_i_hdf wh(in_file,data_types,read_comp_num);

    

  Master_box box;
    

  box.init(wh,filt);
    

    
  cout<<"total number of particles is: "<<box.size()<<endl;;
    
  Tuple<float,2> dims = wh.get_dims();
  hash_case hcase;
  hcase.init(box,dims,max_range/2,wh.get_num_frames());

  cout<<"hash case filled"<<endl;
    
  Corr_gofr gofr(nbins,max_range);
  hcase.compute_corr(gofr);
  cout<<"computed g(r)"<<endl;
    
  //    gofr.display();
  
  
      
  Generic_wrapper_hdf hdf_out(out_file,true);
  gofr.out_to_wrapper(hdf_out,format_name(grp_name,write_comp_num),&md_store);
  cout<<"wrote out g(r)"<<endl;

  // shove in to db
  sql.add_mdata(md_store);
  sql.commit();
  
  // clean up the data base
  sql.close_connection();

  
  return 0;
}

