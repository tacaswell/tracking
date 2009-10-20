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
class particle;

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
  
  H5::H5File * file_;
  mutable H5::Group * current_group_;
  mutable int current_frame_;
  bool file_open_;
  
  static std::string format_name(int in);
  
  void check_group(int frame) const;

  std::set<utilities::D_TYPE> data_types_;
  
protected:

  void init();
public:

  int get_value(int& out,
		int ind,D_TYPE type, int frame) const ;
  float get_value(float& out,
		  int ind,D_TYPE type, int frame) const ;
  std::complex<float> get_value(std::complex<float>& out,
				int ind,D_TYPE type, int frame) const ;
  std::set<D_TYPE>get_data_types() const ;



  int get_num_entries(int frame) const ;

  int get_num_frames() const ;

  bool contains_type(D_TYPE type) const ;
  


  
  ~Wrapper_i_hdf5();
  
  Wrapper_i_hdf5(std::string fname,std::set<utilities::D_TYPE> dtypes);

  void open_file();
  
  void close_file();
  

  
  
};

}

#endif
