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

#include "wrapper_i.h"
#include "enum_utils.h"
#include "data_map.h"

namespace tracking
{
class particle_track;
}

namespace utilities{

class params_matlab;
/**
   Wrapper class for dealing with data from Peter Lu's code
*/

class Wrapper_i_matlab:public Wrapper_in{
private:
  /**
     vectors of pointers to data
     The wrapper owns these pointers
   */
  vector<IPP32f *> data_;
  
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
public:
  int get_num_entries() const;

  //  void print(int ind);
  void print()const;
  float get_value(int ind, utilities::D_TYPE type,int frame = 0)const;
  void get_value(float &out,int ind, utilities::D_TYPE type,int frame = 0)const;
  void get_value(std::complex<float> &out,int ind, utilities::D_TYPE type,int frame = 0)const
  {throw "wrapper_i_plu:wrapper has no complex support";}
  
  
  virtual ~Wrapper_i_plu();
  Wrapper_i_plu(int);
  //void fill_master_box(tracking::master_box_t<tracking::particle_track>& test) const{};
  std::set<utilities::D_TYPE> get_data_types() const;
  void get_data_types(std::set<utilities::D_TYPE>&) const;
  
  /**
     adds a pointer to data_
   */
  void add_frame_data(Ipp32f *, int);
  
};

}

#endif
