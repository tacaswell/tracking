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

#include "wrapper_i_hdf.h"
#include "wrapper_o_hdf.h"
#include "filter.h"



using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;



using utilities::Wrapper_i_hdf;
using utilities::Wrapper_o_hdf;

using utilities::Pair;
using utilities::Filter_trivial;
using utilities::D_TYPE;



using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;


int main(int argc, const char * argv[])
{

  try
  {
    
    
    // set up data types to import form the input
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+9);
    string fname = "test.hdf";    
    
    Wrapper_i_hdf wrapper_in = Wrapper_i_hdf(fname,data_types);
    
    
    Filter_trivial filt;
    Master_box box;
    box.init(wrapper_in,filt);
    cout<<"master_box contains "<<box.size()<<" particles"<<endl;
    
    hash_case hcase(box,wrapper_in.get_dims(),1,wrapper_in.get_num_frames());
    
    set<D_TYPE> d2;
    d2.insert(utilities::D_INDEX);
    
    
    Wrapper_o_hdf hdf_w(fname,data_types,"frame",
			false,false,false);

    
    try
    {
      hcase.output_to_wrapper(hdf_w);
    }
    catch(const char * err)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<err<<endl;
      return -1;
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
  } 

  

  return 0;

}
