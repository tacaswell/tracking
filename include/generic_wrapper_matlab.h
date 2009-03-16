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

#ifndef GWRAPPER_MATLAB
#define GWRAPPER_MATLAB
#include "generic_wrapper_base.h"

#include "mex.h"
namespace utilities{
class Generic_parameters_matlab;

/**
   Class for handling output to matlab
*/

class Generic_wrapper_matlab:public Generic_wrapper_base{
public:


  /**
     @name row operations
   */
  //\@{
  /**
     Opens a new row to add data to
   */
  virtual void start_new_row() ;
  /**
     Inputs data to be added to the wrapper.

     This may write directly in to the underlying data
     structure, or it may buffer the data until a finalize call
   */
  virtual void append_to_row(double data_in);
  /**
     finishes the row and sets it to the underlying strucutre
   */
  virtual void finish_row();
  //\@}


  /**
     @name wrapper operations
   */
  //\@{
  /**
     Tells the wrapper to setup to accept rows
   */
  virtual void initialize_wrapper ();
  /**
     cleans up the wrapper
   */
  virtual void finalize_wrapper () ;
  //\@}
  
  
  virtual ~Generic_wrapper_matlab(){};

  Generic_wrapper_matlab(Generic_parameters_matlab* param);
private:
  double* data_ptr_;
  int rows_;
  int cols_;
  int row_indx_;
  int col_indx_;
  mxArray** mx_ptr_ptr_;
};
}
#endif
