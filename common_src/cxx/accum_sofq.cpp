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


#include "accum_sofq.h"
#include "particle_track.h"
#include <iostream>

using std::cout;
using std::endl;


using utilities::Tuplef;
using utilities::Tuplei;
using tracking::Corr_sofq;


void Corr_sofq::compute(const particle *,const std::vector<const particle*> & ) 
{
  
  return;
  
}

void Corr_sofq::out_to_wrapper(utilities::Generic_wrapper & ) const 
{
  return;
  
}

Corr_sofq::Corr_sofq(const utilities::Tuplef& max_q,const utilities::Tuplei & n_bins):
  n_bins_(n_bins),max_range_(max_q),q_(n_bins_),s_of_q_(n_bins_)
{

  
  // fill in q values
  q_.print();
  
  Tuplei indx;
  do
  {
    q_(indx) = max_range_*(Tuplef(indx+Tuplei(1))/Tuplef(n_bins_));
  }
  while(step_indx(indx));
  q_.print();

}

Corr_sofq::~Corr_sofq()
{
  

}

  
void Corr_sofq::display() const
{
  

}



bool Corr_sofq::step_indx(utilities::Tuplei & indx) const
{
  
  for(int j = 0;j<rank_;++j)
  {
    if(indx[j]+1 < n_bins_[j])
    {
      ++indx[j];
      return true;
    }
    else
      indx[j] = 0;
    
  }
  return false;
  
}
