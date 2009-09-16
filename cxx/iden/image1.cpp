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
// Modified by Thomas Caswell tcaswell@uchicago.edu 09/2009-
#include "ipp.h"
#include "mex.h"

#include <fstream>
#include <math.h>
// tac 2009-09-15
// 
#include "iden/image1.h"

// tac 2009-09-15
// 
using iden::Image2D;


/* modified 6/08
 * Beating this code in to a form that can be compiled in to mex files
 * tcaswell
 */

Image2D::Image2D(const int image_length, const int image_width):
  imagedata(NULL), stepsize(0), width(0), length(0)
{
  numberofpixels = image_width * image_length;
  length = image_length;
  width = image_width;
  imagedata = ippsMalloc_32f(numberofpixels);
  stepsize = 4 * image_width;
  ROIfull.width = image_width;
  ROIfull.height = image_length;
  // tac 2009-03-02 
  // initialize the arrays to 0 to prevent junk in
  // them from around the edges from screwing with the
  // recentering processes
  for(int j = 0;j<image_width*image_length; j++)
    *(imagedata + j) = 0;
  
}

Image2D::~Image2D()
{
  ippsFree(imagedata);
  imagedata = NULL;
}

int Image2D::getx(const int index1D)
{
  return index1D % width;
}

int Image2D::gety(const int index1D)
{
  return (int) floor((float) index1D / width);
}

int Image2D::get1Dindex(const int x, const int y)
{
  return y * width + x;
}


/*Image2D TIFF_to_IPP(FIBITMAP *dib_in)*/
Image2D iden::mat_to_IPP(const mxArray *data)
{
  int j;
  double *mat_tmp;
  mat_tmp = mxGetPr(data);
  IppStatus status;

  int width = mxGetM(data);
  int height = mxGetN(data);
  

  IppiSize roi = {width, height};
  
  Image2D image_in(height, width);
  
  Ipp32f *image_tmp;
  image_tmp = image_in.get_image2D();
  for(j=0;j<height*width;j++)
    {
      image_tmp[j] = mat_tmp[j];
    } 

  return image_in;
}

IppStatus iden::IPP_to_mat(mxArray *data, Image2D &image_out)
{
  int j;
  double *mat_tmp;
  mat_tmp = mxGetPr(data);
  IppStatus status;
  Ipp32f *image_tmp;
  image_tmp = image_out.get_image2D();

  for (j = 0; j <image_out.get_width()*image_out.get_length() ; j++)
    {
      mat_tmp[j] = (double)image_tmp[j];
    }



  return status;
}
