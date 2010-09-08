//Copyright 2010 Thomas A Caswell
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

namespace tracking
{
class Accum_sofq;
class Accumulator;
}


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
  /**
     Constructor for filling with Accum_sofq.  The pointer does
     nothing, but is passed into set the type.
  */
  Accum_case(const tracking::Accum_sofq*,
	     int n, 
	     const utilities::Tuple<float,2>& q_range, 
	     utilities::Tuplef q, 
	     const int n_bins);
  
  /**
     Averages the values in a vector of Accum_sofq and writes 
   */
  void out_to_wrapper(utilities::Generic_wrapper & wrap,tracking::Accum_sofq*) const;

  /**
     Averages the values and returns the results in the vector.  The pointer is there
     only to determine type.
   */
  void average(std::vector<float> & ,tracking::Accum_sofq*) const;
  
  /**
     Display the average.  Pointer is only to identify type.
   */
  void display(tracking::Accum_sofq*) const;

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
  ~Accum_case();
  
private:
  /**
     Number of frames
   */
  int frame_count_;
  /**
     structure to hold the Accumulator ojbects
   */
  std::vector< tracking::Accumulator*> accum_vec_;
  /**
     Computation number.

     This will probably be replaced with an Md_store object
   */
  int comp_num_;
  /**
     dest number

     This will probably be replaced with an Md_store object
   */
  int dset_;
  


};
}

  
