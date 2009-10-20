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
#ifndef HIST2
#define HIST2

#include <vector>
#include <exception>
#include <iostream>

#include "part_def.h"

namespace utilities{
//forward declare
class Generic_wrapper_base;

/**
   Base class for historgram objects.
   Implements a linearly spaced histogram.

*/
class Histogram2D{
public:
  /**
     Add a data point to the histogram.  This may be better to do with just
     a number of overlaoded functions.  
   */
  template<class T>
  void add_data_point(T in1,T in2);
  
  /**
     Returns a vector of length number_bins of the values in the bins
   */
  std::vector<int> get_bin_values();

  /*
     Returns a vector of the edges in the bins, has length number_bins + 1
   */
  //  std::vector<double> get_bin_edges();

  /**
     Spits something reasonable out to std
   */
  void print();

//   int get_over_count(){
//     return over_count_;
//   }


//   int get_under_count(){
//     return under_count_;
//   }
  /**
     Default constructor
   */
  Histogram2D(int num_bins1, double bottom1 , double top1,int num_bins2, double bottom2 , double top2);

  /**
    Out puts the values of the histogram to a (bins +2)x2 array.  the
    first column is the values of the bins, the second the bottom edge
    of the bins.  The extra entry at the end are the number of entries
    outside of the 
  */
  void output_to_wrapper(Generic_wrapper_base * wrapper_out);
  

  
  ~Histogram2D(){};

  void add_data_point(const utilities::Pair & in );
  
protected:
  /**
     Vector of the number of counts per bin
   */
  std::vector<int> hist_array_;




  /**
     Number of points that are below the bottom of the histogram
   */
  int under_count1_;

  /**
     Number of points that are over the top of the histogram
   */
  int over_count1_;

  /**
     Total number of bins
   */
  int number_bins1_;

  /**
     The maximum value to be included in the histogram
  */
  double top_edge1_;
  
  /**
     The minimum value to be included in the histogram
  */
  double bottom_edge1_;

  /**
     Width of each bin
   */
  double bin_width1_;


  /**
     Number of points that are below the bottom of the histogram
   */
  int under_count2_;

  /**
     Number of points that are over the top of the histogram
   */
  int over_count2_;

  /**
     Total number of bins
   */
  int number_bins2_;

  /**
     The maximum value to be included in the histogram
  */
  double top_edge2_;
  
  /**
     The minimum value to be included in the histogram
  */
  double bottom_edge2_;

  /**
     Width of each bin
   */
  double bin_width2_;
private:


};
  
template<class T>
void Histogram2D::add_data_point(T in1,T in2){
  using std::cout;
  using std::endl;
  double tmp_in1 = (double) in1;
  double tmp_in2 = (double) in2;
  if(tmp_in1 < bottom_edge1_){
    ++under_count1_;
    return;
  }
  if(tmp_in1>= top_edge1_){
    ++over_count1_;
    return;
  }
  if(tmp_in2 < bottom_edge2_){
    ++under_count2_;
    return;
  }
  if(tmp_in2>= top_edge2_){
    ++over_count2_;
    return;
  }
  
  int bin1 =(int)((tmp_in1 - bottom_edge1_)/bin_width1_);
  int bin2 =(int)((tmp_in2 - bottom_edge2_)/bin_width2_);
  

  //  ++(*(hist_array_ptr_ + ((int)((tmp_in - bottom_edge_)/bin_width_))));
  //  cout<<in<<"\t"<<((int)((tmp_in - bottom_edge_)/bin_width_))<<endl;
  try
  {
    ++hist_array_.at(bin2*number_bins1_ + bin1);
  }
  catch(std::exception & e)
  {
    cout<<e.what()<<endl;
    cout<<tmp_in1<<"\t"
	<<bottom_edge1_<<"\t"
	<<bin_width1_<<"\t"<<endl;
    cout<<tmp_in2<<"\t"
	<<bottom_edge2_<<"\t"
	<<bin_width2_<<"\t"<<endl;
    
  }
}




}

#endif
