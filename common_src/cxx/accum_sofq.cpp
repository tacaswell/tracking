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

#include "generic_wrapper.h"
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

using utilities::Generic_wrapper;


const std::complex<float> Accum_sofq::i_ = complex<float>(0,1);
const float Accum_sofq::pi_ = 4.0*atan(1.0);

Accum_sofq::Accum_sofq(const utilities::Tuple<float,2>& q_range, 
		       const utilities::Tuplef q, 
		       const int n_bins):
  n_bins_(n_bins),
  q_range_(q_range),
  q_step_((q_range[1]-q_range[0])/(n_bins_-1)),
  q_(q.direction()),
  s_of_q_(n_bins_),
  parts_added_(0)
{
  
  
}


void Accum_sofq::add_particle(const particle * p_in) 
{
  float arg =(p_in->get_position()).dot(q_);
  
  float q_tmp = q_range_[0];
  for(unsigned int j = 0; j<n_bins_;++j)
  {
    s_of_q_[j] += exp(2*pi_*i_*arg*(q_tmp));
    q_tmp +=q_step_;
    
  }
  ++parts_added_;
  
  

  
  return;
  
}

void Accum_sofq::out_to_wrapper(utilities::Generic_wrapper & wrap) const 
{

  // get values to put in to wrapper
  vector<float>tmp(n_bins_,0) ;
  get_magnitude_sqr(tmp);
  
  vector<float>q_tmp(n_bins_) ;
  get_q_vec(q_tmp);
  

  
  const float * data_ptr = NULL;
  
  unsigned nbins = n_bins_;
  
  
  data_ptr = &tmp.front();
  wrap.add_dset(1,&nbins,utilities::V_FLOAT,data_ptr,"sofq");

  data_ptr = &q_tmp.front();
  wrap.add_dset(1,&nbins,utilities::V_FLOAT,data_ptr,"q_vec");

  // logistic information
  // wrap.add_meta_data("comp_num",comp_num_);
  // wrap.add_meta_data("dset",dset_);
  // wrap.add_meta_data("data_file",fname_);
  
  
  wrap.add_meta_data("q0",q_);

  //wrap.add_meta_data("units",units_,"q_vec");
  





  
}

Accum_sofq::~Accum_sofq()
{
  

}


void Accum_sofq::get_q_vec(vector<float> &out)const
{
  out.clear();
  out.resize(n_bins_);
  float q_sum = q_range_[0];
  for(unsigned int j = 0;j<n_bins_;++j)
  {
    out[j] = q_sum;
    q_sum += q_step_;
  }
  
}

  
void Accum_sofq::display() const
{

  cout<<"parts added: "<<parts_added_<<endl;
  
  vector<float>tmp(n_bins_,0) ;
  get_magnitude_sqr(tmp);
  vector<float>q_tmp(n_bins_) ;
  get_q_vec(q_tmp);
  
  
  
    
  Gnuplot g(q_tmp,tmp,"s(q)","steps");
  g.set_grid();
  g.replot();
  
  wait_for_key();



}

void Accum_sofq::get_magnitude_sqr(vector<float>& out)const
{
  // make sure input and storage array are same size
  out.clear();
  if(out.size() != n_bins_)
    out.resize(n_bins_);

  float prts_sq = (float)parts_added_*(float)parts_added_;
  

  for(unsigned int j = 0;j<n_bins_;++j)
  {
    complex<float> tmp_s = s_of_q_[j];
    
    out[j] = (tmp_s.real()*tmp_s.real() + tmp_s.imag()*tmp_s.imag())/prts_sq;
    
  }
  
}

