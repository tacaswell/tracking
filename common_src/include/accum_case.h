//Copyright 2010,2012 Thomas A Caswell
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

#include <vector>
#include "part_def.h"
#include "accumulator.h"



namespace utilities
{
class Generic_wrapper;




/**
   A class to hold a stack of accum objects for handing into a hash_case.  Mostly a wrapper
   of a std::vector.  Takes care of construction and destruction of the accum* objects.
 */

class Accum_case
{
public:

  template <class T>
  Accum_case(const T & base_obj,unsigned int frame_count):
    frame_count_(frame_count),accum_vec_(frame_count)
  {
    for(int j = 0; j<frame_count;j++)
      accum_vec_[j] = new T(base_obj);
  }
  
    
  /**
     Returns the nth element of the case.  Will throw out of range errors
   */
  tracking::Accumulator* at(int j)
  {
    return accum_vec_.at(j);
  }
  /**
     Returns the nth element of the case.  Will not throw out of range errors
   */
  tracking::Accumulator* operator[](int j)
  {
    return accum_vec_[j];
  }


  /**
     Returns size
   */
  unsigned int size()const
  {
    return accum_vec_.size();
  }
  /**
     Destructor
   */
  ~Accum_case(){
    for(unsigned int j = 0; j<accum_vec_.size();j++)
    {
      delete accum_vec_[j];
      accum_vec_[j] = NULL;
    }
  }


  
private:
  /**
     Number of frames
   */
  int frame_count_;
  /**
     structure to hold the Accumulator ojbects
   */
  std::vector< tracking::Accumulator*> accum_vec_;


};
}

  
