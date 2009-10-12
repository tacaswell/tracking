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







#ifndef WRAPPER_I_BASE
#define WRAPPER_I_BASE
#include "enum_utils.h"

#include <set>
#include <complex>
namespace utilities{
/**
   ABC for input wrappers
*/
class Wrapper_in
{
public:
  /*
     Returns the value of the type specified from the particle selected.  use the other version
   */
  //  virtual float get_value(int ind,D_TYPE type, int frame = -1) const=0;


  /**
       Returns the value of the type specified from the particle selected
  */
  virtual int get_value(int& out,
			int ind,D_TYPE type, int frame) const = 0;
  /**
     Returns the value of the type specified from the particle selected
   */
  virtual float get_value(float& out,
			  int ind,D_TYPE type, int frame) const = 0;
  /**
     Returns the value of the type specified from the particle selected
   */
  virtual std::complex<float> get_value(std::complex<float>& out,
				   int ind,D_TYPE type, int frame) const = 0;
  /**
     Returns a set of the data types.  Use the other version
   */
  virtual std::set<D_TYPE> get_data_types() const =0;

  /*
     Returns a set of the data types.  Use the other version
   */
  //  virtual void  get_data_types(std::set<D_TYPE> & out) const = 0;
  
  /**
     Return the number of entries the specified frame. Frame -1 returns the total for all frames
  */
  virtual int get_num_entries(int frame) const = 0;
  
  /**
     Return the number of frames
   */
  virtual int get_num_frames() const = 0;
  
  /**
     Returns if the wrapper contains the specified data type
   */
  virtual bool contains_type(D_TYPE type) const = 0;
  
};





}

#endif

