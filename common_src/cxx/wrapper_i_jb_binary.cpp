//Copyright 2012 Thomas A Caswell
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
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
#include <fstream>
#include "wrapper_i_jb_binary.h"
using std::runtime_error;
using std::ifstream;

using utilities::Wrapper_i_jb_binary;
using utilities::D_TYPE;
using utilities::Tuplef;


  // inherited from ABC
int Wrapper_i_jb_binary::get_value(int& out, int ind,D_TYPE type, int frame) const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_value(out,ind, type, frame);
}

float Wrapper_i_jb_binary::get_value(float& out, int ind,D_TYPE type, int frame) const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_value(out,ind, type, frame);
}

std::complex<float> Wrapper_i_jb_binary::get_value(std::complex<float>& out,
				int ind,D_TYPE type, int frame) const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_value(out,ind, type, frame);
}



std::set<D_TYPE> Wrapper_i_jb_binary::get_data_types() const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_data_types();
}



int Wrapper_i_jb_binary::get_num_entries(unsigned int frame) const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_num_entries(frame);
}


int Wrapper_i_jb_binary::get_num_entries() const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_num_entries();
}


int Wrapper_i_jb_binary::get_num_frames() const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_num_frames();
}


bool Wrapper_i_jb_binary::contains_type(D_TYPE type) const 
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->contains_type(type);
}

Tuplef Wrapper_i_jb_binary::get_dims() const
{
  if(wrapper_int_ == NULL)
    throw runtime_error("wrapper not initialized");
  return wrapper_int_->get_dims();
}

bool Wrapper_i_jb_binary::set_file_name(const std::string & fname)
{
  fname_ = fname;
  return true;
}

bool Wrapper_i_jb_binary::proc_file(unsigned int N)
{
  ifstream ifs ( fname_.c_str() , ifstream::in | ifstream::binary);
  for(int j = 0; j<5; ++j)
  {
    double x,y,z;
    ifs.read((char *) & x,8);
    ifs.read((char *) & y,8);
    ifs.read((char *) & z,8);

    std::cout<<x<<'\t'<<y<<'\t'<<z<<std::endl;
    
  }
  
  return true;
  
}

Wrapper_i_jb_binary::~Wrapper_i_jb_binary()
{
  delete wrapper_int_;
}


Wrapper_i_jb_binary::Wrapper_i_jb_binary():
  fname_(),
  wrapper_int_(NULL)
{
  delete wrapper_int_;
}
