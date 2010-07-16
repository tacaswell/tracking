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

#include<iostream>
#include<fstream>
#include <stdlib.h>
#include <string>



#include "wrapper_i_matlab.h"

using namespace utilities;
using std::cout;
using std::complex;
using std::endl;
using std::set;
using std::map;
using std::logic_error;
using std::runtime_error;



int Wrapper_i_matlab::get_num_entries(int frame)const{
  return frame_offsets_.at(frame +1) - frame_offsets_.at(frame);
  
}

int Wrapper_i_matlab::get_num_frames()const
{
  return frames_;
  
}

bool Wrapper_i_matlab::contains_type(D_TYPE type)const
{
  map<D_TYPE,int>::const_iterator it = data_types_.find(type);
  return it != data_types_.end();
}

Tuplef Wrapper_i_matlab::get_dims()const
{
  return dims_;
  
}



float Wrapper_i_matlab::get_value(int ind,  utilities::D_TYPE type,int frame)const{
  
  int data_posistion = data_map_(type);
  if(data_posistion <0)
    throw logic_error("wrapper_i_matlab: Wrapper does not contain " + DT2str_s(type));
    
  return *(first_ + ind + frame_offsets_.at(frame) + rows_ * data_posistion);
}

int Wrapper_i_matlab::get_value(int& out,int ind,D_TYPE type, int frame) const
{
  out = (int) get_value(ind,type,frame);
  return out;
  
}


float Wrapper_i_matlab::get_value(float &out,int ind, utilities::D_TYPE type,int frame)const
{
  out = get_value(ind,type,frame);
  return out;
  
}

complex<float> Wrapper_i_matlab::get_value(std::complex<float> &out,int ind, utilities::D_TYPE type,int frame)const
{
  throw " this wrapper type does not deal with complex yet";
}


void Wrapper_i_matlab::print()const{
  cout<<"rows: "<<rows_<<" cols: "<<cols_<<endl;
}


Wrapper_i_matlab::~Wrapper_i_matlab(){
  //don't need to do anything here because everything is taken care of
  //by matlab, or so we hope.
  first_ = NULL;
}


std::set<utilities::D_TYPE> Wrapper_i_matlab::get_data_types() const{
  set<utilities::D_TYPE> tmp;
  for(map<utilities::D_TYPE,int>::const_iterator it = data_types_.begin();
      it!= data_types_.end(); it++)
    tmp.insert((*it).first);
  return set<utilities::D_TYPE>(tmp);
}


void Wrapper_i_matlab::get_data_types(std::set<utilities::D_TYPE>& out) const{
  out = get_data_types();
  
}


Wrapper_i_matlab::Wrapper_i_matlab(const mxArray ** mex_array,std::map<utilities::D_TYPE,int> contents,Tuplef dims):
  mex_array_(mex_array),
  rows_(mxGetM(*mex_array)),
  cols_(mxGetN(*mex_array)), first_(mxGetPr(*mex_array)),
  dims_(dims),
  data_map_(contents)
  
{
  // make sure that the array has a frame column
  int frm_offset = data_map_(D_FRAME);
  
  if(frm_offset <0)
    throw logic_error("wrapper_i_matlab: Wrapper does not contain " + DT2str_s(D_FRAME));
  
  // make sure that the frame_offsets_ vector is empty
  frame_offsets_.clear();
  frame_offsets_.push_back(0);
  
  // loop over all data to get frame offsets and record the max x/max y for dims
  int cur_frame = (int) *(first_ + 0  + rows_ * frm_offset);
  for(int j = 0;j<rows_;++j)
  {
    int tmp_frame = (int) *(first_ + j  + rows_ * frm_offset);
    if(tmp_frame != cur_frame)
    {
      if(tmp_frame != (cur_frame +1))
	throw runtime_error("wrapper_i_matlab:: the frames must be evenly spaced and monotonic");
      
      cur_frame = tmp_frame;
      frame_offsets_.push_back(j);
    }
    
  }
  frames_ = frame_offsets_.size();
  
  frame_offsets_.push_back(rows_);
  
  
  
}
