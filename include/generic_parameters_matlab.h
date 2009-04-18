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


#ifndef GPARAM_MATLAB
#define GPARAM_MATLAB

#include "generic_parameters_base.h"

#include "mex.h"

namespace utilities{
class Generic_wrapper_base;

/**
   Class for generic matlab wrapper parameters
*/

class Generic_parameters_matlab:public Generic_parameters_base{
public:
  int rows_;
  int cols_;
  mxArray ** mx_ptr_ptr_;
  Generic_parameters_matlab(int a, int b,mxArray ** );
  /**
     allocate a new wrapper
   */
  Generic_wrapper_base* make_wrapper() ;
  /**
     Allows the mex target to be changed
  */
  void change_mxArray(mxArray ** new_ptr_ptr)
  {
    mx_ptr_ptr_ = new_ptr_ptr;
  }
  
  
};

inline Generic_parameters_matlab::Generic_parameters_matlab(int rows, int cols,mxArray ** out)
  :rows_(rows),cols_(cols),mx_ptr_ptr_(out){}

}
#endif
