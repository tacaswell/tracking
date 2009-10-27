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



#ifndef WRAPPER_I_PLU
#define WRAPPER_I_PLU

#include <complex>
#include <vector>
#include "wrapper_i.h"
#include "enum_utils.h"
#include "data_map.h"
#include "ipp.h"
#include "pair.h"

namespace utilities{

class params_matlab;
/**
   Wrapper class for dealing with data from Peter Lu's code
*/

class Wrapper_i_plu:public Wrapper_in{

public:

  /**
     adds a pointer to data_
   */
  void add_frame_data(Ipp32f[][9], int frame ,int num);

  int get_num_entries(int j) const;
  
  
  

  //  void print(int ind);
  void print()const;

  
  float get_value(int, utilities::D_TYPE, int) const
  {
    throw "wrapper_i_plu: depreciated ";
    return -1;
  }
  
  int get_value(int& out,int ind,D_TYPE type, int frame) const;
  float get_value(float &out,int ind, utilities::D_TYPE type,int frame )const;
  std::complex<float> get_value(std::complex<float> &out,int ind, utilities::D_TYPE type,int frame )const
  {throw "wrapper_i_plu:wrapper has no complex support";}

  
  Wrapper_i_plu(int,utilities::Pair);  
  virtual ~Wrapper_i_plu();
  
  

  
  bool contains_type(utilities::D_TYPE) const;
  
  int get_num_frames()const
  {
    return frame_count_.size();
  }
  
  std::set<utilities::D_TYPE> get_data_types() const
  {
    return std::set<D_TYPE>(data_types_);
  }
  
  Tuple get_dims() const;
  
private:
  /**
     vectors of pointers to data
     The wrapper owns these pointers
   */
  std::vector<Ipp32f(*) [9]> data_;
  std::vector<int> frame_count_;
  
  static const int cols = 9;
  
  
protected:
  /**
     A map between the data types and a posistion in the data structure.
     This orginally had been burried down in the derived classes, but 
     I couldn't think of a data structure for storing particle data where
     this sort of thing wasn't uesful.  Thus, it has been dragged up to
     the top level.
  */
  std::set<utilities::D_TYPE> data_types_;
  /**
     object for fast lookup
   */
  Data_map data_map_;

  /**
     count of particles added
   */
  int count_;
  /**
     dimensions of the images being shoved in
   */
  utilities::Pair dims_;
  
};

}

#endif
