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


#ifndef TRK_ACCUM_BASE
#define TRK_ACCUM_BASE

#include "part_def.h"

namespace tracking
{

/**
   An abstract base class for track accumulator computations.  That is
   computations that only care about displacement values and the time
   step between them, with out worrying about correlations with in
   tracks or anything about the spatial correlations of the particles.
 */
class Trk_accumulator
{

public:
  /**
     Add a track segment displacement

     @param[in] displacement The net displacement of the track segment.
     @param[in] steps The number of time steps in the track segment.
   */
  virtual void add_disp(const utilities::Tuplef & displacement,unsigned steps) = 0;
  
  /**
     Returns the maximum step that the accumulator can deal with.
   */
  virtual unsigned max_step()const = 0;
  
};

  
}


#endif
