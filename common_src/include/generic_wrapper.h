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

#ifndef GWRAPPER_BASE2 
#define GWRAPPER_BASE2

#include "enum_utils.h"
#include "tuple.h"
namespace utilities{

class Md_store;


/**
   Try two at a generic wrapper function.  This is based much more on the
   capabilities of hdf files, not shoving stuff back to matlab.  At some point
   a matlab subclass of this needs to get written.

   This is an abstract base class.
*/
class Generic_wrapper{
public:
  /**
     @name Wrapper state
     
     Change and query the top level wrapper state
   */
  //@{
  /**
     Initializes the wrapper
   */
  virtual void open_wrapper() =0;
  
  /**
     closes and cleans up the wrapper
   */
  virtual void close_wrapper() = 0;

  /**
     Returns if the wrapper is open or not
   */
  virtual bool is_open() const = 0;
  //@}
  
  /**
     @name Group state
     
     Change and query the wrapper current group state
   */
  //@{
  /**
     Opens a group in the wrapper, ie a group in hdf or a cell in matlab
   */
  virtual void open_group(const std::string & name = "none")=0;
  /**
     closes the current group
   */
  virtual void close_group()=0;

  //@}

  /**
     @name Data
     
     get and set data
   */
  //@{

  /**
     Adds data to the wrapper, must be the right size and type specified.
     data will be written safely with in the wrapper during this call so it
     can be safely freed outside if needed

     @param[in] rank rank of data set
     @param[in] dimensions of data set
     @param[in] vt data type
     @param[in] data pointer to data to be set
     @param[in] dset_name name of data set
   */
  virtual void add_dset(int rank,const unsigned int * dims, V_TYPE vt , const void * data,const std::string & dset_name)=0;

  /**
     Gets data from the wrapper.
   */
  virtual void get_dset(int rank,const unsigned int * dims, V_TYPE vt , const void * data,const std::string & dset_name ="none" )=0;


  ///@}
  /**
     @name Group Level Metadata

     getter and setter functions for metadata for the currently open
     group.

   */
  //@{
  /**
     Adds a float meta-data
   */
  virtual void add_meta_data(const std::string & key, float val)=0;
  /**
     Adds a Triple meta-data
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,3> & val)=0;
  /**
     Adds a Pair meta-data
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,2>& val)=0;
  /**
     Adds a string meta-data
   */
  virtual void add_meta_data(const std::string & key,  const std::string & val)=0;
  /**
     Adds an integer meta-data
   */
  virtual void add_meta_data(const std::string & key, int val)=0;
  /**
     Adds an unsigned integer meta-data
   */
  virtual void add_meta_data(const std::string & key, unsigned int val)=0;
  /**
     Adds all the meta data in a Md_store at the group level
   */
  virtual void add_meta_data(const Md_store * md_store)=0;

  /**
     Gets a float meta-data
   */
  virtual float get_meta_data(const std::string & key, float & val)=0;
  /**
     Gets a Triple meta-data
   */
  virtual Tuple<float,3> get_meta_data(const std::string & key,  Tuple<float,3> & val)=0;
  /**
     Gets a Pair meta-data
   */
  virtual Tuple<float,2> get_meta_data(const std::string & key,  Tuple<float,2>& val)=0;
  /**
     Gets a string meta-data
   */
  virtual std::string get_meta_data(const std::string & key,  std::string & val)=0;
  /**
     Gets an integer meta-data
   */
  virtual int get_meta_data(const std::string & key, int & val)=0;
  /**
     Gets an unsigned integer meta-data
   */
  virtual unsigned int get_meta_data(const std::string & key, unsigned int& val)=0;

  /**
     Gets all the meta data in a Md_store at the group level
   */
  virtual Md_store& get_meta_data(Md_store & md_store)=0;
  //@}
  /**
     @name Dataset Level Metadata

     getter and setter functions for metadata for the named dataset.

   */
  //@{
  /**
     Adds a float meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, float val,const std::string & dset_name)=0;
  /**
     Adds a Triple meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name)=0;
  /**
     Adds a Pair meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name)=0;
  /**
     Adds a string meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name)=0;
  /**
     Adds an integer meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, int val,const std::string & dset_name)=0;
  /**
     Adds all the meta data in a Md_store at the dset level
   */
  virtual void add_meta_data(const Md_store * md_store,const std::string & dset_name)=0;


  /**
     Gets a float meta-data for a data set
   */
  virtual float get_meta_data(const std::string & key, float& val,const std::string & dset_name)=0;
  /**
     Gets a Triple meta-data for a data set
   */
  virtual Tuple<float,3>  get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dset_name)=0;
  /**
     Gets a Pair meta-data for a data set
   */
  virtual Tuple<float,2> get_meta_data(const std::string & key, Tuple<float,2>& val,const std::string & dset_name)=0;
  /**
     Gets a string meta-data for a data set
   */
  virtual std::string get_meta_data(const std::string & key,  std::string & val,const std::string & dset_name)=0;
  /**
     Gets an integer meta-data for a data set
   */
  virtual int get_meta_data(const std::string & key, int &val,const std::string & dset_name)=0;
  /**
     Gets all the meta data in a Md_store at the dset level
   */
  virtual Md_store& get_meta_data(Md_store & md_store,const std::string & dset_name)=0;
  //@}
  /**
     Destructor
   */
  virtual ~Generic_wrapper(){};
  
  
 
};
}
#endif
