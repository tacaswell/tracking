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


#include "gnuplot_i.hpp"

using gnuplot::Gnuplot;
using gnuplot::GnuplotException;
using gnuplot::wait_for_key;

using std::cout;
using std::endl;


using utilities::Tuplef;
using utilities::Tuplei;


using tracking::Accum_sofq;

using std::exp;
using std::complex;
using std::abs;
using std::vector;


const std::complex<float> Accum_sofq::i_ = complex<float>(0,1);
const float Accum_sofq::pi_ = 4.0*atan(1.0);

Accum_sofq::Accum_sofq(const utilities::Tuple<float,2>& q_range, utilities::Tuplef q, const int n_bins):
  n_bins_(n_bins),
  q_range_(q_range),
  q_step_((q_range[1]-q_range[0])/(n_bins_-1)),
  q_(q),
  s_of_q_(n_bins_)
{
  
  cout<<"made accum_sofq!!"<<endl;
  
  
}


void Accum_sofq::add_particle(const particle * p_in) 
{
  float arg =(p_in->get_position()).dot(q_);
  
  float q_tmp = q_range_[0];
  for(unsigned int j = 0; j<n_bins_;++j)
  {
    s_of_q_[j] += exp(2*pi_*i_*arg*(q_tmp))/q_tmp;
    q_tmp +=q_step_;
    
  }
  
  

  
  return;
  
}

void Accum_sofq::out_to_wrapper(utilities::Generic_wrapper & ) const 
{
  return;
  
}

Accum_sofq::~Accum_sofq()
{
  

}

  
void Accum_sofq::display() const
{
  vector<float>tmp(n_bins_) ;
  get_magnitude_sqr(tmp);
  vector<float>q_tmp(n_bins_) ;
  float q_sum = q_range_[0];
  for(unsigned int j = 0;j<n_bins_;++j)
  {
    q_tmp[j] = q_sum;
    q_sum += q_step_;
  }
  
  
    
  Gnuplot g(q_tmp,tmp,"g(r)","steps");
  g.set_grid();
  wait_for_key();



}

void Accum_sofq::get_magnitude_sqr(vector<float>& out)const
{
  // make sure input and storage array are same size
  out.clear();
  if(out.size() != n_bins_)
    out.resize(n_bins_);
  
  for(unsigned int j = 0;j<n_bins_;++j)
  {
    complex<float> tmp_s = s_of_q_[j];
    
    out[j] = tmp_s.real()*tmp_s.real() + tmp_s.imag()*tmp_s.imag();
  }
  
}

