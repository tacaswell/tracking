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


#include "generic_wrapper_matlab.h"
#include "generic_parameters_matlab.h"
#include "mex.h"

#include <iostream>


using namespace utilities;
using namespace std;

void Generic_wrapper_matlab::initialize_wrapper(){
  if(wrapper_open_){
    throw "wrapper already open";
  }
 
  *(mx_ptr_ptr_) = mxCreateDoubleMatrix(rows_,cols_, mxREAL);

  data_ptr_ = mxGetPr(*(mx_ptr_ptr_));


  row_indx_ = -1;
  col_indx_ = -1;
  

  wrapper_open_ = true;
}
void Generic_wrapper_matlab::start_new_row() {
  if(!wrapper_open_){
    throw "wrapper not open";
  }
  if(row_open_){
    throw "row still open";
  }
  if((++row_indx_ == rows_) || (row_indx_<0)){
    throw "past last row";
  }
  
  row_open_ = true;
//   cout<<"new row"<<endl;
//   cout<<row_indx_<<"\t"<<col_indx_<<endl;
}

void Generic_wrapper_matlab::append_to_row(float data_in){
  if(!row_open_){
    throw "row not open";
  }
  if((++col_indx_ == cols_)||(col_indx_<0)){
    throw "past last col";
  }
  *(data_ptr_ + row_indx_ + rows_ * col_indx_) =(double) data_in;
  
//   cout<<row_indx_<<"\t"<<col_indx_<<endl;
}

void Generic_wrapper_matlab::finish_row(){
  col_indx_ = -1;
  row_open_ = false;
//   cout<<row_indx_<<"\t"<<col_indx_<<endl;
}

void Generic_wrapper_matlab::finalize_wrapper () {
  if(!wrapper_open_){
    throw "wrapper not open";
  }
  if(row_open_){
    throw "row still open";
  }
  
  data_ptr_ = NULL;

  row_indx_ = -1;
  col_indx_ = -1;
  
  wrapper_open_ = false;
}

Generic_wrapper_matlab::Generic_wrapper_matlab(Generic_parameters_matlab* param)
  :Generic_wrapper_base(false,false),rows_(param->rows_), cols_(param->cols_),  
   row_indx_ (-1),col_indx_ (-1),   
   mx_ptr_ptr_( (param->mx_ptr_ptr_)){
   
  if(rows_ < 1 ||cols_<1)
    throw "nonsense array";
  

}



