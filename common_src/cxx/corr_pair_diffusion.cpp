//Copyright 2012 Thomas A Caswell
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
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.


#include "part_def.h"
#ifdef TRACKING_FLG
// this code only works for 2D
#if DIM_COUNT == 2

#include "corr_pair_diffusion.h"
#include "particle_track.h"
#include "generic_wrapper.h"
#include <stdexcept>


using std::runtime_error;
using std::vector;
using std::string;

using utilities::Generic_wrapper;


using tracking::Corr_gofr;
using tracking::particle;
Corr_pair_diffusion::Corr_pair_diffusion(unsigned int n_dbins,
                                         float max_disp,
                                         unsigned int n_rbins,
                                         float min_r,
                                         float max_r,
                                         unsigned int min_t,
                                         unsigned int t_stride,
                                         unsigned int t_step_count):
  data_(t_step_count,vector<utilities::histogram>(n_rbins,utilities::histogram(n_dbins,-max_disp,max_disp))),
  min_r_(min_r),
  max_r_(max_r),
  n_rbins_(n_rbins)
  min_r_square_(min_r*min_r),
  max_r_square_(max_r*max_r)
  min_t_(min_t),
  t_stride_(t_stride),
  t_step_count_(t_step_count)
{
  

}

  


void Corr_gofr::compute(const particle * p_in,const vector<const particle*> & nhood)
{

  unsigned int max_j = nhood.size();
  tuplef pos_in = p_in->get_position()
  for(unsigned int j = 0; j<max_j;++j)
  {
    // pick out the next particle from the neighborhood 
    const particle* p_nhood= nhood[j];
    //    if(p_in == nhood[j])
    if(p_in == p_nhood)
      continue;
    
    tuplef pos_nhood = p_nhood->get_position();
    
    // make sure we only do each pair once.
    if( pos_nhood[0] < pos_in[0])
      continue

    float tmp_d = p_in->distancesq(p_nhood);
    if(tmp_d<max_r_square_ || tmp_d > min_r_square_)
    {
      // set up variables
      tuplef prll_director = pos_nhood - pos_in;
      prll_director.make_unit();
      tuplef perp_director = tuplef(-prll_director[1],prll_director[0]);
      
      unsigned int r_ind = (unsigned int) (n_rbins * (sqrt(tmp_d) - min_r_)/(max_r_-min_r_);
      const particle * p_in_nxt,p_nhood_nxt,p_in_cur,p_nhood_cur;
      p_in_cur = p_in;
      p_nhood_cur = p_nhood;
      
      // get first set of displacements
      if(!(p_in->step_forwards(min_t_,p_in_nxt) && 
           p_nhood->step_forwards(min_t_,p_nhood_nxt)))
        continue;
      for(unsigned int t = 0; t<t_step_count_;++t)
      {
        tuplef rel_disp = (p_nhood->get_corrected_disp(p_nhood_nxt) -
                           p_in->get_corrected_disp(p_in_nxt));
        
        data_prll_.at(t).at(r_ind).add_data_point(prll_director.dot(rel_disp));
        data_perp_.at(t).at(r_ind).add_data_point(perp_director.dot(rel_disp));

        // set the cur pointer to point to the next objects
        p_in_cur = p_in_nxt;
        p_nhood_cur = p_nhood_nxt;
        // get the new next objects
        if(!(p_in->step_forwards(t_stride_,p_in_nxt) && 
             p_nhood->step_forwards(t_stride_,p_nhood_nxt)))
          // if either track does not go that far, break out of this loop
          break;
        
        
      }
      

    }
  }
  

  

}


void Corr_gofr::out_to_wrapper(Generic_wrapper & in,
			       const std::string & g_name,
			       const utilities::Md_store * md_store)const
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
  

  if(md_store)
    in.add_meta_data(md_store);
  in.add_meta_data("rho", rho );
  
  in.add_meta_data("max_range",max_range_);
  in.add_meta_data("nbins",n_bins_);
  in.add_meta_data("total_number",parts_added_);
  in.add_meta_data("plane_count",plane_count_);
  in.add_meta_data("temperature",temperature_sum_/plane_count_);
  
  in.close_group();
  
  if(opened_wrapper)
    in.close_wrapper();
  
}

#endif DIM_COUNT == 2
#endif TRACKING_FLG
