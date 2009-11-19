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



#ifndef WRAPPER_I_MATLAB
#define WRAPPER_I_MATLAB

#include <complex>

#include "wrapper_i.h"
#include "enum_utils.h"
#include "mex.h"
#include "data_map.h"

namespace tracking
{
class particle_track;
}

namespace utilities{

class params_matlab;
/**
   Wrapper class for dealing with data from matlab
*/

class Wrapper_i_matlab:public Wrapper_in{
private:
  ///Pointer to matlab array that holds the data
  //  const mxArray * data_array;
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
protected:
  /**
     A map between the data types and a posistion in the data structure.
     This orginally had been burried down in the derived classes, but 
     I couldn't think of a data structure for storing particle data where
     this sort of thing wasn't uesful.  Thus, it has been dragged up to
     the top level.
  */
  std::map<utilities::D_TYPE, int> data_types_;
  /**
     object for fast lookup
   */
  Data_map data_map_;
  /**
     internal initialization
   */
  void init();

  /**
     internal get_value
   */
  float get_value(int ind,  utilities::D_TYPE type,int junk)const;
  
public:
  int get_num_entries() const;

  //  void print(int ind);
  void print()const;
  
  int                 get_value(int& out,
				int ind,D_TYPE type, int frame) const ;
  float               get_value(float& out,
				int ind,D_TYPE type, int frame) const ;
  std::complex<float> get_value(std::complex<float>& out,
				int ind,D_TYPE type, int frame) const ;
  
  
  int get_num_entries(int frame) const ;

  int get_num_frames() const ;

  bool contains_type(D_TYPE type) const ;

  Tuple get_dims() const;



  
  virtual ~Wrapper_i_matlab();
  Wrapper_i_matlab(params_matlab* param);
  Wrapper_i_matlab();
  //void fill_master_box(tracking::Master_box<tracking::particle_track>& test) const{};
  std::set<utilities::D_TYPE> get_data_types() const;
  void get_data_types(std::set<utilities::D_TYPE>&) const;
  
};

}

#endif
