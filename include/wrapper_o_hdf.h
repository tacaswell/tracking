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

#ifndef WRAPPER_O_HDF
#define WRAPPER_O_HDF
#include "enum_utils.h"
#include "wrapper_o.h"
#include "data_map.h"
#include <string>
#include <vector>
#include <set>

namespace H5
{
class H5File;
class Group;
class DataSet;
}

namespace tracking
{
class particle_base;
}


namespace utilities{

/**
   Wrapper class for dealing with output to matlab
*/
class Wrapper_o_hdf:public Wrapper_out{
public:  
    
  Wrapper_o_hdf(const std::string& file_name,std::set<D_TYPE> d_add);
  
  void initialize_wrapper();
  void open_frame(int,int);
  void open_particle(int);

  
  void set_value(utilities::D_TYPE type, float val);
  void set_value(utilities::D_TYPE type, int val);
  void set_value(utilities::D_TYPE type, std::complex<float> val);
  
  void set_values(const tracking::particle_base *);
  

  void close_particle();
  void close_frame();
  void finalize_wrapper();

  void reset_wrapper(params * param);
  
  void print()const;
  int  num_entries()const{return 0;};

  void get_content_tpyes(std::set<D_TYPE>&) const;
  
  ~Wrapper_o_hdf();

private:
  ///Count of the number of particles that have been added
  int part_count_;
  

  ///if a particle is 'opened'
  bool part_open_;
  
  ///if the wrapper is 'open' to get additional particles
  bool wrapper_open_;
  
  /**
     if there is a 'open' frame
   */
  bool frame_open_;
  
  ///index of current open particle
  int part_index_;
  
  /**
     maximum number of particles in current frame
   */
  int frame_max_count_;
  
  /**
     index of current open frame
   */
  int frame_index_;

  /**
     if the data is being put back into a hdf file, or is a new hdf file
   */
  bool new_hdf_;

  /**
     if existing data in to be over written
   */
  bool over_write_;
  
  /**
     Pointer to hdf file
   */
  H5::H5File * file_;
  /**
     pointer to currently open group
   */
  std::string file_name_;

  /**
     A set of the data types that are to be added to the file
   */
  std::set<D_TYPE> d_types_add_;
  /**
     A set of the data types to be checked to make sure there isn't
     a mismatch
   */
  std::set<D_TYPE> d_types_check_;

  /**
     The set of data that the file already contains
   */
  std::set<D_TYPE> d_types_already_;
  

  /**
     struct to deal with shoving complex data in to hdf
   */
  typedef struct complex_t
  {
    float re;
    float im;
  } complex_t;
  
  /**
     vector of pointers to temporary arrays for floats 
   */
  std::vector<float *> float_data_;

  /**
     look up table for float data
   */
  Data_map float_map_;
  

  /**
     vector of pointers to temporary arrays for ints
   */
  std::vector<int *> int_data_;

  /**
     look up table for float data
   */
  Data_map int_map_;

  
  /**
     vector of pointers to temporary arrays for floats 
   */
  std::vector<complex_t *> complex_data_;

  /**
     look up table for float data
   */
  Data_map complex_map_;

  
  
  /**
     formats frame names
   */
  std::string format_name(int in)const;
  

  
};

}


#endif
