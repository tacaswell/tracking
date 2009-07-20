//Copyright 2008,2009 Thomas A Caswell
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

#include "H5Cpp.h"
#include <vector>
#include <map>
#include "wrapper_i.h"
#ifndef WRAPPER_I_MATLAB
#define WRAPPER_I_MATLAB


namespace tracking{

class Params_hdf5;

/**
   Wrapper class for eating data from hdf files
 */
class Wrapper_i_hdf5:public wrapper_i_base{
private:
  /**
     name for the data file
   */
  H5std_string file_name_;
  
protected:
  std::vector<int> frame_offsets_;
  void init();
public:
  int num_entries() const;
  
  
  void print()const;
  double get_value(int ind, wrapper::p_vals type)const;
  
  
  virtual ~Wrapper_i_hdf5()
  {
  };
  Wrapper_i_hdf5(Params_hdf5* param);
  void fill_master_box(master_box_t<particle_track> & test) const;

  Wrapper_i_hdf5(std::map<wrapper::p_vals, int> wrap);
  
  
};

}

#endif
