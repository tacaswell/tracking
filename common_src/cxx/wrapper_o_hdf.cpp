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
#include <sstream>
#include <stdexcept>


#include "particle_track.h"


#include "attr_list_hdf.h"
#include "read_config.h"
#include "track_box.h"


#include "wrapper_o_hdf.h"
#include "wrapper_o_hdf_group.h"
#include "H5Cpp.h"


//using namespace H5;

using utilities::Wrapper_o_hdf;
using utilities::Wrapper_o_hdf_group;
using utilities::Attr_list_hdf;
using utilities::Read_config;





using std::string;
using std::set;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::complex;
using std::logic_error;
using std::runtime_error;

using H5::H5File;
using H5::Group;
using H5::DataSpace;
using H5::H5File;
using H5::Attribute;
using H5::PredType;
using H5::CompType;
using H5::DataSet;
using H5::DSetCreatPropList;


using utilities::D_TYPE;

using tracking::particle;


Wrapper_o_hdf::Wrapper_o_hdf(const string& file_name,
			     const set<D_TYPE>& d_add,
			     int comp_number,
			     Wrapper_o_hdf::FILE_T file_type,
			     const string & group_prefix):
  part_count_(0),
  wrapper_open_(false),
  group_open_(false),
  group_index_(-1),
  file_type_(file_type),
  file_(NULL),
  file_name_(file_name),
  group_name_(""),
  d_types_add_(d_add),
  d_types_check_(),
  d_types_already_(),
  group_prefix_(group_prefix),
  current_group_(NULL),
  comp_number_(comp_number)
{
  
  // add logic checks to make sure the three bools are consistent
  if(file_type_ == APPEND_FILE)
    new_indexing_ = false;
  else
    new_indexing_ = true;
  
  
  


}

void Wrapper_o_hdf::initialize_wrapper()
{
  
  try
  {
    
    if(file_type_ == NEW_FILE_OVR)
      file_ = new H5File(file_name_,H5F_ACC_TRUNC);
    else if(file_type_ == NEW_FILE)
      file_ = new H5File(file_name_,H5F_ACC_EXCL);
    else if(file_type_ == APPEND_FILE || file_type_ == FILL_FILE)
      file_ = new H5File(file_name_,H5F_ACC_RDWR);
  }
  catch(...)
  {
    throw runtime_error("failure to create or open file");
  }
  // this needs to go here because the add 
  wrapper_open_ = true;
  
  if(file_type_ != APPEND_FILE)
  {
    dset_pram_group_ = new Group(file_->createGroup("parameters"));
    add_meta_data_root("version",1);
  }
  else
    dset_pram_group_ = new Group(file_->openGroup("parameters"));
  
    
    
  
  

  for(set<D_TYPE>::iterator it = d_types_add_.begin();
      it !=d_types_add_.end();++it)
    dset_pram_group_->createGroup(format_dset_name(*it,comp_number_));
  
  
  

}

void Wrapper_o_hdf::open_group(int group_indx,int p_count,int wrapper_size)
{
  if (wrapper_size <0)
    wrapper_size = p_count;
  
  if(wrapper_open_)
  {
    if(group_open_)
      throw logic_error("wrapper_o_hdf:already have a group open");

    Wrapper_o_hdf_group::GROUP_T g_type;
    switch(file_type_)
    {
    case APPEND_FILE:
      g_type = Wrapper_o_hdf_group::APPEND_GROUP;
      break;
    case NEW_FILE:
    case NEW_FILE_OVR:
      g_type = 	Wrapper_o_hdf_group::NEW_GROUP;
      break;
    case FILL_FILE:
      g_type = Wrapper_o_hdf_group::FILL_GROUP;
      break;
    default:
      g_type = Wrapper_o_hdf_group::ERROR;
      
    }
    

    current_group_ = new Wrapper_o_hdf_group(file_,
					     format_name_(group_indx),
					     d_types_add_,
					     p_count,
					     wrapper_size,
					     comp_number_,
					     g_type);
  }

  else
    throw logic_error("wrapper_o_hdf:wrapper not open");
      
  group_open_ = true;
  

}
    

void Wrapper_o_hdf::set_all_values(const particle* p_in)
{
  if(!group_open_)
    throw logic_error("wrapper_o_hdf: no group open");
  
  current_group_->store_particle(p_in);
}


#ifdef TRACKING_FLG
void Wrapper_o_hdf::set_all_values(const tracking::Track_box * in,const utilities::Tuple<float,3> & scale) 
{
  if(!group_open_)
    throw logic_error("wrapper_o_hdf: no group open");

  
  Tuple<float,3> cord;
  float I;
  in->average_cord(cord,I);
  cord*=scale;
  current_group_->store_particle_pos(cord,I);
  
}
#endif

void Wrapper_o_hdf::add_meta_store(const Md_store * md_in)
{
  current_group_->add_meta_store(md_in);
}


void Wrapper_o_hdf::close_group()
{
  if(!group_open_)
    throw logic_error("void Wrapper_o_hdf::close_group() \n\t there is no open group ");
  
  current_group_->write_to_disk();
  delete current_group_;
  current_group_ = NULL;
  
  group_open_ = false;
}




void Wrapper_o_hdf::finalize_wrapper()
{
  if(wrapper_open_)
  {
    delete dset_pram_group_;
    dset_pram_group_ = NULL;
    
    if(group_open_)
    {
      try
      {
	close_group();
      }
      catch(...)
      {
	cout<<"Something went wrong taking apart the wrapper, but this cant throw"
	    <<" because it gets called by the destructor"<<endl;
      }
    }
    
    delete file_; 
    file_ =NULL; 
    wrapper_open_ = false; 
  } 
}

 
       
void Wrapper_o_hdf::print()const
{
  throw logic_error("not implemented");
} 


Wrapper_o_hdf::~Wrapper_o_hdf()
{
  // clean up temporary arrays
  finalize_wrapper();
  
}

const set<D_TYPE>& Wrapper_o_hdf::get_content_tpyes() const
{
  return d_types_add_;
}



string Wrapper_o_hdf::format_name_(int in)const
{
  std::ostringstream o;
  o.width(format_padding_);
  o.fill('0');
  o<<std::right<<in;
  return  group_prefix_ + o.str();
}

// root meta data
template<class T>
void local_add_meta_data_root(const std::string & key, T val,bool wrapper_open,H5File * file)
{
  if(wrapper_open)
  {
    Group * group =  new Group(file->openGroup("/"));
    Attr_list_hdf atr_lst(group);
    atr_lst.set_value(key,val);
    group->close();
    
    delete group;
  }
  else
    throw runtime_error("wrapper_o_hdf::add_meta_data_root the wrapper is not open");
}

// individual instantiations because virtual functions and templates don't get along
void Wrapper_o_hdf::add_meta_data_root(const std::string & key, float val)
{
  local_add_meta_data_root(key,val,wrapper_open_,file_);
}
void Wrapper_o_hdf::add_meta_data_root(const std::string & key, const Tuple<float,2> & val)
{
  local_add_meta_data_root(key,val,wrapper_open_,file_);
}
void Wrapper_o_hdf::add_meta_data_root(const std::string & key, const Tuple<float,3> & val)
{
  local_add_meta_data_root(key,val,wrapper_open_,file_);
}
void Wrapper_o_hdf::add_meta_data_root(const std::string & key,  const std::string & val)
{
  local_add_meta_data_root(key,val,wrapper_open_,file_);
}
void Wrapper_o_hdf::add_meta_data_root(const std::string & key, int val)
{
  local_add_meta_data_root(key,val,wrapper_open_,file_);
}


// group (frame) level meta data 
template<class T>
void local_add_meta_data(const std::string & key, T val,bool group_open, Wrapper_o_hdf_group * current_group)
{
  if(group_open)
    current_group->set_meta_data(key,val);
  else
    throw runtime_error("wrapper_o_hdf::add_meta_data there is no open group");
}


void Wrapper_o_hdf::add_meta_data(const std::string & key, float val)
{
  local_add_meta_data(key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data(const std::string & key, const Tuple<float,2> & val)
{
  local_add_meta_data(key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data(const std::string & key, const Tuple<float,3> & val)
{
  local_add_meta_data(key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data(const std::string & key,  const std::string & val)
{
  local_add_meta_data(key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data(const std::string & key, int val)
{
  local_add_meta_data(key,val,group_open_,current_group_);
}

  
void Wrapper_o_hdf::add_meta_data_comp(const std::string & key, float val)
{
  string tmp_key = format_name(key,comp_number_);
  local_add_meta_data(tmp_key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data_comp(const std::string & key, const Tuple<float,2> & val){
  string tmp_key = format_name(key,comp_number_);
  local_add_meta_data(tmp_key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data_comp(const std::string & key, const Tuple<float,3> & val){
  string tmp_key = format_name(key,comp_number_);
  local_add_meta_data(tmp_key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data_comp(const std::string & key,  const std::string & val){
  string tmp_key = format_name(key,comp_number_);
  local_add_meta_data(tmp_key,val,group_open_,current_group_);
}
void Wrapper_o_hdf::add_meta_data_comp(const std::string & key, int val){
  string tmp_key = format_name(key,comp_number_);
  local_add_meta_data(tmp_key,val,group_open_,current_group_);
}


// 
template <class T> 
void local_add_meta(const std::string & key, T val,D_TYPE dset_type,bool wrapper_open,int comp_number,Group * dset_pram_group )
{
  if(wrapper_open)
  {
    H5::Group dgroup = dset_pram_group->openGroup(format_dset_name(dset_type,comp_number));
    utilities::Attr_list_hdf atr_lst(&dgroup);
    atr_lst.set_value(key,val);
    
  }
  else
    throw std::logic_error("void Wrapper_o_hdf::add_meta_data_(const std::string & key, T val,D_TYPE dset_type) \n\t can't add meta data while the warpper is closed");
}


void Wrapper_o_hdf::add_meta_data(const std::string & key, int val,D_TYPE dset_type)
{
  local_add_meta(key,val,dset_type,wrapper_open_,comp_number_,dset_pram_group_);
}

void Wrapper_o_hdf::add_meta_data(const std::string & key, float val,D_TYPE dset_type)
{
  local_add_meta(key,val,dset_type,wrapper_open_,comp_number_,dset_pram_group_);
  
}

void Wrapper_o_hdf::add_meta_data(const std::string & key, const string& val,D_TYPE dset_type)
{
  local_add_meta(key,val,dset_type,wrapper_open_,comp_number_,dset_pram_group_);
  
}
void Wrapper_o_hdf::add_meta_data(const std::string & key, const Tuple<float,2>& val,D_TYPE dset_type)
{
  local_add_meta(key,val,dset_type,wrapper_open_,comp_number_,dset_pram_group_);
  
}
void Wrapper_o_hdf::add_meta_data(const std::string & key, const Tuple<float,3>& val,D_TYPE dset_type)
{
  local_add_meta(key,val,dset_type,wrapper_open_,comp_number_,dset_pram_group_);
}



void Wrapper_o_hdf::add_meta_data_list(const Read_config & config, const std::set<D_TYPE> & d_types)
{
  int pram_sz = config.size();
  set<D_TYPE>::const_iterator end= d_types.end();
  int tmpi =0;
  float tmpf=0;
  string tmps="";
  
  for(set<D_TYPE>::const_iterator it = d_types.begin();
      it != end; ++it)
  {
    for(int j = 0; j<pram_sz; ++j)
    {
      V_TYPE type = config.get_type(j);
      string tmp_key = config.get_key(j);
      
      switch(type)
      {
      case utilities::V_INT:
	config.get_value(j,tmpi);
	add_meta_data(tmp_key,tmpi,*it);
	break;
      case utilities::V_FLOAT:
	config.get_value(j,tmpf);
	add_meta_data(tmp_key,tmpf,*it);
	break;
      case utilities::V_STRING:
	config.get_value(j,tmps);
	add_meta_data(tmp_key,tmps,*it);
	break;
      default:
	cerr<<"not of type supported in meta data: "<<tmp_key<<','<<VT2str_s(type)<<endl;
      }
    }
      
  }
    
    

}
