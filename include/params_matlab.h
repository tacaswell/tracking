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
#include <vector>

#include "params.h"
#include "wrapper_i_matlab.h"
#include "wrapper_o_matlab.h"


#include "mex.h"

#ifndef PARAMS_MATLAB
#define PARAMS_MATLAB


namespace utilities{

  /**
     Parameter class for matlab input/output
   */
class params_matlab:public params{
 
public:
  ///Pointer to the pointer to the data array.  
  //using the extra level of dereference to make it work better with
  //output wrapper and matches the input/output format for the mex
  //function
  const mxArray** data_in;
  mxArray** data_out;
    
  ///Number of rows in the data
  int rows;

  ///number of columns in the data
  int cols;
  
  ///

  
  
  ///constructor
  params_matlab(const mxArray ** data_ptr,
		std::map<utilities::D_TYPE,int> contents )
    :params(contents),data_in(data_ptr){

  }
  
  params_matlab( mxArray ** data_ptr,
		std::map<utilities::D_TYPE,int> contents, int rows_in, int cols_in )
    :params(contents),data_out(data_ptr),rows(rows_in),cols(cols_in){

  }

    
  Wrapper_in* make_wrapper_in(){
    return new Wrapper_i_matlab(this);
  }
    
  
  Wrapper_out* make_wrapper_out(){
    return new Wrapper_o_matlab(this);
    }
    
    
};  
}
#endif
