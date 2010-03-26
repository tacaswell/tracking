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

#include "pair.h"
#include "triple.h"
#include <iostream>
using std::endl;
using std::cout;


using utilities::Pair;
using utilities::Triple;

int main()
{
  const Pair<int> test(1,2);
  test.print();

const   Pair<int> test2(1,2.5);
  test2.print();
  
const   Pair<float> test3(1.5,2.5);
  test3.print();

const   Pair<int> test4(test3);
  test4.print();
  

const   Triple<int> testt(1,2,3);
  testt.print();

const   Triple<int> testt2(1,2.5,3);
  testt2.print();
  
const   Triple<float> testt3(1.5,2.5,3.5);
  testt3.print();

  cout<<testt3[0]<<'\t'<<testt3[1]<<'\t'<<testt3[2]<<endl;
  cout<<testt3[0]<<'\t'<<testt3[1]<<'\t'<<testt3[2]<<endl;
  

  return 0;
}
