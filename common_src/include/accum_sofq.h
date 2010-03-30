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

#ifndef CORR_SOFQ
#define CORR_SOFQ

#include <vector>
#include <string>

#include "accumulator.h"
#include "ndarray.h"



namespace tracking
{
/**
   simple g(r) computation
 */
class Corr_sofq : public Accumulator
{
public:
  // basic inherited stuff
  void compute(const particle *,const std::vector<const particle*> & ) ;
  void out_to_wrapper(utilities::Generic_wrapper & ) const ;

  // special stuff
  
  /**
     constructor,will gain arguments 
   */
  Corr_sofq(const utilities::Tuplef& max_q,const utilities::Tuplei& n_bins);
  ~Corr_sofq();
  
  
  void display() const;
  
  

private:
  /**
     number of bins
   */
  utilities::Tuplei n_bins_;
  /**
     maximum range
   */
  utilities::Tuplef max_range_;

  
  /**
     Array to hold the q-values
   */
  utilities::ND_Array<utilities::Tuplef,utilities::Tuplei> q_;
  
  /**
     Array to hold the values
   */
  utilities::ND_Array<utilities::Tuplef,utilities::Tuplei> s_of_q_;


  /**
     name of group for writing out to a wrapper
   */
  std::string name_;

  /**
     count of the number of particles added
   */
  int parts_added_;
  
  /**
     File name of data source
   */
  std::string fname_;
  
  /**
     Data set number
   */
  int dset_;
  /**
     Computation number
   */
  int comp_num_;

  /**
     iterate through elements of arrays
   */
  bool step_indx(utilities::Tuplei & indx) const;
  
  /**
     Rank of array
   */
  const static int rank_ =   utilities::ND_Array<utilities::Tuplef,utilities::Tuplei>::rank_;
  
  
};

}



#endif
