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

#ifndef WRAPPER_I_HDF
#define WRAPPER_I_HDF

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
class Wrapper_i_hdf:public Wrapper_in{
private:
  /**
     name for the data file
  */
  std::string file_name_;
  
  static std::string format_name(int in);
  
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
     Deletes all allocated data
   */
  void clean_data();
  
  /**
     Private initialization function
   */
  void priv_init(int f_count =0);
  /**
     Turns a set of data types in to a set of pairs assuming they
     all have the same computation number.
   */
  void make_dtype_pairs(int comp_nuber);
  /**
     Extracts the set of data types from a set of (D_TYPE,comp_num)
     pairs.
   */
  void make_dtype_set();
  
  /**
     If we are reading in 2D data
   */
  bool two_d_data_;
  
  /**
     A vector of the z-position of each frame.  For 2D data this is
     the center of the plane, for 3D this provides if we have any
     drift.
   */
  std::vector<float> frame_zdata_;

  /**
     first frame to read in
   */
  unsigned int start_;
  
  

protected:

  void init();
public:
  typedef enum DATA_DIMS
    {
      TWO_D = 0,
      THREE_D
    } DATA_DIMS;
    
    
  
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


  
  ~Wrapper_i_hdf();
  
  /**
     Constructor

     @param fname [in] name of hdf file to read in
     @param dtypes [in] a set of D_TYPES to extract from the hdf file
     @param comp_number [in] the computation number to read
     @param start [in] the first frame to read from
     @param frames [in] the number of frames to read
   */
  Wrapper_i_hdf(std::string fname,
		const std::set<utilities::D_TYPE>& dtypes,
		int comp_number,
		unsigned int start=0,
		int frames =0);
  /**
     Constructor

     @param fname [in] name of hdf file to read in
     @param dtypes [in] a set of D_TYPES to extract from the hdf file
     @param comp_number [in] the computation number to read
     @param start [in] the first frame to read from
     @param f_count [in] the number of frames to read
     @param two_d_data [in] if this is 2D data
   */

  Wrapper_i_hdf(std::string fname,
		const std::set<utilities::D_TYPE> &dtypes,
		int comp_number,
		unsigned int start,
		int f_count,
		bool two_d_data);
  
  /**
     Constructor

     @param fname [in] name of hdf file to read in
     @param dtypes [in] a set of (D_TYPES,comp_number) pairs to extract from the hdf file
     @param start [in] the first frame to read from
     @param f_count [in] the number of frames to read
     @param two_d_data [in] if this is 2D data
   */
  
  Wrapper_i_hdf(std::string fname,
		const std::set<std::pair<utilities::D_TYPE,int > > &dtypes,
		unsigned int start,
		int f_count,
		bool two_d_data);
  
		
  /**
     returns the scaling between pixels and microns.  This should be pushed up
     to the next level, but I am lazy.
  */
  float get_xy_scale() const;
  
  /**
     Returns the starting plane.  This may be needed to put data back
     into hdf files properly.  Although, processing only part of the
     file seems like it would lead to madness and chaos.
  */
  unsigned int get_start_offset() const {return start_;}
  
  
  
};

}

#endif
