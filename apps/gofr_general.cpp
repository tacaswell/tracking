//Copyright 2009-2012 Thomas A Caswell
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
#include "wrapper_i_jb_binary.h"
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



using utilities::Wrapper_i_jb_binary;
using utilities::Md_store;
using utilities::SQL_handler;

using utilities::Tuple;

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

  
  
  float max_range = 0.0038191149069655207*2*10;
  int nbins = 2000;
  



  string fname = "/home/tcaswell/work/justin_data/large_packings/3000000_particles_phi_eq_1.0/tempout.dat";
  string out_file = "/home/tcaswell/work/justin_data/large_packings/3000000_particles_phi_eq_1.0/gofr.h5";
  Wrapper_i_jb_binary wrap = Wrapper_i_jb_binary();
  wrap.set_file_name(fname);
  wrap.proc_file(3000000);
  
    


  Filter_trivial filt;
  

  Md_store md_store;
  
    

  hash_case hcase;
  hcase.init(wrap,filt,max_range/2);

  cout<<"hash case filled"<<endl;
  hcase.print();
  
  Corr_gofr gofr(nbins,max_range);
  hcase.compute_corr(gofr);
  cout<<"computed g(r)"<<endl;
    
  //    gofr.display();
  

  string grp_name = "gofr";
  int write_comp_num = 0;
  
      
  Generic_wrapper_hdf hdf_out(out_file);
  gofr.out_to_wrapper(hdf_out,format_name(grp_name,write_comp_num),&md_store);
  cout<<"wrote out g(r)"<<endl;

  
  return 0;
}

