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
#include "attr_list.h"
#include "H5Cpp.h"

using std::cout;
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
    
    string file_name_ = "test.h5";
    
    H5File file = H5File( file_name_, H5F_ACC_RDONLY );  
    
    Group g = file.openGroup("/");
    Attr_list_hdf al(&g);
    al.print();

    Group g2 = file.openGroup("/test_group");
    Attr_list_hdf al2(&g2);
    al2.print();
    
  }
  catch(H5::Exception e)
  {
    e.printError();
    
  }
  
  

  return -1;
}
