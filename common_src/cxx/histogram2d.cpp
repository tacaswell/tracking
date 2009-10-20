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
#include "histogram2d.h"
#include <iostream>
#include "generic_wrapper_base.h"
#include "triple.h"
#include "pair.h"

using namespace utilities;

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

typedef vector<int> vectori;
typedef vector<double> vectord;

void Histogram2D::print(){
  for(vectori::iterator it = hist_array_.begin();
      it<hist_array_.end(); it++)
    cout<<*it<<"\t";
  cout<<endl;
//   cout<<"over: "<<over_count_ <<"\t"<<"under: "<<under_count_<<endl;
}


Histogram2D::Histogram2D(int num_bins1, double bottom1 , double top1,int num_bins2, double bottom2 , double top2):
  hist_array_(num_bins1*num_bins1,0), under_count1_(0), over_count1_(0), number_bins1_(num_bins1),
  top_edge1_(top1), bottom_edge1_(bottom1), bin_width1_((top1-bottom1)/num_bins1),
  under_count2_(0), over_count2_(0), number_bins2_(num_bins2),
  top_edge2_(top2), bottom_edge2_(bottom2), bin_width2_((top2-bottom2)/num_bins2)
{
  
  if(bin_width1_<0){
    cerr<<"negative bin width 1, assuming input flipped and reversing top and bottom"<<endl;
    bottom_edge1_ = top1;
    top_edge1_ = bottom1;
    bin_width1_ = -bin_width1_;
  }

  if(bin_width2_<0){
    cerr<<"negative bin width 2, assuming input flipped and reversing top and bottom"<<endl;
    bottom_edge2_ = top2;
    top_edge2_ = bottom2;
    bin_width2_ = -bin_width2_;
  }

  

}

vectori Histogram2D::get_bin_values(){
  
  return vectori(hist_array_);
}


void Histogram2D::add_data_point(const  utilities::Pair& in )
{
  in[1];
  
  add_data_point(in[0],in[1]);
}


// vectord Histogram2D::get_bin_edges(){
//   vectord tmp;
//   tmp.reserve(number_bins1_ + 1);
  
//   for(int j = 0; j<number_bins_+1; j++)
//     tmp.push_back(bottom_edge_  + j* bin_width_);

//   if(top_edge_!= tmp.back())
//     cerr<<"round error in generateing bin edges: "<<top_edge_ - tmp.back()<<endl;
  
//   return vectord(tmp);
// }


void Histogram2D::output_to_wrapper(Generic_wrapper_base * wrapper_out)
{
  wrapper_out->initialize_wrapper();
  for(int i =0; i<number_bins2_;++i)
  {
    for(int j =0; j<number_bins1_;++j)
      {
	wrapper_out->start_new_row();
	wrapper_out->append_to_row(hist_array_[j + number_bins2_*i]);
	wrapper_out->append_to_row(bottom_edge1_ + j*bin_width1_);
	wrapper_out->append_to_row(bottom_edge2_ + i*bin_width2_);
	wrapper_out->finish_row();
      }
  }
  

//   wrapper_out->start_new_row();
//   wrapper_out->append_to_row(over_count_);
//   wrapper_out->append_to_row(top_edge_);
//   wrapper_out->finish_row();


//   wrapper_out->start_new_row();
//   wrapper_out->append_to_row(under_count_);
//   wrapper_out->append_to_row(bottom_edge_ - bin_width_);
//   wrapper_out->finish_row();

			     
  wrapper_out->finalize_wrapper();
  
}
