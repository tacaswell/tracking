//Copyright 2009-2012 Thomas A Caswell
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
class Generic_wrapper_hdf:public Generic_wrapper,Generic_wrapper_read{
public:
  /**
     Enum for determining hdf file creation behavior
   */
  typedef enum F_TYPE{ 
    /// open existing file read-write                   
    F_DISK_RDWR = 0,
    /// create a new file, must not previously exist             
    F_DISK_EXCL,
    /// creates new file, nukes existing file                    
    F_DISK_TRUNC,
    /// create file in memory                                    
    F_MEM,                     
  } F_TYPE;                                                                        

  /**
     @name Wrapper state
     
     Change and query the top level wrapper state
   */
  //@{  
  void open_wrapper();
  
  void close_wrapper();
  bool is_open()const
  {
    return wrapper_open_;
  }
  //@}
  
  /**
     @name Group state
     
     Change and query the wrapper current group state
   */
  //@{
  void open_group(const std::string & name = "none");
  void close_group();
  //@}
  /**
     @name Data
     
     get and set data
   */
  //@{

  void add_dset(int rank, const unsigned int * dims, V_TYPE , const void *,const std::string & name );
  void get_dset(int rank, const unsigned int * dims, V_TYPE , const void *,const std::string & name ){};  
  //@}   
    /**
     @name Group Level Metadata

     getter and setter functions for metadata for the currently open
     group.

   */
  //@{
  void add_meta_data(const std::string & key, float val);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val);
  void add_meta_data(const std::string & key, const std::string & val);
  void add_meta_data(const std::string & key, int val);
  void add_meta_data(const std::string & key, unsigned int val);
  void add_meta_data(const Md_store * md_store);

  float get_meta_data(const std::string & key, float & val);
  Tuple<float,3> get_meta_data(const std::string & key,  Tuple<float,3> & val);
  Tuple<float,2> get_meta_data(const std::string & key,  Tuple<float,2>& val);
  std::string get_meta_data(const std::string & key,  std::string & val);
  int get_meta_data(const std::string & key, int & val);
  unsigned int get_meta_data(const std::string & key, unsigned int& val);
  Md_store& get_meta_data(Md_store & md_store);
    //@}
  /**
     @name Dataset Level Metadata

     getter and setter functions for metadata for the named dataset.

   */
  //@{
    
  void add_meta_data(const std::string & key, float val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const std::string & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, int val,const std::string & dset_name);
  void add_meta_data(const Md_store * md_store,const std::string & dset_name);

  float get_meta_data(const std::string & key, float& val,const std::string & dset_name);
  Tuple<float,3>  get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dset_name);
  Tuple<float,2> get_meta_data(const std::string & key, Tuple<float,2>& val,const std::string & dset_name);
  std::string get_meta_data(const std::string & key,  std::string & val,const std::string & dset_name);
  int get_meta_data(const std::string & key, int &val,const std::string & dset_name);
  Md_store & get_meta_data(Md_store & md_store,const std::string & dset_name);
  //@}
  
  ~Generic_wrapper_hdf();
 
  /**
     Constructor

     @param fname the name of the file to write to
     @param add_to_file if the file already exists

     @todo replace bool with enum
  */
  Generic_wrapper_hdf(std::string fname, F_TYPE f_type = F_DISK_RDWR);
  
private:
  template<class T>
  void add_meta_data_priv(const std::string & key, const T & val,const std::string & dset_name);
  template<class T>
  void add_meta_data_priv(const std::string & key, const T& val);

  template<class T>
  T get_meta_data_priv(const std::string & key,  T & val,const std::string & dset_name);
  template<class T>
  T get_meta_data_priv(const std::string & key,  T& val);
  
  std::string file_name_;

  
    
  bool wrapper_open_;
  bool group_open_;
  bool dset_open_;

  F_TYPE f_type_;
  
  
  H5::H5File *file_;
  H5::Group * group_;
  
  Attr_list_hdf * group_attrs_;
  
  
  
 
};
}
#endif
