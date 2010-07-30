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

#ifndef COUNTED_VECTOR
#define COUNTED_VECTOR

#include <vector>
#include <string>
namespace utilities{
class Generic_wrapper;
class Md_store;

/**
   A vector class that keeps count of elements being added to the
   bins for use in averaging scalers, ie msd
 */
class Counted_vector{
public:
  /**
     Constructor
   */
  Counted_vector(int n_elements);
  /**
     Adds val to the element t
   */
  void  add_to_element(int t, float val);

  /**
     Adds val to the value and count to the count_array_
   */
  void  batch_add_to_element(int t, float val,int count);

  /**
     Output data and count vectors to out_wrapper.
  */
  void output_to_wrapper(Generic_wrapper * out_wrapper,
			 std::string & g_name,
			 std::string & data_name,
			 std::string & count_name,
			 const Md_store* g_md_store
			 ) const;

  /**
     Sensible output
   */
  void print() const;
  
  /**
     Averages by the count array
   */
  void average_data();

  /**
     Un-averages by the count array
   */
  void unaverage_data();

  /**
     Returns the number of elements
   */
  int get_length()const{return data_array_.size();}

  /**
     Returns the value in element j
  */
  float get_val(int j)const
  {
    return data_array_.at(j);
  }
  /**
     Returns the count in element j
  */
  int get_count(int j)const
  {
    return count_array_.at(j);
  }
  /**
     Return if the array is averaged
   */
  bool averaged() const
  {
    return averaged_;
  }
  
protected:

  /**
     Array that contains the data
   */
  std::vector<float> data_array_;
  /**
     Array of identical size that counts how many times each bin is
     added to
   */
  std::vector<int> count_array_;
  
  bool averaged_;
  
  
};


}



#endif
