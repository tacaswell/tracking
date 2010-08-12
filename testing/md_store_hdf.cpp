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
#include "md_store.h"

using utilities::Attr_list_hdf;
using utilities::VT2str_s;
using utilities::V_TYPE;
using utilities::V_INT;
using utilities::V_FLOAT;
using utilities::V_STRING;
using utilities::Md_store;


using H5::H5Object;
using H5::Attribute;
using H5::PredType;
using H5::DataSpace;
using H5::StrType;
using H5::H5File;
using H5::Group;

using std::list;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::invalid_argument;


int main()
{

  H5File file =  H5File( "md_store_test.h5", H5F_ACC_RDONLY );  
  Group  group = Group(file.openGroup("/frame000000"));
  Attr_list_hdf atr_lst(&group);
  
  // list<string> cont = atr_lst.contents();
  // for(list<string>::iterator it = cont.begin();
  //     it != cont.end();++it)
  // {
  //   int tmpi;
  //   float tmpf;
  //   string tmps;
  //   tmps.clear();
    
  //   V_TYPE vt = atr_lst.get_type(*it);
  //   //cout<<*it<<'\t'<<VT2str_s(vt)<<'\t' ;
  //   switch(vt)
  //   {
  //   // case V_INT:
  //   //   atr_lst.get_value(*it,tmpi);
  //   //   cout<<tmpi;
  //   //   break;
  //   // case V_FLOAT:
  //   //   atr_lst.get_value(*it,tmpf);
  //   //   cout<<tmpf;
  //   //   break;
      
  //   case V_STRING:
  //     cout<<*it<<'\t';
  //     cout<<tmps<<endl;
  //     atr_lst.get_value(*it,tmps);
  //     cout<<tmps<<'\t';
  //     cout<<atr_lst.get_value(*it,tmps);
  //     cout<<'\t'<<tmps;
      
  //     cout<<endl;
      
  //     break;
  //   default:
  //     break;
  //   }
    
    

  //   //cout<< endl;
  // }


  string tmps;
  string key = "look-up-table-name";
  
  // cout<<atr_lst.get_value(key,tmps)<<endl;
  // cout<<atr_lst.get_value(key,tmps)<<endl;
  // key = "spatial-calibration-units";
  // cout<<atr_lst.get_value(key,tmps)<<endl;
  // cout<<atr_lst.get_value(key,tmps)<<endl;
  
  
  
  Md_store md(atr_lst);
  md.print();
  
  

}

