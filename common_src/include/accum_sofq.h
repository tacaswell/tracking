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

class Accum_sofq : public Accumulator
{
public:
  // basic inherited stuff
  void add_particle(const particle *) ;
  void out_to_wrapper(utilities::Generic_wrapper & ) const ;

  // special stuff
  
  /**
     constructor,will gain arguments 
     @param[in] q_range Pair with the min and max \f$|\vec{q}|\f$
     @param[in] q the direction \f$\hat{q}\f$ to take s(q) along,
     does not need to be a unit vector
     @param[in] n_bins the number of bins to use
  */
  Accum_sofq(const utilities::Tuple<float,2>& q_range, 
	     utilities::Tuplef q, 
	     const int n_bins);
  ~Accum_sofq();

  /**
     Returns an array with the magnitudes of the complex s(q) values 
     @param[out] out vector of \f$|s(q)|^2|\f$.  Vector will be re-sized if needed
  */
  void get_magnitude_sqr(std::vector<float>& out )const;

  /**
     Plots to screen using gnuplot
   */
  void display() const;

  /**
     Fills a vector with the q values used.  Out is cleared and
     resized as needed.
     @param[out] out vector of q values
   */
  void get_q_vec(std::vector<float> &out)const;
  

  
  int  get_n_bins() const
  {return n_bins_;}
  
  

private:
  /**
     number of bins
  */
  const unsigned int n_bins_;
  /**
     range
  */
  const utilities::Tuple<float,2> q_range_;
  
  /**
     Spacing of evaluation points in reciprocal space
   */
  const float q_step_;
  

  /**
     direction in reciprocal space to evaluate along.
   */
  const utilities::Tuplef q_;
  
  
  /**
     vector to hold the values
  */
  std::vector<std::complex<float> > s_of_q_;


  /**
     count of the number of particles added
  */
  int parts_added_;

  /**
     the units of q
   */
  std::string units_;
  
  
  /**
     \f$\i\f$
   */
  const static std::complex<float> i_;
  /**
     \f$\pi\f$
   */
  const static float pi_;
  
  
};

}



#endif
