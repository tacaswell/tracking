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

#include "tuple.h"

#include "ndarray.h"
#include <iostream>
#include <complex>
using std::endl;
using std::cout;
using std::complex;


using utilities::Tuple;
using utilities::ND_Array;


typedef Tuple<int,2> pair;

int main()
{

  bool tuple_test = true;
  bool array_test = true;
  if(tuple_test)
  {
    
    const Tuple<int,2> test(1,2);
    test.print();

    const   Tuple<int,2> test2(1,2.5);
    test2.print();
  
    const   Tuple<float,2> test3(1.5,2.5);
    test3.print();

    const   Tuple<int,2> test4(test3);
    test4.print();
  

    const   Tuple<int,3> testt(1,2,3);
    testt.print();

    const   Tuple<int,3> testt2(1,2.5,3);
    testt2.print();
  
    const   Tuple<float,3> testt3(1.5,2.5,3.5);
    testt3.print();

    cout<<testt3[0]<<'\t'<<testt3[1]<<'\t'<<testt3[2]<<endl;
    cout<<testt3[0]<<'\t'<<testt3[1]<<'\t'<<testt3[2]<<endl;
  }
  if(array_test)
  {
    Tuple<int,2> test(10,5);
    ND_Array<int,Tuple<int,2> >atest(test);
    atest.fill_test();
    
    atest.print();
    atest(pair(0,0)) = 123456;
    atest.print();
    
    int t = atest(pair(0,1));
    cout<<"t "<<t<<endl;
    t = 654321;
    cout<<"t "<<t<<endl;
    atest.print();

    
    ND_Array<Tuple<float,2>,Tuple<int,2> >ctest(test);
    for(int j = 0;j<10;++j)
      for(int k = 0;k<5;++k)
	ctest(pair(j,k)) = pair(j,k);
    
    ctest.print();
    
    

    // Tuple<int,3> testt(10,3,2);
    // ND_Array<int,Tuple<int,3> >atestt(testt);
    // try
    // {
    //   atestt.fill_test();
      
    //   atestt.print();
    // }
    // catch(char const * e)
    // {
    //   cout<<e<<endl;
    // }
    
    
    
  }
  

  return 0;
}
