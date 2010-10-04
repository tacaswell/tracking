//Copyright 2008-2010 Thomas A Caswell
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

 

#ifndef WRAPPER_O_BASE
#define WRAPPER_O_BASE

#include <complex>
#include <set>
#include <string>
#include "enum_utils.h"
#include "part_def.h"

namespace tracking
{
  class Track_box;
}


namespace utilities
{
class params;
class Read_config;
class Md_store;


/**
   ABC for output wrappers
*/
class Wrapper_out
{
public:
  /**
     @name Output interface
     New API for output wrappers, which is a bit more carefully thought about.

     All derived classes must implement these and only these public functions (because
     of the way that pointers to this base class are used in the code, only these
     functions are available in the bulk of the code)

     

     Typical life cycle:
     
     -initialize wrapper
     -loop over frames (if wrapper knows about frames)
     --loop over particles
     ---open particle
     ---set values
     ---close particle
     --close particles
     -close frames
     -finalize wrapper
  */
  //\@{

  
  /**
     preforms any initialization that needs to be done
     the logic for this is to not needlessly open and close files, or creat
     matlab arrays if they arn't going to be used.  It also allows the wrapper
     object to be reused (ie, do all the processing, output one subset of data to
     one place, and another subset to another place
   */
  virtual void initialize_wrapper()=0;


  /**
     Opens a group to put particles into. 

     @param group the index of the group to be opened
     @param p_count the number of particles to expect
     @param wrapper_size the size of the group of the underlying wrapper, if <0, then set equal to p_count
   */
  virtual void open_group(int group = 0,int p_count =0, int wrapper_size=-1) = 0;
  
  
  /**
     sets all of the values that the wrapper knows about by asking the particle
   */
  virtual void set_all_values(const tracking::particle *)=0;


#ifdef TRACKING_FLG
  /**
     sets all of the values that the wrapper knows about by asking the particle.

     This function is for outputting the results of 3D linking

   */
  virtual void set_all_values(const tracking::Track_box *,const utilities::Tuple<float,3> &) = 0;
#endif
  /**
     Closes the currently open group.
     Throws an error if there is no open group.
  */
  virtual void close_group()=0;
  
  /**
     Performs any clean up.
     that needs to be done such as closing a file
  */
  virtual   void finalize_wrapper()=0;
  
  
  /*
     changes where the wrapper is pointing (ie, write out to a
     different array or file or some thin
   */
  //virtual void reset_wrapper(params * param)=0;
  //\@}
  
   

  /**
     Returns a set containing the D_TYPES that the wrapper can cope with
   */
  virtual const std::set<D_TYPE>& get_content_tpyes() const = 0;
  
  /**
     virtual destructor
   */
  virtual ~Wrapper_out(){};
  /**
     empty constructor 
   */
  Wrapper_out(){};

  /**
     @name Group level metadata
     adds meta data to a group
  */
  //@{
  /**
     Adds a float meta-data
   */
  virtual void add_meta_data(const std::string & key, float val,bool root_group)=0;
  /**
     Adds a Triple meta-data
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,3> & val,bool root_group)=0;
  /**
     Adds a Pair meta-data
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,2>& val,bool root_group)=0;
  /**
     Adds a string meta-data
   */
  virtual void add_meta_data(const std::string & key,  const std::string & val,bool root_group)=0;
  /**
     Adds an integer meta-data
   */
  virtual void add_meta_data(const std::string & key, int val,bool root_group)=0;
  //@}

  /**
     Adds metadata for datasets
   */
  //@{
  /**
     Adds a float meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, float val,D_TYPE dset_type)=0;
  /**
     Adds a Triple meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,3> & val,D_TYPE dset_type)=0;
  /**
     Adds a Pair meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, const Tuple<float,2>& val,D_TYPE dset_type)=0;
  /**
     Adds a string meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key,  const std::string & val,D_TYPE dset_type)=0;
  /**
     Adds an integer meta-data for a data set
   */
  virtual void add_meta_data(const std::string & key, int val,D_TYPE dset_type)=0;

  /**
     Adds all parameters from the Read_config object to the data sets in the set
   */
  virtual void add_meta_data_list(const Read_config & , const std::set<D_TYPE> &)=0;
  //@}

  /**
     Adds all the meta data in a Md_store object
   */
  virtual void add_meta_store(const Md_store * md_in)  =0;
  
  
};

}
#endif
  

  
  
