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

 

#ifndef WRAPPER_O_BASE
#define WRAPPER_O_BASE

#include <complex>
#include <set>

#include "enum_utils.h"
#include "part_def.h"

namespace tracking
{
  class Track_box;
}


namespace utilities
{
class params;

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
     Opens a group to put particles into
   */
  virtual void open_group(int group = 0,int p_count=0) = 0;
  

  /**
     sets up the wrapper to add new particle to the underlying data
     structure.  Behavior of this function if a particle is already 'open'
     is currently undefined(either close it and move on, or nuke the old
     data, or just keep appending to it)

     This function can either write straight in to the underlyign data structure
     or work through a temporary buffer
  */
  virtual void open_particle(int ind = -1)=0;
  
  /*
     sets the data for type to val for the currently activated
     particle in the data structure.  If the wrapper doesn't know
     about the D_TYPE type, then [[throw an exception|ignore]] )(have
     not decided yet)
    
     @param type
     the parameter to be set
     @param val
     the value to set the parameter to
  */
  //virtual void set_value(D_TYPE type, float val)=0;
  /*
     sets the data for type to val for the currently activated
     particle in the data structure.  If the wrapper doesn't know
     about the D_TYPE type, then [[throw an exception|ignore]] )(have
     not decided yet)
    
     @param type
     the parameter to be set
     @param val
     the value to set the parameter to
  */
  //virtual void set_value(D_TYPE type, int val)=0;
  
  /*
     sets data for complex numbers, see set_value

     @param type
     the parameter to be set
     @param val
     the value to set the parameter to
   */
  //virtual void set_value(D_TYPE type, std::complex<float> val)=0;
  
  
  /**
     sets the value of data type type for the particle p_in with out haveing
     to worry outside of the wrapper about the V_TYPE of the data
   */
  virtual void set_value(D_TYPE type, const tracking::particle * p_in) =0;
  
  /**
     sets all of the values that the wrapper knows about by asking the particle
   */
  virtual void set_all_values(const tracking::particle *)=0;


#if PTYPE == 1    
  /**
     sets all of the values that the wrapper knows about by asking the particle
   */
  virtual void set_all_values(const tracking::Track_box *) = 0;
#endif

  /**
     'closes' a particle. 
     Throws an error if there is no particle open
   */
  virtual void close_particle()=0;
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
  
  
  /**
     changes where the wrapper is pointing (ie, write out to a
     different array or file or some thin
   */
  virtual void reset_wrapper(params * param)=0;

  

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
  
  
  

};

}
#endif
  

  
  
