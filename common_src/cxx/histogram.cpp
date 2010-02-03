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
#include "generic_wrapper_base.h"
#include "gnuplot_i.hpp"
using namespace utilities;

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


void Histogram::output_to_wrapper(Generic_wrapper_base * wrapper_out)const
{
  wrapper_out->initialize_wrapper();
  for(int j =0; j<number_bins_;++j)
  {
    wrapper_out->start_new_row();
    wrapper_out->append_to_row(hist_array_[j]);
    wrapper_out->append_to_row(bottom_edge_ + j*bin_width_);
    wrapper_out->finish_row();
  }

  wrapper_out->start_new_row();
  wrapper_out->append_to_row(over_count_);
  wrapper_out->append_to_row(top_edge_);
  wrapper_out->finish_row();


  wrapper_out->start_new_row();
  wrapper_out->append_to_row(under_count_);
  wrapper_out->append_to_row(bottom_edge_ - bin_width_);
  wrapper_out->finish_row();

			     
  wrapper_out->finalize_wrapper();
  
}


void Histogram::display() const
{

  vector<float> tmp = get_bin_edges();
  tmp.pop_back();
  
  vector<float> tmp2 = vector<float>(0);
  tmp2.reserve(hist_array_.size());
  
  for(vectori::const_iterator it = hist_array_.begin();
      it<hist_array_.end(); ++it)
    tmp2.push_back(*it);
  
  try
  {
    
    Gnuplot g(tmp,tmp2,"track length","histeps");
    g.set_grid();
    wait_for_key();
  }
  catch(GnuplotException & e)
  {
    cout<<e.what()<<endl;
  }
  
}



void Histogram::display(Gnuplot & g ) const
{

  vector<float> tmp = get_bin_edges();
  tmp.pop_back();
  
  vector<float> tmp2 = vector<float>(0);
  tmp2.reserve(hist_array_.size());
  
  for(vectori::const_iterator it = hist_array_.begin();
      it<hist_array_.end(); ++it)
    tmp2.push_back(*it);
  
  try
  {
    
    g.plot_xy(tmp,tmp2);
    g.set_style("histograms");
    
    g.set_grid();
    g.replot();
    
    wait_for_key();
  }
  catch(GnuplotException & e)
  {
    cout<<e.what()<<endl;
  }
  
}

