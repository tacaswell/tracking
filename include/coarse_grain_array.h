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

#ifndef COARSE_GRAIN_ARRAY
#define COARSE_GRAIN_ARRAY

#include <vector>

namespace utilities{
class Generic_wrapper_base;
/**
   An array class with a contitous axis that is coarse
   grained and a discrete axis.  This is for things like
   looking at aging in g(r,t) and D_ab(r,t).  Also keeps a
   count of how many times something is added to each element
   for averaging and statistics.
 */
class Coarse_grain_array{
public:
  /**
     Construtor
   */
  Coarse_grain_array(double r_min,double r_max, int n_r_bins, int n_d_bins);
  /**
     Adds val to the element  in the bin that r falls into in
     the discrete row t
     @param r continuous axis
     @param t discrete axis
     @param val ammount to add
   */
  void  add_to_element(double r,int t, double val);

  /**
     Output to what ever is pointed to by the wrapper
     outputs only the data array.
   */
  void output_to_wrapper(Generic_wrapper_base * out_wrapper) const;

  /**
     Output to what ever is pointed to by the wrappers
   */
  void output_to_wrapper(Generic_wrapper_base * data_out_wrapper,
			 Generic_wrapper_base * count_out_wrapper) const;

  /**
     Sensible output
   */
  void print() const;
  
  /**
     Averages by the count array
   */
  void average_data();
  /**
     returns the maximum radius supported
   */
  double get_r_max()const{return r_max_;}
protected:
  /**
     Hash function
   */
  int hash_function(double r,int t) const;

  /**
     Array that contains the data
   */
  std::vector<double> data_array_;
  /**
     Array of identical size that counts how many times each bin is
     added to
   */
  std::vector<int> count_array_;
  /**
     number of bins along the continous axis
   */
  int r_bins_;
  /**
     number of discrete bins
   */
  int d_bins_;
  /**
     Minimum value of the continous axis
   */
  double r_min_;
  /**
     Maximum value of the continous axis
   */
  double r_max_;
  
};


}



#endif
