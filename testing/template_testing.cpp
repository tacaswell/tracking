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

#include "pair_t.h"
#include "triple_t.h"

using utilities::Pair_t;
using utilities::Triple_t;

int main()
{
  Pair_t<int> test(1,2);
  test.print();

  Pair_t<int> test2(1,2.5);
  test2.print();
  
  Pair_t<float> test3(1.5,2.5);
  test3.print();


  Triple_t<int> testt(1,2,3);
  testt.print();

  Triple_t<int> testt2(1,2.5,3);
  testt2.print();
  
  Triple_t<float> testt3(1.5,2.5,3.5);
  testt3.print();


  return 0;
}
