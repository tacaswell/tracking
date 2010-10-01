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


#include <vector>
#include "data_map.h"

namespace utilities
{
class Data_map;

/**
   A struct to hold the data_map and raw data for input and output wrappers.

   Templated on the type of 
 */
template<class T>
struct Data_store
{
public:
  std::vector<T *> data;
  Data_map map;
  

};
}
