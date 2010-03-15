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


#ifndef GWRAPPER_HDF 
#define GWRAPPER_HDF

#include "generic_wrapper.h"
namespace H5
{
class H5File;
class Group;
}


namespace utilities{

class Attr_list_hdf;

/**
   generic_wrapper for writing to hdf files

*/
class Generic_wrapper_hdf:public Generic_wrapper{
public:

  
  void open_wrapper();
  void close_wrapper();

  void open_group(const std::string & name = "none");
  void close_group();
  
  void add_dset(int rank, const int * dims, V_TYPE , const void *,const std::string & name );
  
  Generic_wrapper_hdf(std::string fname, bool add_to_file = true);
  
  
  void add_meta_data(const std::string & key, float val);
  void add_meta_data(const std::string & key, const Triple & val);
  void add_meta_data(const std::string & key, const Pair& val);
  void add_meta_data(const std::string & key,  const std::string & val);
  void add_meta_data(const std::string & key, int val);
  
  void add_meta_data(const std::string & key, float val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Triple & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Pair& val,const std::string & dset_name);
  void add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, int val,const std::string & dset_name);

  ~Generic_wrapper_hdf();
  
private:
  std::string file_name_;

  
    
  bool wrapper_open_;
  bool group_open_;
  bool dset_open_;

  bool add_to_file_;
  
  
  H5::H5File *file_;
  H5::Group * group_;
  
  Attr_list_hdf * group_attrs_;
  
  
  
 
};
}
#endif
