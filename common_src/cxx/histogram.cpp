//Copyright 2009,2010 Thomas A Caswell
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
#include "histogram.h"
#include <iostream>
#include "generic_wrapper.h"

#include "md_store.h"

using namespace utilities;

using std::string;

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

typedef vector<int> vectori;
typedef vector<float> vectord;

void Histogram::print() const{
  for(vectori::const_iterator it = hist_array_.begin();
      it<hist_array_.end(); it++)
    cout<<*it<<"\t";
  cout<<endl;
  cout<<"over: "<<over_count_ <<"\t"<<"under: "<<under_count_<<endl;
}


Histogram::Histogram(int num_bins, float bottom, float top):
  hist_array_(num_bins,0), under_count_(0), over_count_(0), number_bins_(num_bins),
  top_edge_(top), bottom_edge_(bottom), bin_width_((top-bottom)/num_bins)
{
  
  if(bin_width_<0){
    cerr<<"negative bin width, assuming input flipped and reversing top and bottom"<<endl;
    bottom_edge_ = top;
    top_edge_ = bottom;
    bin_width_ = -bin_width_;
  }

  

}

vectori Histogram::get_bin_values() const{
  
  return vectori(hist_array_);
}


vectord Histogram::get_bin_edges() const{
  vectord tmp;
  tmp.reserve(number_bins_ + 1);
  
  for(int j = 0; j<number_bins_+1; j++)
    tmp.push_back(bottom_edge_  + j* bin_width_);

  if(top_edge_!= tmp.back())
    cerr<<"round error in generateing bin edges: "<<top_edge_ - tmp.back()<<endl;
  
  return vectord(tmp);
}


void Histogram::output_to_wrapper(Generic_wrapper * wrapper_out,
				  const string & g_name,
				  const string & count_name,
				  const string & edges_name,
				  const Md_store* g_md_store
				  )const
{
    // house keeping
  bool opened_wrapper = false;
  
  if(!wrapper_out->is_open())
  {
    wrapper_out->open_wrapper();
    opened_wrapper = true;
      
  }
  wrapper_out->open_group(g_name);
  
  
  unsigned int n_bins = hist_array_.size();
  
  // open group
  
  // shove in count data
  const int * tmpi = &(hist_array_.front());
  wrapper_out->add_dset(1,&n_bins,utilities::V_INT,tmpi,count_name);

  vector<float> bin_edges(n_bins);
  for(unsigned int j = 0;j<n_bins;++j)
    bin_edges[j] = bottom_edge_ + j*bin_width_;
  
  const float * tmpf = &(bin_edges.front());
  wrapper_out->add_dset(1,&n_bins,utilities::V_FLOAT,tmpf,edges_name);
  
  // shove in meta data from store
  if(g_md_store)
    wrapper_out->add_meta_data(g_md_store);

  // add meta data specific to 
  wrapper_out->add_meta_data("over_count",over_count_);
  wrapper_out->add_meta_data("under_count",under_count_);
  wrapper_out->add_meta_data("nbins",n_bins);
  wrapper_out->add_meta_data("bottom_edge",bottom_edge_);
  wrapper_out->add_meta_data("top_edge",top_edge_);
  wrapper_out->add_meta_data("bin_width",bin_width_);
  
  // close group
  wrapper_out->close_group();

  
  // house keeping
  if(opened_wrapper)
    wrapper_out->close_wrapper();
  
  
}


