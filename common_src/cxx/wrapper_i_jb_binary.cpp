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
using std::logic_error;
using std::ifstream;
using std::vector;
using std::set;

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

  int z_slice_count = 150;
  float z_shift = 0.5;
  float x_shift = 0.5;
  float y_shift = 0.5;

  vector< vector <float> > tmp_data_x(z_slice_count);
  vector< vector <float> > tmp_data_y(z_slice_count);
  

  
  
  float plane_thickness = 0.007 * 0.8;
  float plane_spacing = ((float) 1)/z_slice_count;
  

  
  
  
  ifstream ifs ( fname_.c_str() , ifstream::in | ifstream::binary);
  for(unsigned int j = 0; j<N; ++j)
  {
    // variables for reading data
    double x,y,z;
    // variables to cast them to floats (internal data type)
    float xf,yf,zf;
    // read triple of data
    ifs.read((char *) & x,8);
    ifs.read((char *) & y,8);
    ifs.read((char *) & z,8);
    
    xf = (float) x + x_shift; 
    yf = (float) y + y_shift;
    zf = (float) z + z_shift;
    
    // figure out which plane the particle is in
    int plane = (int) (zf/plane_spacing);
    // see if it is close enough to the plane, pulling slab a
    // plane_thickness above the plane (just because the
    // math/variables are a bit simpler)
    if (plane < z_slice_count && zf - plane*plane_spacing < plane_thickness )
    {
      tmp_data_x.at(plane).push_back(xf);
      tmp_data_y.at(plane).push_back(yf);
    }
    
    
  }
  
  wrapper_int_ = new Wrapper_i_generic();
  
  D_TYPE tmp[] = {utilities::D_XPOS,
		  utilities::D_YPOS,
  };
  set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+2);
    
  wrapper_int_->open_wrapper();
  wrapper_int_->setup(data_types,z_slice_count,Tuplef(1,1));
  

  for(int j = 0; j<z_slice_count; ++j)
  {
    vector<float> * tmpx = & tmp_data_x.at(j);
    vector<float> * tmpy = & tmp_data_y.at(j);
    unsigned int num_parts = tmpx->size();
    if (num_parts != tmpy->size())
      throw logic_error("x and y sizes don't match");
    
    wrapper_int_->open_frame(j,num_parts,j*plane_spacing + plane_thickness/2);
    
    wrapper_int_->set_data_type(utilities::D_XPOS);
    wrapper_int_->add_float_data(tmpx->data(),num_parts);

    wrapper_int_->set_data_type(utilities::D_YPOS);
    wrapper_int_->add_float_data(tmpy->data(),num_parts);

    wrapper_int_->close_frame();


  }
     
  wrapper_int_->finalize_wrapper();
    

  ifs.close();
  
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
