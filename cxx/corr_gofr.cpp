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

#include "corr_gofr.h"
#include "particle_base.h"
#include "particle_track.h"
#include "generic_wrapper_base.h"




using std::vector;

using utilities::Generic_wrapper_base;


using tracking::Corr_gofr;
using tracking::particle_base;






void Corr_gofr::compute(const particle_base * p_in)
{
  float max_sq = max_range_*max_range_;
  

  const vector<const particle_base*> nhood = p_in->get_neighborhood();
  vector<const particle_base*>::const_iterator p_end = nhood.end();
  for(vector<const particle_base*>::const_iterator cur_part = nhood.begin();
      cur_part!=p_end;++cur_part)
  {
    float tmp_d = p_in->distancesq(*cur_part);
    if(tmp_d<max_sq)
    {
      int ind = (int)(n_bins_*sqrt(tmp_d)/max_range_);
      ++(bin_count_.at(ind));
      
    }
    
  }
  

}

void Corr_gofr::compute(const particle_track * p_in)
{
  compute(static_cast<const particle_base*>(p_in));
}

Corr_gofr::Corr_gofr(int bins,float max):
  bin_count_(bins),bin_edges_(bins),n_bins_(bins),
  max_range_(max)
{
  if(bins <1)
    throw "number of bins must be greater than 0";
  
  if(max_range_>particle_base::get_neighborhood_range())
    throw "maximum range past what particles know about";
  
  float bin_sz = max_range_/bins;
  for( int j= 0;j<bins;++j)
  {
    bin_count_.at(j) = 0;
    bin_edges_.at(j) = j*bin_sz;
  }
}

void Corr_gofr::out_to_wrapper(Generic_wrapper_base & in)const
{
}
