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

 

#include "part_def.h"
#ifdef TRACKING_FLG
// only include this if the particles know about tracking

#ifndef CORR_PAIR_DIFF
#define CORR_PAIR_DIFF

#include <vector>
#include <string>
#include "corr.h"

#include "histogram.h"

namespace utilities
{
class Generic_wrapper;
}


namespace tracking
{
/**
   computes the motion of a pair of particles in relation to each
   other.  Binned by their initial separation and the wait time.
 */
class Corr_pair_diffusion:public Corr
{
public:
  // basic inherited stuff
  void compute(const particle *,const std::vector<const particle*> & ) ;
  void out_to_wrapper(utilities::Generic_wrapper & wrapper_out,
                                           const std::string & g_name,
                                           const utilities::Md_store * md_store)const;
  
  


  
  /**
     constructor.  

     @param[in] n_dbins then number of bins in displacement histogram
     @param[in] max_disp the maximum displacement to consider
     @param[in] n_rbins then number of bins in separation histogram
     @param[in] min_r the minimum initial separation to consider
     @param[in] max_r the maximum initial separation to consider
     @param[in] min_t the minimum time lag to consider
     @param[in] max_t the maximum time lag to consider
     @param[in] t_step the time step to between computed frames
   */
  Corr_pair_diffusion(unsigned int n_dbins,
                      float max_disp,
                      unsigned int n_rbins,
                      float min_r,
                      float max_r,
                      unsigned int min_t,
                      unsigned int t_stride,
                      unsigned int t_step_count);
  


  float get_max_range()const
  {
    return sqrt(max_r_square_);
  }
  
private:
  /**
     Main data structure.  2 levels of nesting, outer layer is time,
     inner layer is initial separation.  Histogram object take care of
     the displacement layer.
   */
  std::vector< std::vector<utilities::Histogram> > data_prll_;
  std::vector< std::vector<utilities::Histogram> > data_perp_;
  
  const float min_r_;
  const float max_r_;
  const unsigned int n_rbins_;
  

  const   float min_r_square_;
  const   float max_r_square_;  

  const  unsigned int min_t_;
  const  unsigned int t_stride_;
  const  unsigned int t_step_count_;

  
  
};

}



#endif
#endif
