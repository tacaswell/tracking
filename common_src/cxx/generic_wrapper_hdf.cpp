//Copyright 2009-2012 Thomas A Caswell
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
#include "tuple.h"
#include <iostream>
#include <stdexcept>

using utilities::Generic_wrapper_hdf;

using std::logic_error;
using std::runtime_error;


using H5::H5File;
using H5::Group;
using H5::DataSpace;
using H5::H5File;
using H5::Attribute;
using H5::PredType;
using H5::DataSet;
using H5::DSetCreatPropList;
using H5::Exception;
using H5::GroupIException;
using H5::FileIException;
using H5::DataType;
using H5::FileAccPropList;
using H5::FileCreatPropList;

using std::string;
using utilities::Md_store;
using utilities::Tuple;

const static unsigned int CSIZE = 500;
  
Generic_wrapper_hdf::Generic_wrapper_hdf(std::string fname, F_TYPE f_type):
  file_name_(fname),wrapper_open_(false),group_open_(false),dset_open_(false),f_type_(f_type),
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
  Exception::dontPrint();
  if(!wrapper_open_)
  {
  
    switch(f_type_)
    {
    case F_DISK_RDWR:
      // if there is a file to add to open it read/write
      try
      {
	file_ = new H5File(file_name_,H5F_ACC_RDWR);
      }
      catch(FileIException & error)
      {
	file_ = new H5File(file_name_,H5F_ACC_EXCL);
      }
      break;
    case F_DISK_EXCL:
      // if there is not a file to add to try to make a new one
      // and die if there is already a file
      file_ = new H5File(file_name_,H5F_ACC_EXCL);
      break;
    case F_DISK_TRUNC:
      // walk on what ever is there
      file_ = new H5File(file_name_,H5F_ACC_TRUNC);
      break;
    case F_MEM:
      // set up the access list 
      FileAccPropList p_acl = FileAccPropList();
      // set the driver to be core with out a backing, ie memory only
      p_acl.setCore(5000*sizeof(float),false);
      // open the memory based file
      file_ = new H5File(file_name_,H5F_ACC_TRUNC,FileCreatPropList::DEFAULT,p_acl);
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
    throw runtime_error("generic_wrapper_hdf: wrapper not open");
  
  if(group_open_)
    throw runtime_error("generic_wrapper_hdf::open_group: there is already an open group");
  
  bool exists = false;

    

  // if the group does not exist, create it
  try
  {
    group_ = new Group(file_->createGroup(name));
  }
  catch(FileIException & e)
  {
    group_ = new Group(file_->openGroup(name));
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

void Generic_wrapper_hdf::add_dset(int rank, const unsigned int * dims, V_TYPE type, const void * data,
				   const std::string & name  )
{

  hsize_t hdims[rank];
  for(int j = 0;j<rank;++j)
    hdims[j] = dims[j];

  // make dspace
  DataSpace dspace(rank,hdims);

  

  
  // sort out type


  DataType hdf_type,mem_type;
  DSetCreatPropList plist;
  int fill_value_i = -31415;
  unsigned int fill_value_ui = 0;
  float fill_value_f = -3.1415;
  switch(type)
  {
  case V_INT:
  
    hdf_type = PredType::NATIVE_INT;
    mem_type = PredType::NATIVE_INT;
    plist.setFillValue(hdf_type,&fill_value_i);
    break;
  case V_FLOAT:
    
    hdf_type = PredType::NATIVE_FLOAT;
    mem_type = PredType::NATIVE_FLOAT;
    plist.setFillValue(hdf_type,&fill_value_f);
    break;
  case V_UINT:
    hdf_type = PredType::NATIVE_UINT;
    mem_type = PredType::NATIVE_UINT;
    plist.setFillValue(hdf_type,&fill_value_ui);
    break;
  case V_BOOL:
  case V_TIME:
  case V_GUID:
  case V_ERROR:
  case V_COMPLEX:
  case V_STRING:
    throw logic_error("generic_wrapper_hdf: un implemented types");
  }
  
  
  // if the list is big enough, us compression
  if(rank ==1 && *hdims > CSIZE*5)
  {
    hsize_t csize = CSIZE;
    plist.setChunk(1,&csize);
    plist.setSzip(H5_SZIP_NN_OPTION_MASK,10);
  }
  
  DataSet dset;
  if(!group_open_ || name[0] == '/')
  {
    dset = file_ ->createDataSet(name,hdf_type,dspace,plist);  
  }
  else if(group_)
  {
    dset = group_ ->createDataSet(name,hdf_type,dspace,plist);  
  }
  else
  {
    throw runtime_error("gave relative path name with no open group");
  }
  
  // make data set
  
  
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
void Generic_wrapper_hdf::add_meta_data(const std::string & key, unsigned int val)
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

  
float Generic_wrapper_hdf::get_meta_data(const std::string & key, float& val)
{
  return get_meta_data_priv(key,val);
}
Tuple<float,3> Generic_wrapper_hdf::get_meta_data(const std::string & key, Tuple<float,3> & val)
{
  return get_meta_data_priv(key,val);
}
Tuple<float,2> Generic_wrapper_hdf::get_meta_data(const std::string & key,  Tuple<float,2>& val)
{
  return get_meta_data_priv(key,val);
}
std::string Generic_wrapper_hdf::get_meta_data(const std::string & key,   std::string & val)
{
  return get_meta_data_priv(key,val);
}
int Generic_wrapper_hdf::get_meta_data(const std::string & key, int& val)
{
  return get_meta_data_priv(key,val);
}
unsigned int Generic_wrapper_hdf::get_meta_data(const std::string & key, unsigned int& val)
{
  return get_meta_data_priv(key,val);
}

float Generic_wrapper_hdf::get_meta_data(const std::string & key, float& val,const std::string & dget_name)
{
  return get_meta_data_priv(key,val,dget_name);
}
Tuple<float,3> Generic_wrapper_hdf::get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dget_name)
{ 
  return get_meta_data_priv(key,val,dget_name);
}
Tuple<float,2> Generic_wrapper_hdf::get_meta_data(const std::string & key,  Tuple<float,2>& val,const std::string & dget_name)
{
  return get_meta_data_priv(key,val,dget_name);
}
std::string Generic_wrapper_hdf::get_meta_data(const std::string & key,   std::string & val,const std::string & dget_name)
{
  return get_meta_data_priv(key,val,dget_name);
}
int  Generic_wrapper_hdf::get_meta_data(const std::string & key, int & val,const std::string & dget_name)
{
  return get_meta_data_priv(key,val,dget_name);
}


void Generic_wrapper_hdf::add_meta_data(const Md_store * md_store)
{
  
  unsigned int num_entries = md_store->size();
  int tmpi;
  unsigned int tmpui;
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
      
    case V_UINT:
      add_meta_data_priv(key,md_store->get_value(j,tmpui));
      break;
      
    case V_COMPLEX:
    case V_ERROR:
    case V_BOOL:
    case V_TIME:
    case V_GUID:
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
  DataSet dset;
  // open data set  
  if(!group_open_ || dset_name[0] == '/')
  {
    dset = file_->openDataSet(dset_name);
  }
  else if(group_)
  {
    dset = group_->openDataSet(dset_name);
  }
  else
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  
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

template<class T>
T Generic_wrapper_hdf::get_meta_data_priv(const std::string & key,  T& val,const std::string & dset_name)
{
  DataSet dset;
  // open data set  
  if(!group_open_ || dset_name[0] == '/')
  {
    dset = file_->openDataSet(dset_name);
  }
  else if(group_)
  {
    dset = group_->openDataSet(dset_name);
  }
  else
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  
  // make attr wrapper for group
  Attr_list_hdf dset_attr(&dset);
  // shove in meta data
  dset_attr.get_value(key,val);
  return val;
  
}

template<class T>
T  Generic_wrapper_hdf::get_meta_data_priv(const std::string & key, T& val)
{
  if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->get_value(key,val);
  return val;
  
}
