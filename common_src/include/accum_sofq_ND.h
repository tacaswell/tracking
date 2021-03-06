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

#ifndef CORR_SOFQ_ND
#define CORR_SOFQ_ND

#include <vector>
#include <string>
#include <complex>
#include "accumulator.h"
#include "ndarray.h"



namespace tracking
{
/**
   Computes \f$s(\vec{q})\f$
*/
class Accum_sofq_ND : public Accumulator
{
public:
  // basic inherited stuff
  void add_particle(const particle *) ;
  void out_to_wrapper(utilities::Generic_wrapper &,
                      const utilities::Md_store & md_store ) const ;

  // special stuff
  
  /**
     constructor,will gain arguments 
   */
  Accum_sofq_ND(const utilities::Tuplef& max_q,const utilities::Tuplei& n_bins);
  ~Accum_sofq_ND();

  /**
     Returns an array with the magnitudes of the complex s(q) values 
   */
  void get_magnitude(utilities::ND_Array<float,utilities::Tuplei::length_>&)const;
  

  /**
     Plots the result using gnuplot
   */
  void display() const;
  
  /**
     Returns the dimensions of bins
   */
  const utilities::Tuplei & get_n_bins() const
  {return n_bins_;}
  
  

private:
  /**
     number of bins
   */
  const utilities::Tuplei n_bins_;
  /**
     maximum range
   */
  utilities::Tuplef max_range_;

  /**
     Total number of bins
   */
  int total_bins_;
  
  /**
     Array to hold the q-values
   */
  utilities::ND_Array<utilities::Tuplef,utilities::Tuplei::length_> q_;
  
  /**
     Array to hold the values
   */
  utilities::ND_Array<std::complex<float>,utilities::Tuplei::length_> s_of_q_;


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
  const static int rank_ =   utilities::ND_Array<utilities::Tuplef,utilities::Tuplei::length_>::rank_;
  
  
};

}



#endif
