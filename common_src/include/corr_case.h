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

#include <vector>
#include "part_def.h"

namespace utilities
{
class Generic_wrapper;
}


namespace tracking
{
class Corr;
class Corr_gofr;





/**
   A class to hold a stack of accum objects for handing into a hash_case
 */

class Corr_case
{
public:
  /**
     Constructor for filling with accum_sofq.  The pointer does
     nothing, but is passed into set the type.
  */
  Corr_case(const tracking::Corr_gofr*,
	    int comp_count, 
	    float max_range, 
	    const int n_bins,
	    int comp_num,
	    int dset_num);
  
  /**
     Averages the values in a vector of Accum_sofq and writes 
   */
  void out_to_wrapper(utilities::Generic_wrapper & wrap,const std::string & base_name) const;

  /**
     Returns the nth element of the case.  Will throw out of range errors
   */
  tracking::Corr* at(int j)
  {
    return corr_vec_.at(j);
  }
  /**
     Returns the nth element of the case.  Will not throw out of range errors
   */
  tracking::Corr* operator[](int j)
  {
    return corr_vec_[j];
  }


  /**
     Returns size
   */
  unsigned int size()const
  {
    return corr_vec_.size();
  }
  
  ~Corr_case();
  
private:
  int frame_count_;
  std::vector< tracking::Corr*> corr_vec_;
  int comp_num_;
  int dset_;
  


};
}

  
