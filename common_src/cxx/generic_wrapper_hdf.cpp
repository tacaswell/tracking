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


#include <iostream>
#include <stdexcept>

using utilities::Generic_wrapper_hdf;

using std::logic_error;

using namespace H5;
using std::string;

  
Generic_wrapper_hdf::Generic_wrapper_hdf(std::string fname, bool add_to_file):
  file_name_(fname),add_to_file_(add_to_file)
{
}

Generic_wrapper_hdf::~Generic_wrapper_hdf()
{
  if(wrapper_open_)
    close_wrapper();
}


void Generic_wrapper_hdf::open_wrapper()
{
  // if there is a file to add to open it read/write
  if(add_to_file_)
    file_ = new H5File(file_name_,H5F_ACC_RDWR);
  else
    // if there is not a file to add to try to make a new one
    // and die if there is already a file
    //    file_ = new H5File(file_name_,H5F_ACC_EXCL);
    file_ = new H5File(file_name_,H5F_ACC_TRUNC);
  
  wrapper_open_ = true;
  
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
  // make data set

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
  
  
  DataSet dset = group_ ->createDataSet(name,hdf_type,dspace);
  dset.write(data,mem_type,dspace,dspace);
  
  // shove in data

  // close everything

}

  
void Generic_wrapper_hdf::add_meta_data(const std::string & key, float val)
{
  if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->set_value(key,val);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Triple<float> & val)
{
    if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->set_value(key,val);

}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Pair<float>& val)
{
  if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->set_value(key,val);


}
void Generic_wrapper_hdf::add_meta_data(const std::string & key,  const std::string & val)
{
  if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->set_value(key,val);
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, int val)
{
  if(!group_open_)
    throw logic_error("generic_wrapper_hdf:: can't add to a closed group");
  group_attrs_->set_value(key,val);
}

void Generic_wrapper_hdf::add_meta_data(const std::string & key, float val,const std::string & dset_name)
{
  throw "need to write";
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Triple<float> & val,const std::string & dset_name)
{
  throw "need to write";
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, const Pair<float>& val,const std::string & dset_name)
{
  throw "need to write";
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name)
{
  throw "need to write";
}
void Generic_wrapper_hdf::add_meta_data(const std::string & key, int val,const std::string & dset_name)
{
  throw "need to write";
}


