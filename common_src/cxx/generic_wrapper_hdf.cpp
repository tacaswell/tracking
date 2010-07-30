//Copyright 2009-2010 Thomas A Caswell
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

#include "generic_wrapper_hdf.h"
#include "H5Cpp.h"
#include "attr_list_hdf.h"

#include "md_store.h"

#include <iostream>
#include <stdexcept>

using utilities::Generic_wrapper_hdf;

using std::logic_error;

using namespace H5;
using std::string;
using utilities::Md_store;

  
Generic_wrapper_hdf::Generic_wrapper_hdf(std::string fname, bool add_to_file):
  file_name_(fname),wrapper_open_(false),group_open_(false),dset_open_(false),add_to_file_(add_to_file),
  file_(NULL),group_(NULL),group_attrs_(NULL)
{
}

Generic_wrapper_hdf::~Generic_wrapper_hdf()
{
  if(wrapper_open_)
    close_wrapper();
}


void Generic_wrapper_hdf::open_wrapper()
{
  if(!wrapper_open_)
  {
    // if there is a file to add to open it read/write
    if(add_to_file_)
    {
      try
      {
	file_ = new H5File(file_name_,H5F_ACC_RDWR);
      }
      catch(FileIException & error)
      {
	file_ = new H5File(file_name_,H5F_ACC_EXCL);
      }
    }
    
    else
    {
      // if there is not a file to add to try to make a new one
      // and die if there is already a file
      file_ = new H5File(file_name_,H5F_ACC_EXCL);
      //file_ = new H5File(file_name_,H5F_ACC_TRUNC);
    }
    wrapper_open_ = true;
  }
  
}


void Generic_wrapper_hdf::close_wrapper()
{
  if(group_open_)
    close_group();
  
  if(wrapper_open_)
  {
    delete file_;
    file_ = NULL;
    wrapper_open_= false;
  }
  
}


void Generic_wrapper_hdf::open_group(const string & name )
{

  if(!wrapper_open_)
    throw "generic_wrapper_hdf: wrapper not open";
  
  if(name=="none")
    throw "generic_wrapper_hdf: need to give a name";
  
  if(group_open_)
    throw "generic_wrapper_hdf::open_group: there is already an open group";
  

  try
  {
    group_ = new Group(file_->createGroup(name));
  }
  catch(...)
  {
    throw "generic_wrapper_hdf: group creation failed";
  }

  group_attrs_ = new Attr_list_hdf(group_);
  
 
  group_open_ = true;
  
}

void Generic_wrapper_hdf::close_group()
{

  
  if(group_open_)
  {
    delete group_attrs_;
    group_attrs_ = NULL;
    delete group_;
    group_ = NULL;
    group_open_ = false;
  }
  
}

void Generic_wrapper_hdf::add_dset(int rank, const int * dims, V_TYPE type, const void * data,
				   const std::string & name  )
{
  if(name =="none")
    throw "generic_wrapper_hdf: requires real name";

  hsize_t hdims[rank];
  for(int j = 0;j<rank;++j)
    hdims[j] = dims[j];

  // make dspace
  DataSpace dspace(rank,hdims);

  

  
  // sort out type


  DataType hdf_type,mem_type;
  
  switch(type)
  {
  case V_INT:
    hdf_type = PredType::NATIVE_INT;
    mem_type = PredType::NATIVE_INT;
    break;
  case V_FLOAT:
    hdf_type = PredType::NATIVE_FLOAT;
    mem_type = PredType::NATIVE_FLOAT;
    break;
  default:
    throw "generic_wrapper_hdf: un implemented types";
  }
  
  // make data set
  DataSet dset = group_ ->createDataSet(name,hdf_type,dspace);
  // shove in data
  dset.write(data,mem_type,dspace,dspace);
  
  // close everything is taken care of as all variables on stack

}

  
void Generic_wrapper_hdf::add_meta_data(const std::string & key, float val)
{
  add_meta_data_priv(key,val);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Tuple<float,3> & val)
{
  add_meta_data_priv(key,val);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Tuple<float,2>& val)
{
  add_meta_data_priv(key,val);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key,  const std::string & val)
{
  add_meta_data_priv(key,val);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, int val)
{
  add_meta_data_priv(key,val);
}

void Generic_wrapper_hdf::add_meta_data(const std::string & key, float val,const std::string & dset_name)
{
  add_meta_data_priv(key,val,dset_name);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name)
{ 
  add_meta_data_priv(key,val,dset_name);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name)
{
  add_meta_data_priv(key,val,dset_name);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name)
{
  add_meta_data_priv(key,val,dset_name);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, int val,const std::string & dset_name)
{
  add_meta_data_priv(key,val,dset_name);
}

void Generic_wrapper_hdf::add_meta_data(const Md_store * md_store)
{
  
  unsigned int num_entries = md_store->size();
  int tmpi;
  float tmpf;
  string tmps;
  for(unsigned int j = 0; j<num_entries; ++j)
  {
    string key = md_store->get_key(j);
    switch(md_store->get_type(j))
    {
      // integer data
    case V_INT:
    
      add_meta_data_priv(key,md_store->get_value(j,tmpi));
      break;
      // float data
    case V_FLOAT:
    
      add_meta_data_priv(key,md_store->get_value(j,tmpf));
      break;
      // complex data
    case V_STRING:
    
      add_meta_data_priv(key,md_store->get_value(j,tmps));
      break;
    default:
      throw logic_error("should not have hit default");
      
    }
  

  }
}

void Generic_wrapper_hdf::add_meta_data(const Md_store * md_store,const string & dset)
{
  
  unsigned int num_entries = md_store->size();
  int tmpi;
  float tmpf;
  string tmps;
  for(unsigned int j = 0; j<num_entries; ++j)
  {
    string key = md_store->get_key(j);
    switch(md_store->get_type(j))
    {
      // integer data
    case V_INT:
    
      add_meta_data_priv(key,md_store->get_value(j,tmpi),dset);
      break;
      // float data
    case V_FLOAT:
    
      add_meta_data_priv(key,md_store->get_value(j,tmpf),dset);
      break;
      // complex data
    case V_STRING:
    
      add_meta_data_priv(key,md_store->get_value(j,tmps),dset);
      break;
    default:
      throw logic_error("should not have hit default");
      
    }
  

  }
}


template<class T>
void Generic_wrapper_hdf::add_meta_data_priv(const std::string & key, const T& val,const std::string & dset_name)
{
  
  if(!group_open_)
      throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  // open data set
  DataSet dset = group_ ->openDataSet(dset_name);
  // make attr wrapper for group
  Attr_list_hdf dset_attr(&dset);
  // shove in meta data
  dset_attr.set_value(key,val);
}

template<class T>
void Generic_wrapper_hdf::add_meta_data_priv(const std::string & key, const T& val)
{
  if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->set_value(key,val);
}

