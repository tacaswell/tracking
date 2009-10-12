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

#include "filter.h"
#include "wrapper_i.h"
#include "enum_utils.h"
#include "H5Cpp.h"

using namespace utilities;


using H5::H5File;
using H5::Attribute;
using H5::PredType;
using H5::Group;

Filter_basic::Filter_basic(float ecut,float rg_cut,float shift_cut):
  e_cut_(e_cut_),rg_cut_(rg_cut),shift_cut_(shift_cut)
{
}


Filter_basic::Filter_basic(std::string  fname)
{
  H5File* file = new H5File( fname, H5F_ACC_RDONLY );
  Group * group = new Group(file->openGroup("/"));
  
  Attribute at;
           
  at = group->openAttribute("e_cut");
  at.read(PredType::NATIVE_FLOAT,&e_cut_);
  at = group->openAttribute("rg_cut");
  at.read(PredType::NATIVE_FLOAT,&rg_cut_);
  at = group->openAttribute("shift_cut");
  at.read(PredType::NATIVE_FLOAT,&shift_cut_);
  
  
  delete group;
  delete file;
  
}


bool Filter_basic::operator()(int ind, int frame)
{
  float x,y;
  if(wrap_->get_value(x,ind,D_E,frame)>e_cut_ || wrap_->get_value(y,ind,D_R2,frame)>rg_cut_)
    return false;
  wrap_->get_value(x,ind,D_DX,frame);
  wrap_->get_value(y,ind,D_DY,frame);

  if((x*x +y*y) >shift_cut_*shift_cut_)
    return false;
  return true;
}
