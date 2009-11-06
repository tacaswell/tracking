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

#ifndef WRAPPER_I_DUMMY
#define WRAPPER_I_DUMMY



#include <string>
#include <vector>
#include <map>

#include <cmath>

#include "wrapper_i.h"

#include "part_def.h"


namespace utilities{
/**
   Wrapper class for testing purposes
*/
class Wrapper_i_dummy:public Wrapper_in{
private:
  std::set<D_TYPE> d_types_;
  int frames_;
  int count_;
  
  
public:

  int                 get_value(int& out,
				int ind,D_TYPE type, int frame) const {
    out = 0;
    return out;
  }
  
  float               get_value(float& out,
				int ind,D_TYPE type, int frame) const 
  {
    int perd = 6;
    
    out = sin(ind*(3.14/perd))*sin((3.14/perd)*ind);
    return out;
  }
  
  std::complex<float> get_value(std::complex<float>& out,
				int ind,D_TYPE type, int frame) const 
  {
    return out;
  }
  


  std::set<D_TYPE>    get_data_types() const 
  {
    return d_types_;
  }
  



  int get_num_entries(int frame) const {return count_;};

  int get_num_frames() const {return frames_;};

  bool contains_type(D_TYPE in) const
  {
    return d_types_.find(in) != d_types_.end();
  };

  Tuple get_dims() const
  {
    return Tuple();
  }
  


  
  ~Wrapper_i_dummy(){};
  
 
  Wrapper_i_dummy(const std::set<utilities::D_TYPE>& d_types,int count,int frames):
    d_types_(d_types),frames_(frames),count_(count)
  {
  }
  



  

  
  
};

}

#endif
