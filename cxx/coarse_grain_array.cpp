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

#include "coarse_grain_array.h"
#include "generic_wrapper_base.h"
using utilities::Coarse_grain_array;
using std::vector;

Coarse_grain_array::Coarse_grain_array(double r_min,double r_max, 
				       int n_r_bins, int n_d_bins):
  data_array_(n_r_bins * n_d_bins),count_array_(n_r_bins * n_d_bins),
  r_bins_(n_r_bins),d_bins_(n_d_bins),r_min_(r_min),r_max_(r_max){

}

void  Coarse_grain_array::add_to_element(double r,int t, double val){
  int tmp = hash_function(r,t);
  data_array_[tmp] += val;
  ++count_array_[tmp];
}


void Coarse_grain_array::output_to_wrapper(Generic_wrapper_base * data_out_wrapper,
			 Generic_wrapper_base * count_out_wrapper) const{
  data_out_wrapper->initialize_wrapper();
  for(int j =0; j<r_bins_;++j)
  {
    data_out_wrapper->start_new_row();
    for(int k = 0;k<d_bins_;++k)
    {
      data_out_wrapper->append_to_row(data_array_[j + r_bins_*k]);
    }
    data_out_wrapper->finish_row();
  }
  data_out_wrapper->finalize_wrapper();
  
  count_out_wrapper->initialize_wrapper();
  for(int j =0; j<r_bins_;++j)
  {
    count_out_wrapper->start_new_row();
    for(int k = 0;k<d_bins_;++k)
    {
      count_out_wrapper->append_to_row(count_array_[j + r_bins_*k]);
    }
    count_out_wrapper->finish_row();
  }
  count_out_wrapper->finalize_wrapper();
}


void Coarse_grain_array::average_data(){
  vector<double>::iterator data_it = data_array_.begin();
  vector<int>::iterator count_it = count_array_.begin();
  while(data_it!= data_array_.end() && count_it!= count_array_.end())
  {
    (*data_it) /=(*count_it);
    ++data_it;
    ++count_it;
  }
  
}

int Coarse_grain_array::hash_function(double r, int t)const{
  if(r<r_min_ || r>r_max_ || t> d_bins_ || t<0)
    {
      throw "out of range";
    }
  
  return  static_cast<int>(r_bins_ * ((r-r_min_)/(r_max_- r_min_))) + t*r_bins_ ;
  
}
