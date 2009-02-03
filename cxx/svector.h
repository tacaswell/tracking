//Copyright 2008,2009 Thomas A Caswell
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
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.

#ifndef SVECTOR
#define SVECTOR

#include <vector>
#include <iostream>
namespace utilities{
class Generic_wrapper;

/**
   A class that is basically a wrapper of the STL vector to carry around a pointer
   to a Generic_wrapper to facilitate output
*/
template<class T>
struct Svector{
  Generic_wrapper * out;
  std::vector<T> data;
  void print();
};

template<class T>
void Svector<T>::print(){
  for(unsigned int j = 0 ; j<data.size(); j++)
    std::cout<<data[j]<<"\t";
  std::cout<<std::endl;
}

}
#endif
