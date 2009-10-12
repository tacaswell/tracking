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


#ifndef WRAPPER_O_MATLAB
#define WRAPPER_O_MATLAB
#include <complex>

#include "enum_utils.h"
#include "wrapper_o.h"
#include "data_map.h"

#include "mex.h"



namespace utilities{
class params_matlab;
/**
   Wrapper class for dealing with output to matlab
*/
class Wrapper_o_matlab:public Wrapper_out{
public:  

  void set_value(utilities::D_TYPE type, float val);
  void set_value(utilities::D_TYPE type, std::complex<float> val)
  {
    throw "not implemented";
  }
  void close_particle();

  void open_frame(int,int)
  {
    throw "not implemented";
  }
  void close_frame(){
    throw "not implemented";
  }
  void finalize_wrapper();
  void initialize_wrapper();
  void reset_wrapper(params * param);
  void open_particle(int);
  
  Wrapper_o_matlab(params_matlab* parms);
  ~Wrapper_o_matlab();

  void print()const;
  void get_content_tpyes(std::set<D_TYPE>&) const {};
  int  num_entries()const{return 0;};
  

 protected:
    ///Count of the number of particles that have been added
  int part_count;
  
  ///to be removed, move this accounting to derived classes
  ///posistion in squence
  int seq_count;

  ///if a particle is 'opened'
  bool part_open;
  
  ///if the wrapper is 'open' to get additional particles
  bool wrapper_open;
  

  
 private:
  ///Pointer to matlab array that holds the data
  mxArray ** data_array_;
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
  /**
     object that does type to index conversion
   */
  Data_map data_map_;
  
};

}
#endif
