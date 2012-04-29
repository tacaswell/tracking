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
#include "md_store.h"

#include <iostream>
#include <string>



using utilities::Md_store;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

template<class T>
bool test_fun (string str,T val,Md_store & md)
{
  T tmp1,tmp2;
  bool suc;
  
  md.add_element(str.c_str(),val);
  tmp2 = md.get_value(str,tmp1);
  suc = (val == tmp2) && (val == tmp1);
  
  cout<<val<<"\t"<<tmp1<<"\t"<<tmp2<<"\t";
  
  if(suc)
    cout<<"PASS"<<endl;
  else
    cout<<"FAIL"<<endl;


  return suc;
  
  
}

int main()
{

 
  Md_store md;
  int A = 1;
  test_fun("A",A,md);
  int Ap = -1;
  test_fun("Ap",Ap,md);
  float B = 3.14;
  test_fun("B",B,md);
  unsigned int C=  125;
  test_fun("C",C,md);

  bool D=  true;
  test_fun("D",D,md);


  string E=  "test";
  test_fun("E",E,md);

  Md_store md2;
  md2.add_elements(&md);
  
  
  return 0;
  
}

