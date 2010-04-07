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


#include "accum_sofq.h"
#include <set>
#include "enum_utils.h"
#include "wrapper_i.h"
#include "filter.h"
#include "master_box_t.h"
#include "hash_case.h"
#include <cstdlib>
#include "particle_base.h"
using std::rand;


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
  const static int x_max_ = 500;
  const static int y_max_ = 500;
  int get_value(int& out,int ind,D_TYPE type, int frame) const {
    if(type == D_FRAME)
      out = frame;
    else
      out = counter_++;
    return out;
  }
  float get_value(float& out,int ind,D_TYPE type, int frame) const {

    
    if(type == D_XPOS)
      out = ind % x_max_;
    else if(type == D_YPOS)
      out = ind / x_max_;
    else
      out = ind ;
    return out;
  }
  std::complex<float> get_value(std::complex<float>& out,int ind,D_TYPE type, int frame) const 
  {return out;}
  std::set<D_TYPE> get_data_types() const {return d_types_;}
  int get_num_entries(int frame) const {return count_;};
  int get_num_frames() const {return frames_;};
  bool contains_type(D_TYPE in) const{return d_types_.find(in) != d_types_.end();};
  Tuplef get_dims() const{return Tuplef();}
  ~Wrapper_i_dummy(){};
  Wrapper_i_dummy(const std::set<utilities::D_TYPE>& d_types,int count,int frames):
    d_types_(d_types),frames_(frames),count_(count),counter_(0){}
};
}

using tracking::Accum_sofq;
using utilities::Tuplef;
using utilities::Tuplei;
using tracking::particle;

using utilities::Filter_trivial;
using utilities::D_TYPE;
using std::set;
using utilities::Wrapper_i_dummy;

using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;

using std::cout;
using std::endl;

using utilities::Tuple;


int main()
{
  

  
  // Accum_sofq test(range,bins);
  // particle * dummy = NULL;
  // test.add_particle(dummy);
  
  // test.display();
  
  
        
  // set up data types to import form the input
  D_TYPE tmp[] = {utilities::D_XPOS,
		  utilities::D_YPOS
  };
  set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+2);
    
  int frame_c = 1;
  
  int p_c = Wrapper_i_dummy::x_max_ * Wrapper_i_dummy::y_max_;
    
    
  Wrapper_i_dummy wrapper_in = Wrapper_i_dummy(data_types,p_c,frame_c);
  Filter_trivial filt;
  Master_box box;
  box.init(wrapper_in,filt);
  cout<<box.size()<<endl;
  
  

  
  Tuple<float,2>q_range(1./10,2);
  Tuplef q1(1,1) ;
  int bins = 500;

  

  Accum_sofq test2(q_range,q1,bins);
  box.compute_accum(test2);
  test2.display();
  


  return -1;

}
