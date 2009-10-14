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

//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

#include "corr_goftheta.h"
#include "particle_base.h"
#include "particle_track.h"
#include "generic_wrapper.h"

#include "tuple.h"

#include "gnuplot_i.hpp"


using std::vector;
using std::string;
using std::cout;
using std::endl;

using utilities::Generic_wrapper;
using utilities::Tuple;


using tracking::Corr_goftheta;
using tracking::particle_base;

const float pi = acos(-1);


void Corr_goftheta::compute(const particle_track * p_in,const vector<const particle_track*> & )
{
  throw "Corr_goftheta: not implemnted yet:void Corr_goftheta::compute(const particle_track * p_in,const vector<const particle_track*> & )";
  
  //  compute(static_cast<const particle_base*>(p_in));
}

void Corr_goftheta::compute(const particle_base * p_in,const vector<const particle_base*> & nhood)
{
  
  
  int max_j = nhood.size();
  
  Tuple p_in_pos = p_in->get_position();
  
//   vector<const particle_base*>::const_iterator p_end = nhood.end();
//   for(vector<const particle_base*>::const_iterator cur_part = nhood.begin();
//       cur_part!=p_end;++cur_part)
  for(int j = 0; j<max_j;++j)
  {
    //    const particle_base* part_ptr= *curr_part;
    const particle_base* part_ptr= nhood[j];
    
    // skip self
    if(p_in == part_ptr)
      continue;
  
    // assume the nhood is the right size
    float theta = part_ptr->get_theta(p_in_pos) ;
    int ind = floor(n_bins_ * (theta + pi)/(2*pi));
    if(ind ==n_bins_)
      ind = 0;
    ++(bin_count_.at(ind));
    
    
  }
  
  ++parts_added_;
  

}



Corr_goftheta::Corr_goftheta(int bins,float max,string& name):
  bin_count_(bins),bin_edges_(bins),n_bins_(bins),
  max_range_(max),name_(name),parts_added_(0)
{
  if(bins <1)
    throw "number of bins must be greater than 0";
  
//   if(max_range_>particle_base::get_neighborhood_range())
//     throw "maximum range past what particles know about";
  
  float bin_sz = 2*pi/bins;
  for( int j= 0;j<bins;++j)
  {
    bin_count_.at(j) = 0;
    bin_edges_.at(j) = j*bin_sz * 180/pi;
  }
}

void Corr_goftheta::out_to_wrapper(Generic_wrapper & in)const
{
  vector<float> tmp;
  normalize(tmp);
  
  in.open_wrapper();
  in.open_group(name_);
  //in.add_metadata();
  
  const float * yar = &tmp.front();
  
  in.add_dset(1,&n_bins_,utilities::V_FLOAT,yar,"bin_count");
  yar = &bin_edges_.front();
  in.add_dset(1,&n_bins_,utilities::V_FLOAT,yar,"bin_edges");

  in.close_wrapper();
  
}


void Corr_goftheta::display()const
{

  vector<float> tmp;
  normalize(tmp);
  
  Gnuplot g(bin_edges_,tmp,"g(theta)","steps");
  g.set_grid().replot();
  wait_for_key();
  


}

void Corr_goftheta::normalize(std::vector<float> & out) const
{
  // tac 2009-10-09
  // changed to float to prevent integer over flow
  float count_sum = 0;
  for(int j = 0; j<n_bins_;++j)
    count_sum += bin_count_[j];
  
  // this does not need to be averaged by the number of particles
  // added because in the normalization the average is multiplied by
  // the number of particles added so it cancels out
  float avg = (count_sum)/n_bins_;
  


  out.resize(n_bins_);
  for(int j = 1;j<(n_bins_);++j)
  {
    out[j] = bin_count_[j]/avg;
    
  }


}
