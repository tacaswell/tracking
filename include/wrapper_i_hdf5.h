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

#ifndef WRAPPER_I_HDF
#define WRAPPER_I_HDF

#include "H5Cpp.h"

#include <string>
#include <vector>
#include <map>
#include "wrapper_i.h"

namespace tracking
{
template <class T>
class master_box_t;
class particle_base;

}


namespace utilities{

class Params_hdf5;

/**
   Wrapper class for eating data from hdf files
 */
class Wrapper_i_hdf5:public Wrapper_in{
private:
  /**
     name for the data file
   */
  std::string file_name_;
  
protected:
  std::vector<int> frame_offsets_;
  void init();
public:
  int num_entries() const;
  
  
  void print()const;
  double get_value(int ind, utilities::D_TYPE type)const;
  
  
  virtual ~Wrapper_i_hdf5()
  {
  };
  Wrapper_i_hdf5(Params_hdf5* param);
  void fill_master_box(tracking::master_box_t<tracking::particle_base> & test) const;

  Wrapper_i_hdf5(std::map<utilities::D_TYPE, int> wrap);
  
  
};

}

#endif
