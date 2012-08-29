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

#ifndef CORR_THETA_2PT
#define CORR_THETA_2PT

#include <vector>
#include <string>
#include "corr.h"
#include "histogram.h"


namespace utilities
{
class Generic_wrapper;
class Md_store;

}


namespace tracking
{
/**
   Computes the distribution made by the angles between pairs of particles to vertical for pairs in
   the given separation range.
 */
class Corr_theta_2pt:public Corr
{
public:
  // basic inherited stuff
  void compute(const particle *,const std::vector<const particle*> & ) ;
  void out_to_wrapper(utilities::Generic_wrapper &,const std::string &,const utilities::Md_store * ) const ;
  float get_max_range()const;
  
  /**
     constructor.  
     @param[in] t_bins the number of theta bins
     @param[in] max_r the maximum displacement to be included
     @param[in] min_r the minimum displacement to be included
   */
  Corr_theta_2pt(int t_bins,float min_r, float max_r);
    

private:
  /**
       A vector of histograms for 
   */
  utilities::Histogram  theta_;

  /**
     minimum r range
   */
  float min_r_range2_;

  /**
     maximum r range
   */
  float max_r_range2_;
  
  
};

}



#endif
