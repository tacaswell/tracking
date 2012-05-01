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


#include "md_store.h"
#include "enum_utils.h"
#include "attr_list_hdf.h"

#include <string>

#include <list>



using utilities::Md_store;
using utilities::Attr_list_hdf;


using std::string;

using std::list;

const string string_str = "string";

Md_store::Md_store(const utilities::Attr_list_hdf & atr_lst )
{
  
  list<string> cont = atr_lst.contents();
  for(list<string>::iterator it = cont.begin();
      it != cont.end();++it)
  {
    int tmpi;
    int tmpui;
    float tmpf;
    string tmps;
    tmps.clear();
    
    string key = *it;
    
    V_TYPE vt = atr_lst.get_type(key);
    switch(vt)
    {
    case V_INT:
      atr_lst.get_value(key,tmpi);
      add_element((key).c_str(),tmpi);
      break;

    case V_UINT:
      atr_lst.get_value(key,tmpui);
      add_element((key).c_str(),tmpui);
      break;
    case V_FLOAT:
      atr_lst.get_value(key,tmpf);
      add_element((key).c_str(),tmpf);
      break;
      
    case V_STRING:
      atr_lst.get_value(key,tmps);
      add_element(key,string_str,tmps);
      break;
    default:
      break;
    }
  }
}

