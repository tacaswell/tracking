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

#ifndef CORR_SOFQ
#define CORR_SOFQ

#include <vector>
#include <string>
#include <complex>
#include "accumulator.h"




namespace tracking
{
/**
   s(|q|) computation.  This takes in a specific direction in
   reciprocal space and computes s(|q|) along that direction.  If the
   assumption that the particles have no preferred direction holds,
   then this saves a lot of computation time.

*/

class Accum_chi4 : public Accumulator
{
public:
  // basic inherited stuff
  void add_particle(const particle *) ;
  void out_to_wrapper(utilities::Generic_wrapper & ) const ;

  // special stuff
  
  /**
     constructor,will gain arguments 
     @param[in] a_range Pair with the min and max a assumed to be in the natural units of length of the data
     @param[in] t_range min and max t
  */
  Accum_chi4(const utilities::Tuple<float,2>& a_range, 
	     const int a_step,
	     const utilities::Tuple<int,2>& t_range,
	     const int t_step)

  ~Accum_chi4();
  

private:
  /**
     
   */
  
};

}



#endif
