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

#include<string>
#include<iostream>
#include "image_series.h"
#include "md_store.h"
#include "image_stack.h"

using std::string;
using utilities::Image_stack;
using utilities::Image_series;

using utilities::Md_store;
using std::endl;
using std::cout;

static string fname = "/home/tcaswell/colloids/2010/data/polyNIPAM_batch_12/20100524/4/exp2/28-8_warming_slow.tif";
static string base_name = "/home/tcaswell/colloids/2010/data/polyNIPAM_batch_12/2010-10-03/ssd_test/t3/test3_";
int main()
{
 
  cout<<"trying series code"<<endl;
  
  Image_series im_sr(base_name,3);
  for( int j = 0; j<2;++j)
  {
    im_sr.select_plane(1+j*100);
    Md_store * tmp_store = im_sr.get_plane_md();
    tmp_store->print();
    delete tmp_store;
  }
  
  cout<<"trying stack code"<<endl;
  Image_stack im_sk(fname);
  cout<<"object built"<<endl;
  for( int j = 1; j<2;++j)
  {

    im_sk.select_plane(j);
    cout<<"selected frame "<<j<<endl;
    Md_store * tmp_store = im_sk.get_plane_md();
    tmp_store->print();
    delete tmp_store;
  }
  
  


  

    

  return 0;
  
}
