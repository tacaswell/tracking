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
#include "wrapper_i.h"

#ifndef WRAPPER_NING
#define WRAPPER_NING
namespace tracking{
class params_file;
/**
   Wrapper class for dealing with data from a text file.  This
   is mainly for testing purposes.  The assumption is that the 
   data is stored in space deliniated file with one particle per
   row 
*/

class wrapper_i_ning:public wrapper_i_base{
public:
  int num_entries();

  //  void print(int ind);
  void print();
  double get_value(int ind, wrapper::p_vals type);
  
  virtual ~wrapper_i_ning();
  wrapper_i_ning(params_file* param);
  wrapper_i_ning(std::map<wrapper::p_vals,int>map_in);
  
protected:
  ///Pointer to the first data point of the 
  double * first;
  
  ///number of rows(particles) that the 
  int rows;

  int cols;
  //std::map<wrapper::p_vals, int> contents;
  virtual void fill_data(std::string file_name, int row, int col);
  void init();

};
}

#endif
