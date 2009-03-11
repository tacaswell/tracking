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
#include <iostream>
#include <map>

#include "wrapper_i.h"

#ifndef WRAPPER_FILE
#define WRAPPER_FILE
namespace tracking{

using std::map;
/**
   Wrapper class for dealing with data from a text file.  This
   is mainly for testing purposes.  The assumption is that the 
   data is stored in space deliniated file with one particle per
   row with the orderin [indx, x, y, I, r2,E]
*/
class wrapper_i_file:public wrapper_i_base{
private:
  ///Pointer to the first data point of the 
  double * first;
protected:
  ///number of rows(particles) that the 
  int rows;

  int cols;
  map<wrapper::p_vals, int> contents;
  void fill_data(string file_name, int row, int col);
public:
  int num_entries();

  //  void print(int ind);
  void print();
  double get_value(int ind, wrapper::p_vals type);
  
  ~wrapper_i_file();
  wrapper_i_file(params_file* param);
  

};
}

#endif
