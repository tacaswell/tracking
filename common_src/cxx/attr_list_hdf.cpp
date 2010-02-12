//Copyright 2010 Thomas A Caswell
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

#include "attr_list_hdf.h"
#include "H5Cpp.h"
#include <iostream>
#include <stdexcept>
using utilities::Attr_list_hdf;

using H5::H5Object;
using H5::Attribute;
using H5::PredType;
using H5::DataSpace;

using std::string;
using std::list;
using std::cout;
using std::cerr;

using std::endl;
using std::invalid_argument;


void add_key(H5Object & a, std::string b, void * d)
{
  list<string> * data = (list<string>*)d;
  data->push_back(b);
}


Attr_list_hdf::Attr_list_hdf(H5Object * obj):obj_(obj)
{
  unsigned int n =0;
  obj_->iterateAttrs(add_key,&n,&keys_);
}

Attr_list_hdf::~Attr_list_hdf()
{}

bool Attr_list_hdf::contains_attr(const std::string & key) const
{
  list<string>::const_iterator last = keys_.end();
  for(list<string>::const_iterator it = keys_.begin();it!=last;++it)
    if(key.compare(*it) == 0)
      return true;
  return false;
}


int Attr_list_hdf::get_value(const std::string & key, int & value_out) const 
{
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t type_class = tmpa.getTypeClass();
  H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  if(type_class == H5T_INTEGER && space_type == H5S_SCALAR )
    tmpa.read(PredType::NATIVE_INT,&value_out);
  else
    throw invalid_argument("output does not match attribute dtype");
  
  
  return value_out;

}

void Attr_list_hdf::set_value(const std::string & key,  const int &   value_in,bool over_write) 
{
  if(contains_attr(key))
  {
    if( over_write)
    {
      Attribute  tmpa =  Attribute(obj_->openAttribute(key));
      H5T_class_t type_class = tmpa.getTypeClass();
      H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  
      if(type_class == H5T_INTEGER && space_type == H5S_SCALAR )
	tmpa.write(PredType::NATIVE_INT,&value_in);
      else
      	throw invalid_argument("output does not match attribute dtype");
    }
    else
      throw invalid_argument("attribute name already exists");
  }
  else
  {
    DataSpace dspace =  DataSpace(0,NULL);
    Attribute  tmpa =Attribute(obj_->createAttribute(key,PredType::STD_I32LE,dspace));
    tmpa.write(PredType::NATIVE_INT,&value_in);
    keys_.push_back(key);
  }
}


float Attr_list_hdf::get_value(const std::string & key, float & value_out) const 
{
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t type_class = tmpa.getTypeClass();
  H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  if(type_class == H5T_FLOAT && space_type == H5S_SCALAR )
    tmpa.read(PredType::NATIVE_FLOAT,&value_out);
  else
    throw invalid_argument("output does not match attribute dtype");
  
  
  return value_out;

}

void Attr_list_hdf::set_value(const std::string & key,  const float &   value_in,bool over_write) 
{
  if(contains_attr(key))
  {
    if( over_write)
    {
      Attribute  tmpa =  Attribute(obj_->openAttribute(key));
      H5T_class_t type_class = tmpa.getTypeClass();
      H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  
      if(type_class == H5T_FLOAT && space_type == H5S_SCALAR )
	tmpa.write(PredType::NATIVE_FLOAT,&value_in);
      else
      	throw invalid_argument("output does not match attribute dtype");
    }
    else
      throw invalid_argument("attribute name already exists");
  }
  else
  {
    DataSpace dspace =  DataSpace(0,NULL);
    Attribute  tmpa =Attribute(obj_->createAttribute(key,PredType::NATIVE_FLOAT,dspace));
    tmpa.write(PredType::NATIVE_FLOAT,&value_in);
    keys_.push_back(key);
  }
}






void Attr_list_hdf::print()const 
{
  for(list<string>::const_iterator it = keys_.begin();  it != keys_.end();++it)
  {
    cout<<*it<<endl;
    
  }
}


