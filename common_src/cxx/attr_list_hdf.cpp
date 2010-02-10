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
using utilities::Attr_list_hdf;

using H5::H5Object;

using std::string;
using std::list;
using std::cout;

using std::endl;


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


void Attr_list_hdf::print()const 
{
  for(list<string>::const_iterator it = keys_.begin();  it != keys_.end();++it)
  {
    cout<<*it<<endl;
    
  }
}


