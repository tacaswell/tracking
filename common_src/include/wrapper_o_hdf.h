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

#ifndef WRAPPER_O_HDF
#define WRAPPER_O_HDF

#include <string>
#include <vector>
#include <set>


#include "enum_utils.h"
#include "wrapper_o.h"
#include "data_map.h"
#include "part_def.h"



namespace H5
{
class H5File;
class Group;
class DataSet;
}


namespace utilities{
class Wrapper_o_hdf_group;
class Md_store;

/**
   Wrapper class for dealing with output to matlab
*/
class Wrapper_o_hdf:public Wrapper_out{
public:  
  typedef enum FILE_T
    {
      APPEND_FILE=0,
      NEW_FILE,
      NEW_FILE_OVR,
      FILL_FILE
    }FILE_T;
  
    
      
      

  /**
     Constructor
     
     \param file_name full name of the file to open
     \param d_add a set of the data types to add
     \param group_prefix the prefix used for building group names
     \param new_file if a new file should be made, or writing data 
     in to an existing file
     \param new_indexing if the particles should be reindexed 
     when added to the wrapper
     \param over_write if existing data should be nuked 
     during name clashes
  */
  Wrapper_o_hdf(const std::string& file_name,
		const std::set<D_TYPE>& d_add,
		int comp_number,
		Wrapper_o_hdf::FILE_T file_type,
		const std::string & group_prefix="frame");
  
    
  void initialize_wrapper();
  void open_group(int,int,int);


  
  void set_all_values(const tracking::particle *);
#if PTYPE == 1
  void set_all_values(const tracking::Track_box *,const utilities::Tuple<float,3> &);
#endif
  void set_value(D_TYPE,const tracking::particle *);
  
  void add_meta_store(const Md_store * md_in);
  

  void close_group();
  void finalize_wrapper();

  void reset_wrapper(params * param);
  
  void print()const;
  int  num_entries()const{return 0;};

  const std::set<D_TYPE>& get_content_tpyes() const;
  



  

  

  void add_meta_data(const std::string & key, float val,bool root_group = true);
  void add_meta_data(const std::string & key, const Tuple<float,2> & val,bool root_group = true);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val,bool root_group = true);
  
  

  void add_meta_data(const std::string & key,  const std::string & val,bool root_group = true);

  void add_meta_data(const std::string & key, int val,bool root_group = true);

  
  void add_meta_data(const std::string & key, float val,D_TYPE dset_type);
  
  void add_meta_data(const std::string & key, const Tuple<float,2> & val,D_TYPE dset_type);
  
  void add_meta_data(const std::string & key, const Tuple<float,3> & val,D_TYPE dset_type);
  
  void add_meta_data(const std::string & key,  const std::string & val,D_TYPE dset_type);
  
  void add_meta_data(const std::string & key, int val,D_TYPE dset_type);

  void add_meta_data_list(const Read_config & , const std::set<D_TYPE> &);
  ~Wrapper_o_hdf();

private:

  ///Count of the number of particles that have been added
  int part_count_;
  
  ///if the wrapper is 'open' to get additional particles
  bool wrapper_open_;
  
  /**
     if there is a 'open' group
   */
  bool group_open_;
  
  /**
     index of current group
   */
  int group_index_;

  /**
     Parameter to control behavior of indexing
  */
  bool new_indexing_;
  
  /**
     Parameter to control file creation and over writing
   */
  Wrapper_o_hdf::FILE_T file_type_;
  
  /**
     Pointer to hdf file
   */
  H5::H5File * file_;
  /**
     file name
   */
  std::string file_name_;
  /**
     name of currently open group
   */
  std::string group_name_;
  
  /**
     A set of the data types that are to be added to the file
   */
  std::set<D_TYPE> d_types_add_;
  /**
     A set of the data types to be checked to make sure there isn't
     a mismatch
   */
  std::set<D_TYPE> d_types_check_;

  /**
     The set of data that the file already contains
   */
  std::set<D_TYPE> d_types_already_;
  
  /**
     prefix for use in formatting the group name
   */
  std::string group_prefix_;
  
  /**
     the number if digits in the formatted group name
   */
  static const int format_padding_ = 6;
  
  
  /**
     formats group names
   */
  std::string format_name(int in)const;
  
  /**
     object for current group
   */
  Wrapper_o_hdf_group * current_group_;
  

  /**
     computation number
   */
  int comp_number_;
  

  H5::Group * dset_pram_group_;
  
};

}

#endif
