//Copyright 2012 Thomas A Caswell
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
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
#include <iostream>
#include <map>

#include "wrapper_i_generic.h"

#ifndef WRAPPER_JB_BINARY
#define WRAPPER_JB_BINARY
namespace utilities{

using std::map;
using std::string;

/**
   Wrapper class for dealing with data from Justin's simulations.  The
   format is a binary file with a stream X1Y1Z1X2Y2Z2... for order
   million particles. 

   Only one 3D packing per file, but this can be split up into any
   number of planes for faking 3D data.
*/
class Wrapper_i_jb_binary:public Wrapper_in{
private:
  ///path to file to read
  std::string fname_;
  /**
     pointer to Wrapper_i_generic object that deals with actual data wrangling.
   */
  utilities::Wrapper_i_generic * wrapper_int_;
  
  
public:

  // inherited from ABC
  int                 get_value(int& out,
				int ind,D_TYPE type, int frame) const ;
  float               get_value(float& out,
				int ind,D_TYPE type, int frame) const ;
  std::complex<float> get_value(std::complex<float>& out,
				int ind,D_TYPE type, int frame) const ;


  std::set<D_TYPE>    get_data_types() const ;



  int get_num_entries(unsigned int frame) const ;
  int get_num_entries() const ;

  int get_num_frames() const ;

  bool contains_type(D_TYPE type) const ;

  Tuplef get_dims() const;
  
  ~Wrapper_i_jb_binary();
  Wrapper_i_jb_binary();
  
  // local
  /**
     Sets the file name
   */
  bool set_file_name(const std::string & fname);
  /**
     reads in the binary file.  There should be lots of parameters on
     this, but for now what I need in hard coded in.
   */
  bool proc_file(unsigned int N);
  


};
}

#endif
