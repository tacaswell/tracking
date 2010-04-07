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


#include "accum_sofq_ND.h"
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
using utilities::ND_Array;

using tracking::Accum_sofq_ND;

using std::exp;
using std::complex;
using std::abs;


void Accum_sofq_ND::add_particle(const particle * p_in) 
{
  Tuplef pos = p_in->get_position();
  
  
  complex<float> i (0,1);
  float pi = 4.0*atan(1.0);
  
  
  
  for(int indx = 0; indx<total_bins_;++indx)
  {

    s_of_q_(indx) += exp(2*pi*i*(q_(indx).dot(pos)));
    // cout<<pos<<'.'<<q_(indx)<<endl;
    // cout<<q_(indx).dot(pos)<<endl;
    // cout<<exp(2*pi*i*(q_(indx).dot(pos)))<<endl;
    // cout<<"---------------"<<endl;
  }
  

  
  return;
  
}

void Accum_sofq_ND::out_to_wrapper(utilities::Generic_wrapper & ) const 
{
  return;
  
}

Accum_sofq_ND::Accum_sofq_ND(const utilities::Tuplef& max_q,const utilities::Tuplei & n_bins):
  n_bins_(n_bins),
  max_range_(max_q),
  total_bins_(n_bins.prod()),
  q_(n_bins_),
  s_of_q_(n_bins_)
{
  Tuplei indx;
  do
  {
    q_(indx) = max_range_*(Tuplef(indx+Tuplei(1))/Tuplef(n_bins_));
  }
  while(step_indx(indx));
  
}

Accum_sofq_ND::~Accum_sofq_ND()
{
  

}

  
void Accum_sofq_ND::display() const
{
  // cout<<"q array"<<endl;
  // q_.print();
  // cout<<"----------"<<endl;
  // cout<<"s(q) array"<<endl;
  ND_Array<float,Tuplei::length_> abs_array(n_bins_);
  get_magnitude(abs_array);
  // s_of_q_.print();
  // abs_array.print();
  
  
  if(rank_ == 2)
  {
    
    Gnuplot g;
    cout << "displaying s(q) data" << endl;
    const int iWidth  = n_bins_[0];
    const int iHeight = n_bins_[1];
    g.set_xrange(0,iWidth).set_yrange(0,iHeight).set_cbrange(0,255);

    g.cmd("set palette gray");
    unsigned char ucPicBuf[iWidth*iHeight];
    // generate a greyscale image
    float max = -1;
    Tuplei indx;
    do
    {
      if(abs_array(indx) > max)
	max = abs_array(indx);
    }
    while(step_indx(indx));
    
    for(int oIndex = 0; oIndex < iHeight; oIndex++)
    {
      for(int iIndex = 0;iIndex<iWidth;++iIndex)
      {
	*(ucPicBuf + iIndex +iWidth*oIndex) =
	  (unsigned char)(255*(abs_array(Tuplei(iIndex,oIndex))/max));
      }
    }
    g.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
    wait_for_key();
    g.remove_tmpfiles();
  }
  



}

void Accum_sofq_ND::get_magnitude(utilities::ND_Array<float,Tuplei::length_>& out)const
{
  // make sure input and storage array are same size

  Tuplei indx;
  do
  {
    out(indx) = abs(s_of_q_(indx));
  }
  while(step_indx(indx));  

}


bool Accum_sofq_ND::step_indx(utilities::Tuplei & indx) const
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
