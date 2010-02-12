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

#include <iostream>
#include <string>
#include <stdexcept>
#include "attr_list_hdf.h"
#include "H5Cpp.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using H5::H5File;
using H5::Group;

using utilities::Attr_list_hdf;


int main()
{
  
  try
  {
    
    cout<<"Hello World"<<endl;
    
    string file_name_ = "attr_test.h5";
    
    H5File file = H5File( file_name_, H5F_ACC_RDWR );  
    
    Group g = file.openGroup("/");
    Attr_list_hdf al(&g);
    al.print();
    
    cout<<al.contains_attr("test_attr")<<endl;
    cout<<al.contains_attr("test_attr2")<<endl;
   
    cout<<"end contain test"<<endl;
    
    int n = 0;
    cout<<"try 1:"<<endl;
    
    cout<<al.get_value("test_attr",n)<<endl;;
    int n4 = 13;
    
    al.set_value("attr3", n4);
    cout<<al.contains_attr("attr3")<<endl;
    cout<<al.get_value("attr3",n)<<endl;;
    
    n4 = 123;
    
    al.set_value("attr3", n4,true);
    cout<<al.get_value("attr3",n)<<endl;

    float f = 0;
    
    al.get_value("test_attrf",f);
    cout<<"float value "<<f<<endl;
    al.set_value("test_attrf",(float)654.321,true);
    
    al.get_value("test_attrf",f);
    cout<<"float value "<<f<<endl;
    
    al.set_value("test_attrf2",(float)3.14159);
    
    al.get_value("test_attrf2",f);
    cout<<"float value "<<f<<endl;
    
  }
  catch(H5::Exception & e)
  {

    
    e.printError();
    
  }
  catch(std::exception & e)
  {
    cerr<<e.what()<<endl;
  }
  
  

  return -1;
}
