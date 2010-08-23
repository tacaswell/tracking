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
#include <sstream>

#include <string>
#include <vector>
#include <map>

#include <cmath>

#include "master_box_t.h"
#include "particle_base.h"
#include "hash_case.h"


#include "wrapper_o_hdf.h"
#include "filter.h"

#include "hash_case.h"



#include "wrapper_i.h"

#include "part_def.h"
#include "md_store.h"

namespace utilities{
/**
   Wrapper class for testing purposes
*/
class Wrapper_i_dummy:public Wrapper_in{
private:
  std::set<D_TYPE> d_types_;
  int frames_;
  int count_;
  mutable int counter_;
public:
  int get_value(int& out,int ind,D_TYPE type, int frame) const {
    if(type == D_FRAME){out = frame;}
    else{out = counter_++;}
    return out;
  }
  float get_value(float& out,int ind,D_TYPE type, int frame) const {
    out = ind ;
    return out;
  }
  std::complex<float> get_value(std::complex<float>& out,int ind,D_TYPE type, int frame) const 
  {return out;}
  std::set<D_TYPE> get_data_types() const {return d_types_;}
  int get_num_entries(unsigned int frame) const {return count_;};
  int get_num_entries() const {return count_*frames_;};
  int get_num_frames() const {return frames_;};
  bool contains_type(D_TYPE in) const{return d_types_.find(in) != d_types_.end();};
  Tuplef get_dims() const{return Tuplef();}
  ~Wrapper_i_dummy(){};
  Wrapper_i_dummy(const std::set<utilities::D_TYPE>& d_types,int count,int frames):
    d_types_(d_types),frames_(frames),count_(count),counter_(0){}
};
}


using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::exception;



using utilities::Wrapper_i_dummy;
using utilities::Wrapper_o_hdf;
using utilities::Tuple;
using utilities::Md_store;

using utilities::Filter_trivial;
using utilities::D_TYPE;



using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;

// taken from
// http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/#comment-22
template <class T>
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}




int main(int argc, const char * argv[])
{

  try
  {
      
    // set up data types to import form the input
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+2);
    
    int frame_c = 5;
    int p_c = 20;
    
    
    Wrapper_i_dummy wrapper_in = Wrapper_i_dummy(data_types,p_c,frame_c);
    wrapper_in.set_Md_store_size(frame_c);
    for(int j = 0; j< frame_c; ++j)
    {
      Md_store * tmp = new Md_store;
      wrapper_in.set_Md_store(j,tmp);
      string si= "int";
      string sf = "float";
      string ss = "string";
      string sji = to_string(j);
      string sjf = to_string(((float)j)*.1);
      tmp->add_element(si,si,sji);
      tmp->add_element(sf,sf,sjf);
      tmp->add_element(ss,ss,sjf);
    }
    
    
    Filter_trivial filt;
    Master_box box;
    box.init(wrapper_in,filt);
    cout<<box.size()<<endl;
    

    hash_case hcase;
    hcase.init(box,Tuple<float,2>(p_c+1,p_c + 1),(p_c+1)/10 + 1,frame_c);
    string fname = "test.hdf";

    
    Wrapper_o_hdf hdf_w(fname,data_types,322,Wrapper_o_hdf::NEW_FILE_OVR, "frame");

    
    try
    {
      hdf_w.initialize_wrapper();
      hcase.output_to_wrapper(hdf_w);
      hdf_w.add_meta_data("range",15,utilities::D_XPOS);
      
      hdf_w.finalize_wrapper();
    }
    catch(const char * err)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<err<<endl;
      return -1;
    }
    catch(exception& e)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<"\n"<<e.what()<<endl;
    }
    
    catch(...)
    {
      std::cerr<<"make_fake_hdf \n\tunknown error type"<<endl;
      return -1;
    }
    

    
    
    
 
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 

  

  return 0;

}
