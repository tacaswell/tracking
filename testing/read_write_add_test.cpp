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
#include <map>
#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"
#include "pair.h"

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "hash_case.h"
#include "hash_shelf.h"



#include <string>
#include <vector>
#include <map>

#include <cmath>

#include "wrapper_i.h"

#include "part_def.h"

using std::map;

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::exception;



using utilities::Wrapper_i_hdf;
using utilities::Wrapper_o_hdf;

using utilities::Pair;
using utilities::Filter_trivial;
using utilities::D_TYPE;



using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Hash_shelf;



int main()
{

  string fake_data_file = "test.h5";
  string out_file = "test_out.hdf";
  float neighbor_range = 10;
  

  // read in hdf file written by python script that makes hex clusters
  try
  {
    // set up data types to import form the input
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+2);

    
    Wrapper_i_hdf wrapper_in = Wrapper_i_hdf(fake_data_file,data_types,0);
    
    
    Filter_trivial filt;
    Master_box box;
    box.init(wrapper_in,filt);

    cout<<"master_box contains "<<box.size()<<" particles"<<endl;
    
    Pair dim_tmp = wrapper_in.get_dims();
    
    int frame_tmp = wrapper_in.get_num_frames();
    
      
    
    hash_case hcase(box,dim_tmp,1,frame_tmp);
    cout<<"finished making case"<<endl;
    

    

    // write out to a new hdf file
    Wrapper_o_hdf hdf_w(out_file,data_types,1,Wrapper_o_hdf::NEW_FILE_OVR,"frame");
    hdf_w.initialize_wrapper();
    hcase.output_to_wrapper(hdf_w);
    hdf_w.add_meta_data("range",15,utilities::D_XPOS);
    hdf_w.add_meta_data("range",15,utilities::D_YPOS);
    hdf_w.finalize_wrapper();
				 
  }
  catch(std::exception & e)
  {
    cerr<<"hit an error"<<endl;
    
    cerr<<e.what()<<endl;
    return -1;
    
  }
  
  catch(...)
  {
    // make this a more sensible catch
    cout<<"fail on first read or write"<<endl;
    
    return -1;
    
  }
  
  cout<<"Escaped the first try"<<endl;
  
  // re open file and compute and add phi6 data
  try
  {
    
  
    //nonsense to get the map set up
    map<utilities::D_TYPE, int> contents;
    utilities::D_TYPE tmp[] = {utilities::D_XPOS,
			       utilities::D_YPOS};
    set<D_TYPE> data_types = set<D_TYPE>(tmp,tmp+2);
    
      
    // set up the input wrapper
    Wrapper_i_hdf wh(out_file,data_types,1);
  

    // fill the master_box
    Master_box box;
    //  Filter_basic filt(h5_file);
    Filter_trivial filt;
    box.init(wh,filt);
    cout<<"master_box contains "<<box.size()<<" particles"<<endl;
  
    hash_case hcase(box,wh.get_dims(),neighbor_range,wh.get_num_frames());
    cout<<"hash case filled"<<endl;
    
    
      
    particle::set_neighborhood_range(neighbor_range);
  
    hcase.pass_fun_to_shelf(&Hash_shelf::fill_in_neighborhood);
    hcase.pass_fun_to_part(&particle::fill_phi_6);


    // output everything back to the hdf file

  
    set<D_TYPE> d2;
    d2.insert(utilities::D_S_ORDER_PARAMETER);

  
    Wrapper_o_hdf hdf_w(out_file,d2,2,Wrapper_o_hdf::APPEND_FILE,"frame");

    
    try
    {
      hdf_w.initialize_wrapper();
      hcase.output_to_wrapper(hdf_w);
      hdf_w.add_meta_data("range",neighbor_range,utilities::D_S_ORDER_PARAMETER);
      hdf_w.finalize_wrapper();
    
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
  catch(std::exception & e)
  {
    cerr<<"hit an error"<<endl;
    
    cerr<<e.what()<<endl;
    return -1;
    
  }
  catch(const char * err)
  {
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  }
  
  catch(...)
  {
    cout<<"uncought error"<<endl;
    return -1;
    
  }
  

  return 0;
  

}
