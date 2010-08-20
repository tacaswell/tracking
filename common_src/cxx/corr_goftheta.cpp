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
#include "particle_track.h"
#include "generic_wrapper.h"



#include <cmath>
#include <stdexcept>
using std::runtime_error;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::acos;


using utilities::Generic_wrapper;
using utilities::Tuplef;


using tracking::Corr_goftheta;
using tracking::particle;

const float pi = acos(-1);

void Corr_goftheta::trim_nhood(const particle * p_in,const vector<const particle * > & nhood_in,vector<const particle * > &nhood_out) const
{
  int max = nhood_in.size();
  float max_range_sq = max_range_*max_range_;
  
  bool *indx = new bool[max];
  
  int keep_count = 0;

  for(int j =0; j<max;++j)
  {
    if(p_in->distancesq(nhood_in[j]) < max_range_sq)
    {
      indx[j] = true;
      ++keep_count;
    }
    else
    {
      indx[j] = false;
    }
  }
  nhood_out.clear();
  nhood_out.reserve(keep_count);
  

  
  for(int j =0; j<max;++j)
  {
    if(indx[j])
    {
      nhood_out.push_back(nhood_in[j]);
    }
  }
  delete [] indx;
  

}


void Corr_goftheta::compute(const particle * p_in,const vector<const particle*> & nhood_in)
{
  
  
  

  
  Tuplef p_in_pos = p_in->get_position();
  
  vector<const particle*> nhood;
  trim_nhood(p_in,nhood_in,nhood);
  int max_j = nhood.size();
  
  
//   vector<const particle*>::const_iterator p_end = nhood.end();
//   for(vector<const particle*>::const_iterator cur_part = nhood.begin();
//       cur_part!=p_end;++cur_part)
  for(int j = 0; j<max_j;++j)
  {
    //    const particle* part_ptr= *curr_part;
    const particle* part_ptr_outer= nhood[j];
    
    // skip self
    if(p_in == part_ptr_outer)
      continue;
    
    Tuplef p_outer_pos = p_in_pos - part_ptr_outer->get_position();
    if(p_outer_pos.magnitude() ==0 )
      continue;
    
    p_outer_pos.make_unit();
    
    for(int k = j+1;k<(max_j-1);++k)
    {
      //    const particle* part_ptr= *curr_part;
      const particle* part_ptr_inner= nhood[k];
      
      // skip self inside the loop too
      if(p_in == part_ptr_inner)
	continue;
      
      Tuplef p_inner_pos = (p_in_pos - part_ptr_inner->get_position());
      if(p_inner_pos.magnitude() == 0)
	continue;
      
      p_inner_pos.make_unit();
      
      
      float  dotp = p_inner_pos.dot(p_outer_pos);
      if(dotp < -1)
	dotp = -1;
      if(dotp >1)
	dotp = 1;
      
      float theta = acos(dotp);
      
      
      
      // assume the nhood is the right size
      
      unsigned int ind = (unsigned int)floor(n_bins_ * (theta)/(pi));
      if(ind ==n_bins_)
	ind = 0;
      try
      {
	
	++(bin_count_.at(ind));
      }
      catch(...)
      {
	cout<<"ind "<<ind<<endl;
	cout<<"theta "<<theta<<endl;
	cout<<"p inner "<<p_inner_pos<<endl;
	cout<<"p outer "<<p_outer_pos<<endl;
	cout<<"p outer_basic " <<part_ptr_outer->get_position()<<endl;
	cout<<"p_in_put "<<p_in_pos<<endl;
	
	cout<<"dot prod "<<  dotp<<endl;
	throw runtime_error("stupidity");
	
      }
      
    }
    
    
  }
  
  ++parts_added_;
  

}



Corr_goftheta::Corr_goftheta(int bins,float max,string& name):
  bin_count_(bins),bin_edges_(bins),n_bins_(bins),
  max_range_(max),name_(name),parts_added_(0)
{
  if(bins <1)
    throw runtime_error("number of bins must be greater than 0");
  
//   if(max_range_>particle::get_neighborhood_range())
//     throw runtime_error("maximum range past what particles know about");
  
  float bin_sz = pi/bins;
  for( int j= 0;j<bins;++j)
  {
    bin_count_.at(j) = 0;
    bin_edges_.at(j) = j*bin_sz;
  }
  std::cout<<n_bins_<<std::endl;
  
}

void Corr_goftheta::out_to_wrapper(Generic_wrapper & in,const string& name)const
{
  vector<float> tmp;
  normalize(tmp);
  
  in.open_wrapper();
  in.open_group(name);
  //in.add_metadata();
  
  const float * yar = &tmp.front();
  
  in.add_dset(1,&n_bins_,utilities::V_FLOAT,yar,"bin_count");
  yar = &bin_edges_.front();
  in.add_dset(1,&n_bins_,utilities::V_FLOAT,yar,"bin_edges");

  in.close_wrapper();
  
}


void Corr_goftheta::normalize(std::vector<float> & out) const
{
  // tac 2009-10-09
  // changed to float to prevent integer over flow
  float count_sum = 0;
  for(unsigned int j = 0; j<n_bins_;++j)
    count_sum += bin_count_[j];
  
  // this does not need to be averaged by the number of particles
  // added because in the normalization the average is multiplied by
  // the number of particles added so it cancels out
  float avg = (count_sum)/n_bins_;
  


  out.resize(n_bins_);
  for(unsigned int j = 1;j<(n_bins_);++j)
  {
    out[j] = bin_count_[j]/avg;
    
  }


}
