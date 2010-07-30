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


#include "counted_vector.h"
#include "generic_wrapper.h"
#include <iostream>

using utilities::Counted_vector;
using std::vector;
using std::string;

Counted_vector::Counted_vector( int n_elements):
  data_array_(n_elements,0),count_array_(n_elements,0),averaged_(false)
{
}

void  Counted_vector::add_to_element(int t, float val){
  if(averaged_)
  {
    throw "can not add to averaged vector";
  }

  // let it do the error checking the first time
  data_array_.at(t) += val;
  // if it doesn't throw an error on the first one, then this is safe
  ++count_array_[t];
}



void  Counted_vector::batch_add_to_element(int t, float val,int count){
  if(averaged_)
  {
    throw "can not add to averaged vector";
  }

  // let it do the error checking the first time
  data_array_.at(t) += val;
  // if it doesn't throw an error on the first one, then this is safe
  count_array_[t]+=count;
}


void Counted_vector::output_to_wrapper(Generic_wrapper * out_wrapper,
				       string & g_name,
				       string & data_name,
				       string & count_name,
				       const Md_store* g_md_store
				       ) const
{
  // house keeping
  bool opened_wrapper = false;
  if(!out_wrapper->is_open())
  {
    out_wrapper->open_wrapper();
    opened_wrapper = true;
    
  }
  int n_bins = data_array_.size();
  
  // open group
  out_wrapper->open_group(g_name);
  // shove in real data
  const float * tmpf = &(data_array_.front());
  out_wrapper->add_dset(1,&n_bins,utilities::V_FLOAT,tmpf,data_name);
  const int * tmpi = &(count_array_.front());
  out_wrapper->add_dset(1,&n_bins,utilities::V_INT,tmpi,count_name);
  // shove in meta data
  if(g_md_store)
    out_wrapper->add_meta_data(g_md_store);
  // close group
  out_wrapper->close_group();

  
  // house keeping
  if(opened_wrapper)
    out_wrapper->close_wrapper();
  

}


void Counted_vector::average_data(){
  if(!averaged_)
  {
    vector<float>::iterator data_it = data_array_.begin();
    vector<int>::iterator count_it = count_array_.begin();
    while(data_it!= data_array_.end() && count_it!= count_array_.end())
    {
      (*data_it) /=(*count_it);
      ++data_it;
      ++count_it;
    }
    averaged_ = true;
  }
  
    
}


void Counted_vector::unaverage_data(){
  if(averaged_)
  {
    vector<float>::iterator data_it = data_array_.begin();
    vector<int>::iterator count_it = count_array_.begin();
    while(data_it!= data_array_.end() && count_it!= count_array_.end())
    {
      (*data_it) *=(*count_it);
      ++data_it;
      ++count_it;
    }
    averaged_ = false;
  }
  
    
}

void Counted_vector::print() const{
  vector<float>::const_iterator data_it = data_array_.begin();
  vector<int>::const_iterator count_it = count_array_.begin();
  std::cout<<"d\tc"<<std::endl;
  while(data_it!= data_array_.end() && count_it!= count_array_.end())
  {
    std::cout<<(*data_it)<<"\t"<<(*count_it) <<std::endl;
    ++data_it;
    ++count_it;
  }
  std::cout<<"---"<<std::endl;
  
  

}
