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

namespace utilities{
/**
   Try two at a generic wrapper function.  This is based much more on the
   capabilities of hdf files, not shoving stuff back to matlab

   This is an abstract base class 
*/



class Generic_wrapper{
public:

  /**
     Initializes the wrapper
   */
  virtual void open_wrapper() =0;
  /**
     closes and cleans up the wrapper
   */
  virtual void close_wrapper() = 0;

  /**
     Opens a group in the wrapper, ie a group in hdf or a cell in matlab
   */
  virtual void open_group(const std::string & name = "none")=0;
  /**
     closes the current group
   */
  virtual void close_group()=0;
  /**
     Adds data to the wrapper, must be the right size and type specified.
     data will be written safely with in the wrapper during this call so it
     can be safely freed outside if needed
   */
  virtual void add_dset(int rank,const  int * dims, V_TYPE , const void *,const std::string & name ="none" )=0;

  /*
     Add metadata to the open group
   */
  //virtual void add_metadata(std::string, const void * , V_TYPE) = 0;
  
  
 
};
}
#endif
