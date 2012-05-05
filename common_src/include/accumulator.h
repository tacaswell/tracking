//Copyright 2010 Thomas A Caswell
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

#ifndef CUM_
#define CUM_

#include <vector>

#include "part_def.h"

namespace utilities
{
class Generic_wrapper;
}





namespace tracking
{



/**
   An ABC of objects that accumulate information from particles.  In
   my naming scheme Accumulator objects are for measurements that do
   not care about a particles neighborhood, ex \f$s(q)\f$.  This is
   different than Corr objects that deal with measurements that depend
   on the neighborhood around the particle.

   This may need to pick up temperature functionality like 
   Corr. This class and Corr might be able to be merged.

*/
class Accumulator
{
public:
  /**
     Adds a particle.  The sub class determines what is computed from
     the particle.
   */
  virtual void add_particle(const particle *) = 0;

  /**
     outputs the result of the computation to the wrapper
   */
  virtual void out_to_wrapper(utilities::Generic_wrapper &,
			 const utilities::Md_store & md_store ) const =0;
  /**
     Destructor
   */
  virtual ~Accumulator(){}; 
  

};


}


#endif
