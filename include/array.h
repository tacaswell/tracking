//Copyright 2009 Thomas A Caswell
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
#ifndef ARRAY_T
#define ARRAY_T

#include <vector>

namespace utilities{
class Generic_wrapper_base;
class Touple;
/**
   Class to accumulate touples into.
 */
class Array{
public:
  /**
     Sets the whole array to the specified output wrapper
   */
  void set_array(Generic_wrapper_base * out_wrapper);
  /**
     Adds a touple to the end of the array
   */
  void push(Touple touple_in);
  /**
     Sensible printing
   */
  void print();
  /**
     Constructor
   */
  Array(
	int rows);
protected:
  /**
     Number of rows
   */
  int rows_;
  /**
     number of colmuns
   */
  int cols_;
  /**
     Vector to hold data
   */
  std::vector<double> data_;
  /**
     The current row, in [0,rows_-1]
   */
  int cur_index_;
};
}

#endif
