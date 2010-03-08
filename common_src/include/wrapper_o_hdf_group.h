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


#ifndef WRAPPER_O_HDF_GRP
#define WRAPPER_O_HDF_GRP


#include <string>
#include <vector>
#include <set>

#include "H5Cpp.h"
#include "attr_list_hdf.h"
#include "enum_utils.h"
#include "wrapper_o.h"
#include "data_map.h"
#include "part_def.h"


#include "triple.h"
#include "pair.h"


namespace H5
{
class H5File;
class Group;
class DataSet;
}


namespace utilities{

/**
   Wrapper class for dealing with output to matlab
*/
class Wrapper_o_hdf_group{

public:  
  typedef enum GROUP_T
    {
      APPEND_GROUP=0,
      NEW_GROUP,
      FILL_GROUP
    }GROUP_T;
  
  
  /**
     set the size of the data chunks if compression is used.  Compression
     is used if the group has more than 5 times this many entries.
  */
  static const int format_padding;
  
  Wrapper_o_hdf_group(H5::CommonFG * parent, const std::string & g_name,
		      std::set<D_TYPE>,
		      int size,
		      int comp_num,
		      GROUP_T type);
  
		      
  

  void write_to_disk();
  
  
  
  void store_particle(const tracking::particle *);
  




  // group level
  bool contains_meta(const std::string & key);

  template <class T>
  void set_meta_data(const std::string & key, T & val);
  
  template <class T>
  T get_meta_data(const std::string & key, T &  val) ;

  // data set level
  bool contains_meta(const std::string & key,D_TYPE dtype);
  
  
  template <class T>
  void set_meta_data(const std::string & key, T & val, D_TYPE dtype);
  
  template <class T>
  T get_meta_data(const std::string & key, T &  val, D_TYPE dtype) ;

  ~Wrapper_o_hdf_group();
  
private:
  /**
     Parent object
  */
  H5::CommonFG * parent_;

  /**
     The group object
  */
  H5::Group * group_;
  
  /**
     Vector of data set pointers
  */
  std::vector<H5::DataSet * > dsets_;
  
  
  /**
     look up table for data sets
  */
  Data_map dset_map_;


  /**
     the reference number from the database 
  */
  int comp_number_;
  /**
     the number of particles to be added to the group, this must be known at
     creation time
  */
  int size_;

  /**
     a running count of the particles added
  */
  int added_count_;
  
  /**
     Sets what sort of group to try to deal with
   */  
  GROUP_T type_;
  
  /**
     The data types to add to the group, must be set at construction time
  */
  std::set<D_TYPE> d_types_add_;
  

  /**
     struct to deal with shoving complex data in to hdf
  */
  typedef struct complex_t
  {
    float re;
    float im;
  } complex_t;
  
  /**
     vector of pointers to temporary arrays for floats 
  */
  std::vector<float *> float_data_;

  /**
     look up table for float data
  */
  Data_map float_map_;
  

  /**
     vector of pointers to temporary arrays for ints
  */
  std::vector<int *> int_data_;

  /**
     look up table for float data
  */
  Data_map int_map_;

  
  /**
     vector of pointers to temporary arrays for floats 
  */
  std::vector<complex_t *> complex_data_;

  /**
     look up table for float data
  */
  Data_map complex_map_;

  
  /**
     set the size of the data chunks if compression is used.  Compression
     is used if the group has more than 5 times this many entries.
  */
  static const int csize_;
  
};


template <class T>
void Wrapper_o_hdf_group::set_meta_data(const std::string & key, T & val)
{
  Attr_list_hdf attr_list(group_);
  attr_list.set_value(key,val);
}
  
template <class T>
T Wrapper_o_hdf_group::get_meta_data(const std::string & key, T &  val) 
{
  Attr_list_hdf attr_list(group_);
  return attr_list.get_value(key,val);
}





}

#endif
