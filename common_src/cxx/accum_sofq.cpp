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
using utilities::ND_Array;

using tracking::Accum_sofq;

using std::exp;
using std::complex;
using std::abs;


void Accum_sofq::add_particle(const particle * p_in) 
{
  //Tuplef pos = p_in->get_position();
  Tuplef pos(0,1);
  
  complex<float> i (0,1);
  float pi = 4.0*atan(1.0);
  
  
  Tuplei indx;
  do
  {
    s_of_q_(indx) = exp(2*pi*i*(q_(indx).dot(pos)));
  }
  while(step_indx(indx));

  
  return;
  
}

void Accum_sofq::out_to_wrapper(utilities::Generic_wrapper & ) const 
{
  return;
  
}

Accum_sofq::Accum_sofq(const utilities::Tuplef& max_q,const utilities::Tuplei & n_bins):
  n_bins_(n_bins),max_range_(max_q),q_(n_bins_),s_of_q_(n_bins_)
{
  Tuplei indx;
  do
  {
    q_(indx) = max_range_*(Tuplef(indx+Tuplei(1))/Tuplef(n_bins_));
  }
  while(step_indx(indx));
}

Accum_sofq::~Accum_sofq()
{
  

}

  
void Accum_sofq::display() const
{
  cout<<"q array"<<endl;
  q_.print();
  cout<<"----------"<<endl;
  cout<<"s(q) array"<<endl;
  ND_Array<float,Tuplei> abs_array(n_bins_);
  get_magnitude(abs_array);
  s_of_q_.print();
  abs_array.print();
  
  

  // Gnuplot g;
  // cout << "displaying image data" << endl;
  // const int iWidth  = width_;
  // const int iHeight = height_;
  // //  const int iWidth  = 500;
  // //  const int iHeight = 200;
  
  // g.set_xrange(0,iWidth).set_yrange(0,iHeight).set_cbrange(0,255);

  // g.cmd("set palette gray");
  // unsigned char ucPicBuf[iWidth*iHeight];
  // // generate a greyscale image
  // Ipp32f max = -1;
  // ippiMax_32f_C1R(imagedata_,stepsize_,ROIfull_,&max);
  // cout<<"Max: "<<max<<endl;
  // for(int oIndex = 0; oIndex < iHeight; oIndex++)
  // {
  //   for(int iIndex = 0;iIndex<iWidth;++iIndex)
  //   {
  //     *(ucPicBuf + iIndex +iWidth*oIndex) =
  // 	(unsigned char)(((*(imagedata_ +iIndex +stepsize_/sizeof(Ipp32f) * oIndex))/max)*255);
  //   }
  // }
  // g.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
  // wait_for_key();
  // g.remove_tmpfiles();
  



}

void Accum_sofq::get_magnitude(utilities::ND_Array<float,Tuplei>& out)const
{
  // make sure input and storage array are same size

  Tuplei indx;
  do
  {
    out(indx) = abs(s_of_q_(indx));
  }
  while(step_indx(indx));  

}


bool Accum_sofq::step_indx(utilities::Tuplei & indx) const
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
