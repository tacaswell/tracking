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
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of this Program grant
//you additional permission to convey the resulting work.

#ifndef CORR_SOFQ
#define CORR_SOFQ

#include <vector>
#include <string>
#include <complex>
#include "accumulator.h"




namespace tracking
{
/**
   Computes the self part of the susceptibility.
*/
#ifdef TRACKING_FLG
class Accum_chi4_self : public Accumulator
{
public:
  // basic inherited stuff
  void add_particle(const particle *) ;
  void out_to_wrapper(utilities::Generic_wrapper &,
                      const utilities::Md_store & md_store ) const ;

  // special stuff
  
  /**
     Constructor 
  */
  Accum_chi4_self(unsigned max_t,float min_l,float max_l,unsigned int l_steps,float (*w)(float,float));

  ~Accum_chi4_self();
  

private:
  /**
     maximum t step
   */
  unsigned int  max_step_;

  /**
     number of l values to try
   */
  unsigned int l_steps_;

  /**
     l step size
   */

  float l_step_sz_;

  /**
     l step size
   */

  float l_min_;
  
  /**
     w function.  

     See:
     J. Chem. Phys., Vol. 119, No. 14, 8 October 2003

     A.R. Abate and D.J. Durian. (2007). Topological persistence and dynamical heterogeneities near jamming. Physical Review E 76, 021306.

     PRL 95, 265701 (2005)

     260 nature physics VOL 3 APRIL 2007 www.nature.com/naturephysics

   */
  float (*w)(float,float) w_ = NULL;
  
  /**
     Data vector
   */
  std::vector<float> Q_;

 
  /**
     Data vector
   */
  std::vector<unsigned int> count_;
  
  /**
     internal hash function to deal with cord -> index 
   */
  inline  unsigned int to_indx(unsinged int a, unsigned int b)
  {
    return a * l_steps_ + b;
  }
    
};
#endif
}



#endif
