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
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting

#ifndef WRAPPER_I_PYTHON
#define WRAPPER_I_PYTHON

#include "H5Cpp.h"

#include <string>
#include <vector>
#include <map>

#include "wrapper_i.h"
#include "data_map.h"
#include "part_def.h"

namespace utilities{
/**
   Wrapper class for eating data from hdf files, take 2
*/
class Wrapper_i_generic:public Wrapper_in{
private:
  
  /**
     Set of pairs of data types and the computation number to be
     used for that data type.
   */
  std::set<std::pair<utilities::D_TYPE,int> > data_types_;
  /**
     Set of data types in the wrapper.  This is mostly redundant, but
     useful.
   */
  std::set<utilities::D_TYPE> data_types_set_;
  
  /**
     Data map for integer data
   */
  Data_map d_mapi_;
  /**
     Data map for float data
  */
  Data_map d_mapf_;
  /**
     Data map for complex<float> data
  */
  Data_map d_mapc_;
  
  /**
     Data structure for integer data. First dimension is data type,
     second is frame, third in particle index
   */
  std::vector<std::vector<int*> > data_i_;
  /**
     Data structure for float data. First dimension is data type,
     second is frame, third in particle index
   */
  std::vector<std::vector<float*> > data_f_;
  /**
     Data structure for complex<float> data. First dimension is data
     type, second is frame, third in particle index
   */
  std::vector<std::vector<std::complex<float>*> > data_c_;
  
  /**
     the number of particles in each frame
   */
  std::vector<int> frame_c_;
  /**
     The number of frames
   */
  unsigned int  frame_count_;

  /**
     The total number of particles in all frames
   */
  unsigned int total_part_count_;
  
  /**
     A vector of the z-position of each frame.  For 2D data this is
     the center of the plane, for 3D this provides if we have any
     drift.
   */
  std::vector<float> frame_zdata_;
  
  /**
     if the parameters are locked
   */
  bool locked_;
  

  /**
     Deletes all allocated data
   */
  void clean_data();
  

public:
    
  
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


  
  ~Wrapper_i_generic();

  /**
     Constructor
   */
  Wrapper_i_generic();
  
  /**
     open wrapper to accept data, nukes everything currently in the wrapper
   */
  bool open_wrapper();
  /**
     Sets the number of frames to expect.

     if this is not set will grow dynamically.
   */
  bool set_frames();

  /**
     Sets the data to expect for each frame
   */
  bool set_data_types(std::set<D_TYPE>);
  /**
     set to accept data
   */
  bool finish_setup();
  /**
     Opens a frame to add data to it
   */
  bool open_frame(unsigned int frame);
  
  /**
     adds data, internal logic will cast it right (we hope!)
   */
  template<class T>
  bool add_data(D_TYPE dtype,T * data);
  
  /**
     Closes frame.  Throws error 
   */
  bool close_frame();
  
  /**
     Closes wrapper from taking any new data
   */
};

}

#endif
