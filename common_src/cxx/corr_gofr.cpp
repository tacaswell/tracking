//Copyright 2009-2010 Thomas A Caswell
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

#include "corr_gofr.h"
#include "particle_track.h"
#include "generic_wrapper.h"


using std::vector;
using std::string;

using utilities::Generic_wrapper;


using tracking::Corr_gofr;
using tracking::particle;


const float pi = acos(-1);

void Corr_gofr::compute(const particle * p_in,const vector<const particle*> & nhood)
{
  float max_sq = max_range_*max_range_;
  
  int max_j = nhood.size();
  
//   vector<const particle*>::const_iterator p_end = nhood.end();
//   for(vector<const particle*>::const_iterator cur_part = nhood.begin();
//       cur_part!=p_end;++cur_part)
  for(int j = 0; j<max_j;++j)
  {
    //    const particle* part_ptr= *curr_part;
    const particle* part_ptr= nhood[j];
    //    if(p_in == nhood[j])
    if(p_in == part_ptr)
      continue;
    
    float tmp_d = p_in->distancesq(part_ptr);
    if(tmp_d<max_sq)
    {
      int ind = (int)(n_bins_*sqrt(tmp_d)/max_range_);
      ++(bin_count_.at(ind));
    }
  }
  
  ++parts_added_;
  

}



Corr_gofr::Corr_gofr(int bins,float max,int comp_num,int dset,int read_comp):
  bin_count_(bins),bin_edges_(bins),n_bins_(bins),
  max_range_(max),parts_added_(0),dset_(dset),comp_num_(comp_num),read_comp_(read_comp)
{
  if(bins <1)
    throw "number of bins must be greater than 0";
  
  float bin_sz = max_range_/bins;
  for( int j= 0;j<bins;++j)
  {
    bin_count_[j] = 0;
    bin_edges_[j] = j*bin_sz;
  }
}

void Corr_gofr::out_to_wrapper(Generic_wrapper & in,const std::string & g_name)const
{
  bool opened_wrapper = false;
  
  vector<float> tmp;
  float rho = normalize(tmp);

  if(!in.is_open())
  {
    in.open_wrapper();
    opened_wrapper = true;
    
  }
  
  in.open_group(g_name);
  //in.add_metadata();
  
  const float * yar = &tmp.front();
  
  in.add_dset(1,&n_bins_,utilities::V_FLOAT,yar,"bin_count");
  yar = &bin_edges_.front();
  in.add_dset(1,&n_bins_,utilities::V_FLOAT,yar,"bin_edges");

  in.add_meta_data("comp_num",comp_num_);
  in.add_meta_data("iden_key",read_comp_);
  in.add_meta_data("dset",dset_);
  in.add_meta_data("max_range",max_range_);
  in.add_meta_data("nbins",n_bins_);
  in.add_meta_data("rho", rho );
  in.add_meta_data("temperature",temperature_sum_/plane_count_);
  
  in.close_group();
  
  if(opened_wrapper)
    in.close_wrapper();
  
}


float Corr_gofr::normalize(vector<float> & out)const
{
  // tac 2009-10-09
  // changed to float to prevent integer over flow
  float count_sum = 0;
  for(int j = 0; j<n_bins_;++j)
    count_sum += bin_count_[j];
  
  // this does not need to be averaged by the number of particles
  // added because in the normalization the average is multiplied by
  // the number of particles added so it cancels out

#if DIM_COUNT == 2
  float avg = (count_sum)/(pi*max_range_*max_range_);
#elif DIM_COUNT == 3
  float avg = (count_sum)/(pi*max_range_*max_range_*max_range_*4.0/3.0);
#endif
  
  out.resize(n_bins_);

  float front,back,norm_factor;
  // tac 2010-05-27
  // I don't see why the first one is a special case, as bin_edges_[0] should be 0
  
  for(int j = 0;j<(n_bins_-1);++j)
  {
    // made it easier to read/more efficient
    front = bin_edges_[j+1];
    back = bin_edges_[j];
    
#if DIM_COUNT == 2
    norm_factor = ( avg * pi*(front * front - back*back));
#elif DIM_COUNT ==3
    norm_factor = ( avg * pi*(front * front * front - back * back * back))*4/3;
#endif
    out[j] = bin_count_[j]/norm_factor;
  }

  // handle last bin separately
  front = max_range_;
  back = bin_edges_[n_bins_-1];
      
#if DIM_COUNT == 2
  norm_factor = ( avg * pi*(front * front - back*back));
#elif DIM_COUNT ==3
  norm_factor = ( avg * pi*(front * front * front - back * back * back))*4/3;
#endif
  out[n_bins_-1] = bin_count_[n_bins_-1]/norm_factor;
  
  return avg/(float)parts_added_;
  
}

void Corr_gofr::get_bin_edges(std::vector<float> & out) const
{
  out = bin_edges_;
}
