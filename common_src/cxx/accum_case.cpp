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

#include "accum_case.h"
#include "generic_wrapper.h"
#include "accum_sofq.h"
#include "gnuplot_i.hpp"

using gnuplot::Gnuplot;
using gnuplot::GnuplotException;
using gnuplot::wait_for_key;

using std::vector;
using utilities::Accum_case;


using tracking::Accum_sofq;


void Accum_case::average(vector<float> & s_sum,tracking::Accum_sofq*) const
{
  int frame_count = accum_vec_.size();
  int nbins = dynamic_cast<Accum_sofq*>(accum_vec_.at(0))->get_n_bins();
  
  s_sum.clear();
  s_sum.resize(nbins);
  vector<float>s_tmp(nbins);
  for(int j = 0;j<frame_count;++j)
  {
    dynamic_cast<Accum_sofq*>(accum_vec_[j])->get_magnitude_sqr(s_tmp);
    for(int k = 0;k<nbins;++k)
      s_sum[k] += s_tmp[k];
  }
  

}

void Accum_case::display(tracking::Accum_sofq*) const
{
  int nbins = dynamic_cast<Accum_sofq*>(accum_vec_.at(0))->get_n_bins();
  vector<float> sum(nbins);
  vector<float> q(nbins);
  average(sum,(Accum_sofq*)NULL);
  dynamic_cast<Accum_sofq*>(accum_vec_[0])->get_q_vec(q);
  Gnuplot g(q,sum,"s(q)","|q|");
  g.set_grid();
  wait_for_key();
}


Accum_case::~Accum_case(){
  for(unsigned int j = 0; j<accum_vec_.size();j++)
  {
    delete accum_vec_[j];
    accum_vec_[j] = NULL;
  }
}

Accum_case::Accum_case(const tracking::Accum_sofq*,
		       int frame_count, 
		       const utilities::Tuple<float,2>& q_range, 
		       utilities::Tuplef q, 
		       const int n_bins):
  accum_vec_(frame_count)
{
  
  for(int j = 0; j<frame_count;j++)
    accum_vec_[j] = new Accum_sofq(q_range,q,n_bins);
  

}
