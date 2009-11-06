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
#include "triple.h"
#include "pair.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "master_box_t.h"

#include "generic_wrapper_hdf.h"
#include "corr_goftheta.h"



using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Tuple;
using utilities::Pair;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_goftheta;


static string base_proc_path = "/home/tcaswell/colloids/processed/";

int main(int argc, const char * argv[])
{

  if(argc != 3)
  {
    cerr<< "wrong number of args "<<argc<<endl;
    return 0;
  }
  
  
  string file_path = string(argv[1]);
  string file_name = string(argv[2]);
  string out_name = string(argv[0]);
  unsigned int slash_index = out_name.find_last_of("/");
  if(slash_index < out_name.size())
    out_name = out_name.substr(1+slash_index);
  
			     
  

  string proc_file = base_proc_path + file_path + file_name + ".h5";
  string out_file = base_proc_path + file_path + out_name + "_12.h5";
  cout<<"file to read in: "<<proc_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;

  try
  {


     
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
		    utilities::D_DX,
		    utilities::D_DY,
		    utilities::D_I,
		    utilities::D_R2,
		    utilities::D_MULT,
		    utilities::D_E
		    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+8);

  
    
    Wrapper_i_hdf wh(proc_file,data_types);

    

    Master_box box;
    Filter_basic filt(proc_file);
    //    Filter_trivial filt;
    

    box.init(wh,filt);


    
    cout<<"total number of particles is: "<<box.size()<<endl;;
    



    
    Pair dims = wh.get_dims();
    hash_case hcase(box,dims,12,wh.get_num_frames());

    cout<<"hash case filled"<<endl;
    
//    particle::set_neighborhood_range(14);
//    hcase.fill_in_neighborhood();
   
//    cout<<"neighborhood filled"<<endl;
   
    
    //hcase.print();

    
    Corr_goftheta goft(2000,(float)12,file_name);
    cout<<"made corr obj"<<endl;
    
    hcase.compute_corr(goft);
    cout<<"computed g(theta)"<<endl;
    
//     goft.display();

     
    Generic_wrapper_hdf hdf_out(out_file,true);
    goft.out_to_wrapper(hdf_out);
    cout<<"wrote out g(theta)"<<endl;
    
    
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 
  
  return 0;
}

