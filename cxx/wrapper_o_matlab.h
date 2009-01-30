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
#include "wrapper_o.h"
#include "mex.h"


#ifndef WRAPPER_O_MATLAB
#define WRAPPER_O_MATLAB


namespace tracking{
class params_matlab;
/**
   Wrapper class for dealing with output to matlab
*/
class wrapper_o_matlab:public wrapper_o_base{
public:  
  //old
  int  num_entries()					  {return 0;};
  void set_value(int, tracking::wrapper::p_vals, double)  {};
  void print(int)					  {};
  int  add_particle()                                     {return 0;}; 


  //new
  void set_new_value(wrapper::p_vals type, double val);
  void end_new_particle();
  void finalize_wrapper();
  void initialize_wrapper();
  void reset_wrapper(params * param);
  void start_new_particle();
  
  wrapper_o_matlab(params_matlab* parms);
  ~wrapper_o_matlab();

  void print(){};


 protected:

  
 private:
  ///Pointer to matlab array that holds the data
  mxArray ** data_array;
  ///The number of rows in the array.  This isn't strictly needed,
  ///hoever it should make returning values faster by amoritizing the
  ///dereference cost, maybe
  int rows;
  ///The number of columns (and hence number of values in the array.
  ///This isn't strictly needed, hoever it should make returning
  ///values faster by amoritizing the dereference cost, maybe
  int cols;
  ///anchor to data with in array
  double * first;
  ///index of current open particle
  int part_index;
};

}
#endif
