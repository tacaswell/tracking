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


#include "part_def.h"

#ifdef TRACKING_FLG
#include <stdexcept>

#include "accum_chi4_self.h"
#include "particle_track.h"
using std::vector;
using std::logic_error;
using std::runtime_error;

using utilities::Tuplef;

using tracking::Accum_chi4_self;
typedef unsigned int uint ;


void Accum_chi4_self::add_particle(const particle * init_pos )
{
  if(!init_pos)
    return;
  
  const particle * cur_p = init_pos;
  
  for(uint j = 0; j< max_step_; ++j)
  {                             
    // take one step forward
    cur_p = cur_p->get_next();
    // if we are off the end, we are done with this particle
    if(!cur_p)
      return;
    // we have another valid particle at this tau
    count_.at(j) +=1;
    // get the forward displacement
    float d = init_pos->distancesq_corrected(cur_p);
    for(uint k =0;k<l_steps_;++k)
    {
      Q_.at(to_indx(j,k)) += w_(d,l_step_sz_*k + l_min_);
    }
  }
}

void Accum_chi4_self::out_to_wrapper(utilities::Generic_wrapper &,
                    const utilities::Md_store & md_store ) const 
{
  throw logic_error("function not written");
}

Accum_chi4_self::Accum_chi4_self(unsigned max_t,float min_l,float max_l,unsigned int l_steps,float (*w)(float,float)):
  max_step_(max_t),
  l_steps_(l_steps),
  l_step_sz_((max_l - min_l)/l_steps),
  l_min_(min_l),
  w_(w),
  Q_(max_t * l_steps),count_(max_t)
{
}


Accum_chi4_self:: ~Accum_chi4_self()
{
}

void  Accum_chi4_self::add_to_chi4(vector<float>& Q_accum,vector<float>& Q2_accum,const int time_steps)const
{
  uint qsize = Q_.size();
  
  if(Q_accum.size() != qsize ||Q2_accum.size() != qsize)
    throw runtime_error("vector sizes do not match");

  for(uint j = 0; j<qsize; ++j)
  {
    float q = Q_.at(j);
    int count = count_.at(j/l_steps_);
    Q_accum.at(j) += q/(time_steps * count);
    Q_accum.at(j) += (q*q)/(time_steps * count * count);
  }
  
}
    

  



#endif
