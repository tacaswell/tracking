//Copyright 2009-2011 Thomas A Caswell
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

using std::cerr;
using std::endl;


using std::string;
using std::vector;


using std::runtime_error;


// ers filter
Filter_ers::Filter_ers():
  e_cut_(0),rg_cut_(0),shift_cut_(0),wrap_(NULL)
{
}



void Filter_ers::init(const Md_store & md_store)
{
  md_store.get_value("e_cut",e_cut_);
  md_store.get_value("rg_cut",rg_cut_ );
  md_store.get_value("shift_cut",shift_cut_);
  shift_cut_sqr_ = shift_cut_*shift_cut_;
  
}



bool Filter_ers::operator()(int ind, int frame)const
{
  float dx,dy;
  if(wrap_->get_value(dx,ind,D_E,frame)>e_cut_ || wrap_->get_value(dy,ind,D_R2,frame)>rg_cut_)
    return false;
  wrap_->get_value(dx,ind,D_DX,frame);
  wrap_->get_value(dy,ind,D_DY,frame);

  if (dx ==0 || dy ==0)
  {
    return false;
  }
  
  
  if((dx*dx +dy*dy) >shift_cut_sqr_)
    return false;
  return true;
}

Md_store Filter_ers::get_parameters()const
{
  Md_store tmp;
  tmp.add_element("e_cut",e_cut_);
  tmp.add_element("rg_cut",rg_cut_);
  tmp.add_element("shift_cut",shift_cut_);
  
  return Md_store(tmp);
}

// ersi filter
Filter_ersI::Filter_ersI():
  e_cut_(0),rg_cut_(0),shift_cut_(0),I_min_cut_(0),wrap_(NULL)
{
}



void Filter_ersI::init(const Md_store & md_store)
{
  md_store.get_value("e_cut",e_cut_);
  md_store.get_value("rg_cut",rg_cut_ );
  md_store.get_value("shift_cut",shift_cut_);
  md_store.get_value("I_min_cut",I_min_cut_);
}



bool Filter_ersI::operator()(int ind, int frame)const
{
  float tmpf;
  if(wrap_->get_value(tmpf,ind,D_E,frame)  > e_cut_ ||
     wrap_->get_value(tmpf,ind,D_R2,frame) > rg_cut_ ||
     wrap_->get_value(tmpf,ind,D_I,frame)  < I_min_cut_)
    return false;
  float x,y;
  wrap_->get_value(x,ind,D_DX,frame);
  wrap_->get_value(y,ind,D_DY,frame);

  if((x*x +y*y) >shift_cut_*shift_cut_)
    return false;
  return true;
}

Md_store Filter_ersI::get_parameters()const
{
  Md_store tmp;
  tmp.add_element("e_cut",e_cut_);
  tmp.add_element("rg_cut",rg_cut_);
  tmp.add_element("shift_cut",shift_cut_);
  
  return Md_store(tmp);
}

Md_store utilities::extract_prams(const std::string & fname,int comp_num,const vector<string> &pram_list)
{
  // hdf stuff
  H5File file = H5File( fname, H5F_ACC_RDONLY );
  Group  group = file.openGroup("/parameters/" + format_dset_name(utilities::D_XPOS,comp_num));
  Attr_list_hdf attr_list(&group);

  
  
  Md_store filter_prams;    
  
  
  for(vector<string>::const_iterator it = pram_list.begin();
      it <pram_list.end(); ++it)
  {
    string pram = *it;
    if(!attr_list.contains_attr(pram))
      throw runtime_error("the field " + pram + " is not in the file");
    utilities::V_TYPE vtype = attr_list.get_type(pram);
    int tmpi;
    unsigned int tmpui;
    float tmpf;
    switch(vtype)
    {
    case V_UINT:
      filter_prams.add_element(pram.c_str(),attr_list.get_value(pram,tmpui));
      break;
    case V_INT:
      filter_prams.add_element(pram.c_str(),attr_list.get_value(pram,tmpi));
      break;
    case V_FLOAT:
      filter_prams.add_element(pram.c_str(),attr_list.get_value(pram,tmpf));
      break;
    case V_ERROR:
    case V_COMPLEX:
    case V_STRING:
    case V_BOOL:
    case V_TIME:
    case V_GUID:
      throw runtime_error("the field " + pram + " is of type " + VT2str_s(vtype) + " which is not implemented yet.");
    }
    
  }
  
  // return a copy of the assembled parameters 
  return Md_store(filter_prams);
  
}

// helper functions
Filter * utilities::filter_factory(const Md_store & filter_prams)
{
  bool has_e = filter_prams.contains_key("e_cut");
  bool has_rg = filter_prams.contains_key("rg_cut");
  bool has_s = filter_prams.contains_key("shift_cut");
  bool has_I_min = filter_prams.contains_key("I_min_cut");
  if(has_e && has_rg && has_s )
  {
    if(has_I_min)
    {
      Filter_ersI* F = new utilities::Filter_ersI();
      F->init(filter_prams);
      return (Filter *) F;
    }
    else
    {
      Filter_ers* F = new utilities::Filter_ers();
      F->init(filter_prams);
      return (Filter *) F;
    }
    
  }

  return (Filter *) new Filter_trivial();
  
}
