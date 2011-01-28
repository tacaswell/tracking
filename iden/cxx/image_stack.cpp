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

#include "image_stack.h"

#include "FreeImagePlus.h"

#include "tuple.h"

#include "mm_md_parser.h"
#include "md_store.h"

using utilities::Image_stack;
using utilities::Md_store;
using std::endl;
using std::cout;
using std::runtime_error;



Image_stack::Image_stack(std::string fname):
  fname_(fname)
{
  
  // load multi page
  FreeImage_Initialise();
  BOOL bMemoryCache = TRUE;
  src_ = fipMultiPage(bMemoryCache);
  // Open src file (read-only, use memory cache)
  bool succes = src_.open(fname_.c_str(), FALSE, TRUE);
  if(!succes)
    throw runtime_error("image_stack::image_stack could not open file");
  
    
  
  image_ = src_.lockPage(0);
  if(!image_.isValid())
    throw runtime_error("Image_stack::Image_stack the plane is not valid");
  
  
}

Image_stack::~Image_stack()
{
  src_.unlockPage(image_,false);
  
  src_.close(0);
  
  FreeImage_DeInitialise();
}


void Image_stack::select_plane(unsigned int plane)
{
  src_.unlockPage(image_,false);

  // add some sanity checks
  cur_plane_ = plane;
  image_ = src_.lockPage(cur_plane_);
  if(!image_.isValid())
    throw runtime_error("Image_stack::select_plane the plane is not valid");
  
}

const WORD * Image_stack::get_plane_pixels() const
{
  return (WORD *) image_.accessPixels();
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
  int rows = image_.getHeight();
  int cols = image_.getWidth();
  tmp[1] = rows;
  tmp[0] = cols;
  return tmp;
}

WORD Image_stack::get_scan_step() const
{
  return image_.getScanWidth();
}


int Image_stack::get_frame_count() const
{
  return src_.getPageCount();
}

utilities::PIX_SIZE Image_stack::get_pixel_size() const
{
  unsigned pix_size = image_.getBitsPerPixel();
  if(pix_size == 16)
    return utilities::U16;
  if(pix_size == 8)
    return utilities::U8;
  return utilities::ERROR;
  
}
