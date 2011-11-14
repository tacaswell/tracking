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
#include "attr_list_hdf.h"
#include "md_store.h"
#include "H5Cpp.h"

using namespace utilities;


using H5::H5File;
using H5::Group;

using utilities::Attr_list_hdf;
using utilities::Md_store;

using std::cerr;
using std::endl;


Filter_basic::Filter_basic():
  e_cut_(0),rg_cut_(0),shift_cut_(0),wrap_(NULL)
{
}



void Filter_basic::init(const std::string & fname,int comp_num)
{
  H5File file = H5File( fname, H5F_ACC_RDONLY );
  Group  group = file.openGroup("/parameters/" + format_dset_name(utilities::D_XPOS,comp_num));
  
  
  Attr_list_hdf attr_list(&group);
  if(attr_list.contains_attr("e_cut"))
    attr_list.get_value("e_cut",e_cut_);
  else
    cerr<<"does not contain c_cut parameter, using default: e_cut = "<<e_cut_<<endl;
  if(attr_list.contains_attr("rg_cut"))
    attr_list.get_value("rg_cut",rg_cut_);
  else
    cerr<<"does not contain c_cut parameter, using default: rg_cut = "<<rg_cut_<<endl;
  if(attr_list.contains_attr("shift_cut"))
    attr_list.get_value("shift_cut",shift_cut_);
  else
    cerr<<"does not contain c_cut parameter, using default: shift_cut = "<<shift_cut_<<endl;
}


void Filter_basic::init(float ecut,float rg_cut,float shift_cut)
{
  e_cut_ = ecut;
  rg_cut_ = rg_cut;
  shift_cut_ = shift_cut;
    
}



bool Filter_basic::operator()(int ind, int frame)const
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

Md_store Filter_basic::get_parameters()const
{
  Md_store tmp;
  tmp.add_element("e_cut",e_cut_);
  tmp.add_element("rg_cut",rg_cut_);
  tmp.add_element("shift_cut",shift_cut_);
  
  return Md_store(tmp);
}

Filter * filter_factory(const Md_store & filter_prams)
{
  bool has_e = filter_prams.contains_key("e_cut");
  bool has_rg = filter_prams.contains_key("rg_cut");
  bool has_s = filter_prams.contains_key("shift_cut");
  bool has_I_min = filter_prams.contains_key("I_min_cut");
  if(has_I_min && has_e && has_rg && has_s )
  {
    Filter_ers* F = new utilities::Filter_ers();
    F->init(filter_prams);
    return (Filter *) F;
    
  }

  return (Filter *) new Filter_trivial();
  
}
