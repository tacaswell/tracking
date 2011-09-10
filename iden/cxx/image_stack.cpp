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
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
#include <iostream>
#include <stdexcept>

#include "image_stack.h"

#include "FreeImagePlus.h"

#include "tuple.h"

#include "mm_md_parser.h"
#include "md_store.h"

using utilities::Image_stack;
using utilities::PIX_TYPE;

using utilities::Md_store;
using std::endl;
using std::cout;
using std::runtime_error;

using std::invalid_argument;



Image_stack::Image_stack(std::string fname):
  fname_(fname),cur_plane_(-1),stack_open_(false),convert_to_grey_(false),
  pix_type_(ERROR)
{
  FreeImage_Initialise();
}
void Image_stack::initialize()
{
  
  // load multi page
  
  BOOL bMemoryCache = TRUE;
  src_ = fipMultiPage(bMemoryCache);
  // Open src file (read-only, use memory cache)
  bool succes = src_.open(fname_.c_str(), FALSE, TRUE);
  if(!succes)
    throw runtime_error("image_stack::image_stack could not open file");
  
    
  cur_plane_ = 0;
  // extract the plane we want
  fipImage tmp_img;
  tmp_img = src_.lockPage(cur_plane_);
  
  
  if(!tmp_img.isValid())
    throw runtime_error("Image_stack::Image_stack the plane is not valid");
  
  FREE_IMAGE_TYPE img_type = tmp_img.getImageType();
  
  
  // check image type
  switch(img_type)
  {
  case FIT_UINT16:	// array of unsigned short	: unsigned 16-bit
    // trust the id, assume in really 16u, go on
    pix_type_ = U16;
    break;
  case FIT_BITMAP:	// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit  
    // if the image is a bitmap and is not a grey scale, set flag to
    //  convert the image to a grey scale image when loading
    if(!tmp_img.isGrayscale())
      convert_to_grey_ = true;
    pix_type_ = U8;
    break;
  case FIT_INT16:	// array of short			: signed 16-bit
    // trust the id, assume in really 16s, go on
    pix_type_ = S16;
    break;
  case FIT_FLOAT:	// array of float			: 32-bit IEEE floating point
    // trust the id, assume in really 32f, go on
    pix_type_ = F32;
    break;
  case FIT_RGB16:	// 48-bit RGB image			: 3 x 16-bit
  case FIT_RGBF:	// 96-bit RGB float image	: 3 x 32-bit IEEE floating point
    throw invalid_argument("This program can't deal with colors");
    break;
  case FIT_UNKNOWN:	// unknown type
    throw runtime_error("Free image can not sort out what kind of image this is!");
    break;
  case FIT_COMPLEX:	// array of FICOMPLEX		: 2 x 64-bit IEEE floating point
    throw invalid_argument("This program can't deal with complex images");
    break;
  case FIT_RGBA16:	// 64-bit RGBA image		: 4 x 16-bit
  case FIT_RGBAF:	// 128-bit RGBA float image	: 4 x 32-bit IEEE floating point
    throw invalid_argument("This program can't deal with an alpha channel or colors");
    break;
  case FIT_UINT32:	// array of unsigned long	: unsigned 32-bit
  case FIT_INT32:	// array of long			: signed 32-bit
  case FIT_DOUBLE:	// array of double			: 64-bit IEEE floating point
    throw invalid_argument("This program can't deal with this type of image, excessive bit depth");
    break;
  }
  
  image_ = FreeImage_Clone( tmp_img );
  // return the frame to the multipage
  src_.unlockPage(tmp_img,false);
  if(convert_to_grey_)
    to_grey();

  
}
void Image_stack::to_grey()
{
  FIBITMAP* tmp = FreeImage_ConvertToGreyscale(image_);
  FreeImage_Unload(image_);
  image_ = tmp;
  
}

void Image_stack::deinitialize()
{

  if(src_.isValid())
  {
    if(image_)
      FreeImage_Unload(image_);
    
      
    
    
    int count = 0;
    src_.getLockedPageNumbers(NULL,&count);
    
    bool closed;
    
    if(    src_.isValid())
      closed =  src_.close(0);
  
    if(!closed)
      cout<<"failed to properly close"<<endl;
    
  }
  
  
}


Image_stack::~Image_stack()
{
  deinitialize();
  
  
  
  //FreeImage_DeInitialise();
}


void Image_stack::select_plane(unsigned int plane)
{
  
  // if we are on our current plane, don't bother to change anything
  if(plane == cur_plane_)
    return;
  if(image_)
    FreeImage_Unload(image_);
  
  // extract the plane we want
  fipImage tmp_img;
  tmp_img = src_.lockPage(cur_plane_);
  // make a clone of the plane
  image_ = FreeImage_Clone(tmp_img);
  // return the frame to the multipage
  src_.unlockPage(tmp_img,false);
  
  if(convert_to_grey_)
    to_grey();
  
}

const void * Image_stack::get_plane_pixels() const
{
  
  return (void *) FreeImage_GetBits(image_);
  
}

Md_store * Image_stack::get_plane_md()const 
{
  // returns an empty md_store with out calling to parser at all
  
  // the right way to deal with this is to have a pointer to a parser
  // passed in at construction

  //return mm_md_p_.parse_md(image_);
  return new Md_store();
  

}

utilities::Tuple<unsigned int,2> Image_stack::get_plane_dims()const
{
  Tuple<unsigned int,2> tmp;
  int rows = FreeImage_GetHeight(image_);
  int cols = FreeImage_GetWidth(image_);;
  tmp[1] = rows;
  tmp[0] = cols;
  return tmp;
}

WORD Image_stack::get_scan_step() const
{
  
  return FreeImage_GetPitch(image_);
}


int Image_stack::get_frame_count() const
{
  return src_.getPageCount();
}

PIX_TYPE Image_stack::get_pixel_type() const
{
  return pix_type_;
  
}
