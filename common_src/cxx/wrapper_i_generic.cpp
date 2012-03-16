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
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
#include <stdexcept>

#include "wrapper_i_generic.h"
#include "master_box_t.h"
#include "particle_base.h"
#include "enum_utils.h"
#include "attr_list_hdf.h"
#include "md_store.h"

using H5::H5File;
using H5::Exception;
using H5::DataSpaceIException;
using H5::DataSetIException ;
using H5::FileIException;
using H5::Group;
using H5::DataSet;
using H5::DataSpace;
using H5::PredType;
using H5::Attribute;
using H5::Group;

using utilities::Tuplef;
using utilities::Wrapper_i_generic;
using utilities::D_TYPE;



using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;
using std::complex;
using std::invalid_argument;
using std::logic_error;
using std::runtime_error;
using std::pair;


using tracking::particle;



Wrapper_i_generic::Wrapper_i_generic():
    data_types_set_(), 
  d_mapi_(),
  d_mapf_(),
  d_mapc_(),
  data_i_(0),
  data_f_(0),
  data_c_(0),
  frame_c_(0),
  frame_count_(0),
  total_part_count_(0),
  frame_zdata_(0),
  locked_(false),
  frame_added_datatypes_(NULL),
  frame_open_(false),
  cur_dtype_(D_SENTRY),
  two_d_data_(true)
{
}

Wrapper_i_generic::~Wrapper_i_generic()
{
  // clean up data
  clean_data();
  
}


// data insertion functions
bool Wrapper_i_generic::open_wrapper()
{
  clean_data();
  return true;
  
}


bool Wrapper_i_generic::setup(std::set<D_TYPE> dtypes, int N)
{

  if (locked_)
    throw runtime_error("trying to set the data types of a locked wrapper");
  
  frame_count_ = N;
  frame_c_.reserve(frame_count_);  
  frame_zdata_.resize(frame_count_);
  
  data_types_set_ = dtypes;
    
    
  // logic to set up data maps and data storage
  int i_count =0;
  int f_count =0;
  int c_count =0;
    
  for(set<D_TYPE >::iterator it = data_types_set_.begin();
      it != data_types_set_.end();++it)
  {
    D_TYPE cur = (*it);
    
    
    switch(v_type(cur))
    {
    case V_INT:
      data_i_.push_back(vector<int*>(frame_count_));
      d_mapi_.set_lookup(cur,i_count++);
      break;
    case V_FLOAT:
      data_f_.push_back(vector<float*>(frame_count_));
      d_mapf_.set_lookup(cur,f_count++);
      break;
    case V_COMPLEX:
      data_c_.push_back(vector<complex<float>*>(frame_count_));
      d_mapc_.set_lookup(cur,c_count++);
      break;
    case V_STRING:
    case V_BOOL:
    case V_GUID:
    case V_TIME:
    case V_UINT:
    case V_ERROR:
      throw logic_error("wrapper_i_generic: The data type should not have been " + VT2str_s(v_type(cur)));
    }
  }

  locked_ = true;
  return true;
  
}

bool Wrapper_i_generic::open_frame(unsigned int frame,int N,float z)
{
  if(frame_open_)
  {
    close_frame();
  }
  
    
  if(!(frame < frame_count_))
    throw runtime_error("Trying to added more frames than wrapper is set up to deal with");
  
  frame_c_.at(frame) = N;
  cur_frame_size_ = N;
  cur_frame_number_ = frame;
  total_part_count_ += N;
  frame_zdata_.at(frame) = z;
  frame_added_datatypes_ = new std::set<utilities::D_TYPE>;
  
  for(set<D_TYPE>::iterator it = data_types_set_.begin();
      it != data_types_set_.end();++it)
  {
    
    D_TYPE cur_type = *it;
  
    switch(v_type(cur_type))
    {
    case V_INT:
      data_i_.at(d_mapi_(cur_type)).at(cur_frame_number_) = new int [cur_frame_size_];
  
      break;
    case V_FLOAT:
      data_f_.at(d_mapf_(cur_type)).at(cur_frame_number_) = new float [cur_frame_size_];

      break;
    case V_COMPLEX:
      throw logic_error("not implemented yet");
	  
      break;
    case V_STRING:
    case V_BOOL:
    case V_GUID:
    case V_TIME:
    case V_UINT:
    case V_ERROR:
      throw logic_error("wrapper_i_hdf: The data type should not have been " + VT2str_s(v_type(cur_type)));
    }
	
  }
  
  frame_open_ = true;
  return true;
  
}



bool Wrapper_i_generic::close_frame()
{
  // add checks to make sure all of the data was filled in

  delete frame_added_datatypes_;
  cur_frame_size_ = 0;
  cur_dtype_ = D_SENTRY;
  
  return false;
}

// access functions lifted from _hdf
int Wrapper_i_generic::get_value(int& out,
			     int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw runtime_error("wrapper_i_generic: wrapper does not contain " + DT2str_s(type));
  
  
  if(v_type(type) != V_INT)
    throw runtime_error("wrapper_i_generic: wrong data type, not int");
  out = data_i_.at(d_mapi_(type)).at(frame)[ind]    ;
  

  return out;
}

float Wrapper_i_generic::get_value(float& out,
			       int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw runtime_error("wrapper_i_generic: wrapper does not contain " + DT2str_s(type));
  
  
  if(v_type(type) != V_FLOAT)
    throw runtime_error("wrapper_i_generic: wrong data type, not float");


  
  if(type == utilities::D_ZPOS && two_d_data_)
  {
    out = frame_zdata_.at(frame);
  }
  
  else
  {
    out = data_f_.at(d_mapf_(type)).at(frame)[ind]    ;
  }
  

  return out;
}

std::complex<float> Wrapper_i_generic::get_value(std::complex<float>& out,
					     int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw runtime_error("wrapper_i_generic: wrapper does not contain " + DT2str_s(type));
  
  if(v_type(type) != V_COMPLEX)
    throw runtime_error("wrapper_i_generic: wrong data type, not complex");
  out = data_c_.at(d_mapc_(type)).at(frame)[ind]    ;
  

  return out;
}







std::set<D_TYPE>Wrapper_i_generic::get_data_types() const 
{
  return set<D_TYPE>(data_types_set_);
}
int Wrapper_i_generic::get_num_entries(unsigned int frame)const
{
  return frame_c_.at(frame);
}

int Wrapper_i_generic::get_num_entries()const
{
  return total_part_count_;
}



int Wrapper_i_generic::get_num_frames() const
{
  return frame_count_;
}
      

bool Wrapper_i_generic::contains_type(utilities::D_TYPE in) const
{
  set<D_TYPE>::iterator it = data_types_set_.find(in);
  return it != data_types_set_.end();
}



void Wrapper_i_generic::clean_data()
{
  vector<std::vector<int*> >::iterator i_o_end = data_i_.end();
  for(vector<std::vector<int*> >::iterator outer = data_i_.begin();
      outer != i_o_end; ++outer)
  {
    vector<int *>::iterator i_i_end = (*outer).end();
    for(vector<int *>::iterator inner = (*outer).begin();
	inner!=i_i_end;++inner)
    {
      delete [] *inner;
      *inner = NULL;
      
    }
  }
  vector<std::vector<float*> >::iterator f_o_end = data_f_.end();
  for(vector<vector<float*> >::iterator outer = data_f_.begin();
      outer != f_o_end; ++outer)
  {
    vector<float *>::iterator f_i_end = (*outer).end();
    for(vector<float *>::iterator inner = (*outer).begin();
	inner!=f_i_end;++inner)
    {
      delete [] *inner;
      *inner = NULL;
    }
  }
  vector<vector<complex<float>*> >::iterator c_o_end = data_c_.end();
  for(vector<vector<complex<float>*> >::iterator outer = data_c_.begin();
      outer != c_o_end; ++outer)
  {
    vector<complex<float> *>::iterator c_i_end = (*outer).end();
    for(vector<complex<float> *>::iterator inner = (*outer).begin();
	inner!=c_i_end;++inner)
    {
      delete [] *inner;
      *inner = NULL;
    }
  }
  locked_= false;

}
