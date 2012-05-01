//Copyright 2012 Thomas A Caswell
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
#include <exception>
#include <vector>
#include <string>
#include "generic_wrapper_hdf.h"




using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::cerr;



using utilities::Generic_wrapper_hdf;

int main(int argc, const char * argv[])
{
  unsigned int N = 25;
  int  fake_data[N];
  for (unsigned int j =0;j<25;++j)
    fake_data[j] = j;
  

  Generic_wrapper_hdf wrap = Generic_wrapper_hdf("test",Generic_wrapper_hdf::F_DISK_TRUNC);
  wrap.open_wrapper();
  wrap.open_group("/test");
  wrap.add_dset(1,&N,utilities::V_INT,fake_data,"test1");
  wrap.add_dset(1,&N,utilities::V_INT,fake_data,"/test2");
  wrap.close_group();
  
  wrap.open_group("/test");
  wrap.add_dset(1,&N,utilities::V_INT,fake_data,"test15");
  wrap.close_group();

  
  wrap.open_group("/test/testy");
  wrap.add_dset(1,&N,utilities::V_INT,fake_data,"test15");
  wrap.close_group();
  

  for (unsigned int j =0;j<25;++j)
    fake_data[j] = j*2;
  wrap.add_dset(1,&N,utilities::V_INT,fake_data,"/test3");
  wrap.add_dset(1,&N,utilities::V_INT,fake_data,"test4");
  
  vector<int> data;
  vector<unsigned int> dims;
  wrap.get_dset(data,dims,"test4");
  for(unsigned int j =0; j< 25;++j)
    cout<<j<<'\t'<<data[j]<<endl;
  
  for(vector<unsigned int>::iterator it = dims.begin(); it != dims.end(); ++it)
    cout<<*it<<endl;
  
  
  wrap.close_wrapper();
  
}
