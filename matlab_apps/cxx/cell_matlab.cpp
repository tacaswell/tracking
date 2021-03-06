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

#include "cell_matlab.h"
#include "array.h"
#include "generic_wrapper_matlab.h"
#include "generic_parameters_matlab.h"

using namespace utilities;

Cell_matlab::Cell_matlab(int length, mxArray** ptr_in)
  :Cell(length),cur_index_(0)
{
  mwSize mlen= length_;
  cell_ = mxCreateCellArray(1,&mlen);
  *ptr_in = cell_;

}
 
void Cell_matlab::add_array(const Array & in){
  mxArray * tmp;
  
  Generic_parameters_matlab array_parm(in.get_row_num(),in.get_col_num(),&tmp);
  Generic_wrapper_base * wrapper = array_parm.make_wrapper();

  in.set_array(wrapper);
  
  if(!(cur_index_<length_))
  {
    throw "out of cell range";
  }
  // attempt to kill memory leak, doubt this is it
  mxArray * tmp_cell = mxGetCell(cell_,cur_index_);
  mxDestroyArray(tmp_cell);
  mxSetCell(cell_,cur_index_++,tmp);


}
