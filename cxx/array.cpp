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
#include <iostream>

#include "array.h"
#include "tuple.h"
#include "generic_wrapper_base.h"

using namespace utilities;
using std::cout;
using std::endl;
Array::Array(int rows)
  :rows_(rows),cols_(Tuple::length_),data_(rows_*cols_),cur_index_(0){
  if(rows_<1)
    throw "you are being stupid, too few rows";
}

void Array::print(){
  for( int j = 0; j < rows_; ++j){
    for(int i = 0; i < cols_; ++i){
      cout<<data_[cols_*j + i]<<"\t";
    }
    cout<<endl;
  }
}

void Array::push(const Tuple&  tuple_in){
  if((cur_index_) == rows_)
  {
    cout<<cur_index_<<" "<<rows_<<endl;
    
    throw "past end of array";
  }
  
  for(int j = 0; j<cols_;++j)
    data_.at(cur_index_*cols_ + j) = tuple_in[j];
  
  ++cur_index_;

}

void Array::set_array(Generic_wrapper_base * out_wrapper) const{

  
  out_wrapper->initialize_wrapper();
  for(int j = 0; j<rows_;++j){
    out_wrapper->start_new_row();
    for(int k = 0; k<cols_;++k){
      out_wrapper->append_to_row(data_[j*cols_ + k]);
    }
    out_wrapper->finish_row();
  }
  out_wrapper->finalize_wrapper();

  
}

void Array::clear(){
  cur_index_ = 0;
  data_.clear();
  data_.resize(rows_*cols_);
}


void Array::clear(int rows){
  cur_index_ = 0;
  data_.clear();
  data_.resize(rows*cols_);
  rows_ = rows;
}


void Array::resize(int rows){
  data_.resize(rows*cols_);
  rows_ = rows;
}
