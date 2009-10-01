//Peter J. Lu
//Copyright 2008 Peter J. Lu. 
//http://www.peterlu.org
//plu@fas.harvard.edu
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
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work. 
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of freeimage-license, the
//licensors of this Program grant you additional permission to convey
//the resulting work.  Corresponding Source for a non-source form of
//such a combination shall include the source code for the parts of
//FreeImage used as well as that of the covered work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

//copied form https://plutarc.svn.sourceforge.net/svnroot/plutarc/trunk/matlab_wrapper rev9
// Modified by Thomas A Caswell tcaswell@uchicago.edu 09/2009-
#include "ipp.h"
#include "mex.h"

#include <fstream>
#include <iostream>
#include <math.h>
// tac 2009-09-15
// 
#include "iden/image1.h"

// tac 2009-09-15
// 
using iden::Image2D;
using std::cout;
using std::endl;

// tac 2009-09-21
// added displaying functionality
#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot


/* modified 6/08
 * Beating this code in to a form that can be compiled in to mex files
 * tcaswell
 */

Image2D::Image2D(const int image_length, const int image_width):
  imagedata_(NULL), stepsize_(0), width_(0), height_(0)
{
  numberofpixels_ = image_width * image_length;
  height_ = image_length;
  width_ = image_width;
  imagedata_ = ippsMalloc_32f(numberofpixels_);
  stepsize_ = 4 * image_width;
  ROIfull_.width = image_width;
  ROIfull_.height = image_length;
  // tac 2009-03-02 
  // initialize the arrays to 0 to prevent junk in
  // them from around the edges from screwing with the
  // recentering processes
  // for(int j = 0;j<image_width*image_length; j++)
  //     *(imagedata + j) = 0;
  // tac 2009-09-17
  // changed to use ipp methods
  
  ippiSet_32f_C1R(0,imagedata_,stepsize_,ROIfull_);
  
  
}



Image2D::~Image2D()
{
  ippsFree(imagedata_);
  imagedata_ = NULL;
}

int Image2D::getx(const int index1D)
{
  return index1D % width_;
}

int Image2D::gety(const int index1D)
{
  return (int) floor((float) index1D / width_);
}

int Image2D::get1Dindex(const int x, const int y)
{
  return y * width_ + x;
}


void Image2D::set_data(const WORD * data_in, int rows, int cols,WORD in_step)
{
  if(!(cols==width_ && rows == height_))
  {
    std::cerr<<"height: "<<height_<<'\t'<<"rows: "<<rows<<std::endl;
    std::cerr<<"width: "<<width_<<'\t'<<"cols: "<<cols<<std::endl;
    throw "Image2D: data is wrong size";
  }
  

  int data_step = cols*sizeof(Ipp32f);
  

  // this make the assumption that WORD is a short is an unsigned
  // short and an Ipp16u is also an unsigned short, or atleast that
  // they are the same thing.  This is true on linux


  // copy data to this object
  ippiConvert_16u32f_C1R(data_in, in_step,imagedata_,data_step,ROIfull_);
  
}



void wait_for_key();
 
void Image2D::display_image() const
{
  Gnuplot g;
  cout << "displaying image data" << endl;
   const int iWidth  = width_;
   const int iHeight = height_;
//  const int iWidth  = 500;
//  const int iHeight = 200;
  
  g.set_xrange(0,iWidth).set_yrange(0,iHeight).set_cbrange(0,255);

  g.cmd("set palette gray");
  unsigned char ucPicBuf[iWidth*iHeight];
  // generate a greyscale image
  Ipp32f max = -1;
  ippiMax_32f_C1R(imagedata_,stepsize_,ROIfull_,&max);
  cout<<"Max: "<<max<<endl;
  for(int oIndex = 0; oIndex < iHeight; oIndex++)
  {
    for(int iIndex = 0;iIndex<iWidth;++iIndex)
    {
      *(ucPicBuf + iIndex +iWidth*oIndex) =
	(unsigned char)(((*(imagedata_ +iIndex +stepsize_/sizeof(Ipp32f) * oIndex))/max)*255);
    }
  }
  g.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
  wait_for_key();
  g.remove_tmpfiles();
  
}


void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
  cout << endl << "Press any key to continue..." << endl;

  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
  _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
  cout << endl << "Press ENTER to continue..." << endl;

  std::cin.clear();
  std::cin.ignore(std::cin.rdbuf()->in_avail());
  std::cin.get();
#endif
  return;
}
