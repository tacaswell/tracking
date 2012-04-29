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
using H5::StrType;
using H5::IntType;


using std::string;
using std::list;
using std::cout;
using std::cerr;
using std::runtime_error;

using std::endl;
using std::invalid_argument;

// This function needs to match a signature in hdf for the iteration
// functions.  This will raise an error that nothing can be done about
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
  {
    IntType int_type = tmpa.getIntType();
    if(int_type.getSign() == H5T_SGN_2)
      tmpa.read(PredType::NATIVE_INT,&value_out);
    else if(int_type.getSign() == H5T_SGN_NONE)
    {
      unsigned int tmp_int;
      
      // extract as a normal int
      tmpa.read(PredType::NATIVE_UINT,&tmp_int);
      // check limits
      if (tmp_int >INT_MAX)
	throw runtime_error("attr_list_hdf:: to big to be cast to a signed integer " + key );
      // cast
      value_out = (int) tmp_int;
    }
    else
      throw runtime_error("attr_list_hdf :: invalid sign type  " + key );
    
  }
  else
    throw invalid_argument("output does not match attribute dtype " + key );
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
      	throw invalid_argument("output does not match attribute dtype " + key );
    }
    else
    {
      // check to see if they match
      int tmp = 0;
      get_value(key,tmp);
      if(tmp != value_in)
	throw invalid_argument("attribute name already exists and values don't match: int " + key );
    }
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
    throw invalid_argument("output does not match attribute dtype " + key );
  
  
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
      	throw invalid_argument("output does not match attribute dtype " + key );
    }
        else
    {
      // check to see if they match
      float tmp = 0;
      get_value(key,tmp);
      if(tmp != value_in)
      {
	cerr<<"setting attribute: "<<key<<endl;
	cerr<<"the existing value: "<<tmp<<endl;
	cerr<<"the input value: "<<value_in<<endl;
	
	throw invalid_argument("attribute name already exists and values don't match: float " + key );
      }
    }
    
  }
  else
  {
    DataSpace dspace =  DataSpace(0,NULL);
    Attribute  tmpa =Attribute(obj_->createAttribute(key,PredType::NATIVE_FLOAT,dspace));
    tmpa.write(PredType::NATIVE_FLOAT,&value_in);
    keys_.push_back(key);
  }
}



std::string  Attr_list_hdf::get_value(const std::string & key, std::string & value_out) const 
{
  
  value_out.clear();
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t type_class = tmpa.getTypeClass();
  H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  if(type_class == H5T_STRING && space_type == H5S_SCALAR )
  {
    // cribbed from tattr.cpp
    StrType str_type = tmpa.getStrType();
    tmpa.read(str_type,value_out);
  }
  
  else
    throw invalid_argument("output does not match attribute dtype " + key );
  
  return string(value_out);

}

void Attr_list_hdf::set_value(const std::string & key,  const std::string &   value_in,bool over_write) 
{

  
  if(contains_attr(key))
  {
    if(over_write)
    {
      Attribute  tmpa =  Attribute(obj_->openAttribute(key));
      H5T_class_t type_class = tmpa.getTypeClass();
      H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  
      if(type_class == H5T_STRING && space_type == H5S_SCALAR )
      {
	obj_->removeAttr(key);
      }
      else
      	throw invalid_argument("input does not match attribute dtype " + key );
    }
    else
    {
      string tmp_string;
      get_value(key,tmp_string);
      if(tmp_string.compare(value_in)!=0)
      {
	throw invalid_argument("input does not match existing value : " 
			       + key + ": " + tmp_string +" (" + value_in + ")");
      }
      return;
      
    }

  }
  if(value_in.size()>0)
  {
    StrType str_type(0,value_in.size());
    DataSpace dspace =  DataSpace(H5S_SCALAR);
    Attribute  tmpa =Attribute(obj_->createAttribute(key,str_type,dspace));
    tmpa.write(str_type,value_in);
    keys_.push_back(key);
  }
  
}


template<int N>
utilities::Tuple<float,N> Attr_list_hdf::get_value(const std::string & key,utilities::Tuple<float,N>  & value_out) const 
{
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t type_class = tmpa.getTypeClass();
  DataSpace dspace = tmpa.getSpace();
  H5S_class_t space_type = dspace.getSimpleExtentType();
  
  if(type_class == H5T_FLOAT && 
     space_type == H5S_SIMPLE  && 
     dspace.getSimpleExtentNdims() == 1 && 
     dspace.getSimpleExtentNpoints() == Tuple<float,N>::length_)
    tmpa.read(PredType::NATIVE_FLOAT,value_out.get_ptr());
  else
    throw invalid_argument("output does not match attribute dtype: " + key);
  
  
  
  return Tuple<float,N>(value_out);

}

template<int N>
void Attr_list_hdf::set_value(const std::string & key,  const Tuple<float,N> &   value_in,bool over_write) 
{
  if(contains_attr(key))
  {
    if( over_write)
    {
      Attribute  tmpa =  Attribute(obj_->openAttribute(key));
      H5T_class_t type_class = tmpa.getTypeClass();
      DataSpace dspace = tmpa.getSpace();
      H5S_class_t space_type = dspace.getSimpleExtentType();
  
      if(type_class == H5T_FLOAT && space_type == H5S_SIMPLE  && 
	 dspace.getSimpleExtentNdims() == 1 && dspace.getSimpleExtentNpoints() == Tuple<float,N>::length_)
	tmpa.write(PredType::NATIVE_FLOAT,value_in.get_ptr());
      
      else
      	throw invalid_argument("output does not match attribute dtype " + key );
    }
    else
    {
      // check to see if they match
      Tuple<float,N> tmp ;
      get_value(key,tmp);
      if(!(tmp == value_in))
	throw invalid_argument("attribute name already exists and values don't match: Tuple<float,N> " + key );
    }
  }
  else
  {
    hsize_t dim_c = (hsize_t) Tuple<float,N>::length_;
    DataSpace dspace =  DataSpace(1,&dim_c);
    Attribute  tmpa =  obj_->createAttribute(key,PredType::NATIVE_FLOAT,dspace);
    tmpa.write(PredType::NATIVE_FLOAT,value_in.get_ptr());
  }
}





void Attr_list_hdf::remove_attr(const string & key)
{
  obj_->removeAttr(key);
  keys_.remove(key);
  
}



void Attr_list_hdf::print()const 
{
  for(list<string>::const_iterator it = keys_.begin();  it != keys_.end();++it)
  {
    cout<<*it<<endl;
    
  }
}



unsigned int Attr_list_hdf::get_value(const std::string & key, unsigned int & value_out) const 
{
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t type_class = tmpa.getTypeClass();
  H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  if(type_class == H5T_INTEGER && space_type == H5S_SCALAR )
  {
    IntType int_type = tmpa.getIntType();
    if(int_type.getSign() == H5T_SGN_NONE)
      tmpa.read(PredType::NATIVE_UINT,&value_out);
    else if(int_type.getSign() == H5T_SGN_2)
    {
      int tmp_int;
      
      // extract as a normal int
      tmpa.read(PredType::NATIVE_INT,&tmp_int);
      // check limits
      if (tmp_int <0)
	throw runtime_error("attr_list_hdf:: can not cast a negative int to an unsigned int " + key );
      // cast
      value_out = (unsigned int) tmp_int;
    }
    else
      throw runtime_error("attr_list_hdf :: invalid sign type  " + key );
  }
  
  else
    throw invalid_argument("output does not match attribute dtype " + key );
  return value_out;
}



void Attr_list_hdf::set_value(const std::string & key,  const unsigned int &   value_in,bool over_write) 
{
  if(contains_attr(key))
  {
    if( over_write)
    {
      Attribute  tmpa =  Attribute(obj_->openAttribute(key));
      H5T_class_t type_class = tmpa.getTypeClass();
      H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  
      if(type_class == H5T_INTEGER && space_type == H5S_SCALAR )
	tmpa.write(PredType::NATIVE_UINT,&value_in);
      else
      	throw invalid_argument("output does not match attribute dtype " + key );
    }
    else
    {
      // check to see if they match
      unsigned int tmp = 0;
      get_value(key,tmp);
      if(tmp != value_in)
	throw invalid_argument("attribute name already exists and values don't match: int " + key );
    }
  }
  else
  {
    DataSpace dspace =  DataSpace(0,NULL);
    Attribute  tmpa =Attribute(obj_->createAttribute(key,PredType::NATIVE_UINT,dspace));
    tmpa.write(PredType::NATIVE_UINT,&value_in);
    keys_.push_back(key);
  }
}


template 
void Attr_list_hdf::set_value(const std::string & key,  
			      const Tuple<float,2> &   
			      value_in,
			      bool over_write) ;
template 
void Attr_list_hdf::set_value(const std::string & key,
			      const Tuple<float,3> &   
			      value_in,
			      bool over_write) ;

template 
utilities::Tuple<float,2> Attr_list_hdf::get_value(const std::string & key,
						   utilities::Tuple<float,2>  & value_out) const ;

template 
utilities::Tuple<float,3> Attr_list_hdf::get_value(const std::string & key,
						   utilities::Tuple<float,3>  & value_out) const ;


bool Attr_list_hdf::get_value(const std::string & key, bool & value_out) const 
{
  int tmp = 0;
  
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t type_class = tmpa.getTypeClass();
  H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  if(type_class == H5T_INTEGER && space_type == H5S_SCALAR )
    tmpa.read(PredType::NATIVE_INT,&tmp);
  else
    throw invalid_argument("output does not match attribute dtype " + key );
  
  if(tmp == 0)
    value_out = false;
  else
    value_out = true;
  
  return value_out;

}

void Attr_list_hdf::set_value(const std::string & key,  const bool &   value_in,bool over_write) 
{
  int tmp_in = 0;
  if(value_in)
    tmp_in = 1;
  
  
  if(contains_attr(key))
  {
    if( over_write)
    {
      Attribute  tmpa =  Attribute(obj_->openAttribute(key));
      H5T_class_t type_class = tmpa.getTypeClass();
      H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  
      if(type_class == H5T_INTEGER && space_type == H5S_SCALAR )
	tmpa.write(PredType::NATIVE_INT,&tmp_in);
      else
      	throw invalid_argument("output does not match attribute dtype " + key );
    }
        else
    {
      // check to see if they match
      int tmp = 0;
      get_value(key,tmp);
      if(tmp != value_in)
      {
	cerr<<"setting attribute: "<<key<<endl;
	cerr<<"the existing value: "<<tmp<<endl;
	cerr<<"the input value: "<<value_in<<endl;
	
	throw invalid_argument("attribute name already exists and values don't match: float " + key );
      }
    }
    
  }
  else
  {
    DataSpace dspace =  DataSpace(0,NULL);
    Attribute  tmpa =Attribute(obj_->createAttribute(key,PredType::NATIVE_INT,dspace));
    tmpa.write(PredType::NATIVE_INT,&tmp_in);
    keys_.push_back(key);
  }
}

list<string> Attr_list_hdf::contents()const
{
  return keys_;
}


utilities::V_TYPE Attr_list_hdf::get_type(const std::string & key)const
{
  
  Attribute  tmpa =  Attribute(obj_->openAttribute(key));
  H5T_class_t attr_class = tmpa.getTypeClass();
  H5S_class_t space_type = tmpa.getSpace().getSimpleExtentType();
  H5T_sign_t sign;
  
  if( space_type != H5S_SCALAR)
    return V_ERROR;
  switch(attr_class)
  {
  case H5T_INTEGER:  
    sign  = tmpa.getIntType().getSign();
    if(sign  == H5T_SGN_2)
      return V_INT;
    else if(sign == H5T_SGN_NONE)
      return V_UINT;
    else
      return V_ERROR;
  case H5T_FLOAT:  
    return V_FLOAT;
  case H5T_STRING:  
    return V_STRING;
  case H5T_TIME:  
  case H5T_BITFIELD:  
  case H5T_OPAQUE:  
  case H5T_COMPOUND:  
  case H5T_REFERENCE:  
  case H5T_ENUM:	    
  case H5T_VLEN:	    
  case H5T_ARRAY:	    
  case H5T_NO_CLASS:
  case H5T_NCLASSES:
    return V_ERROR;
  }
  return V_ERROR;
}
