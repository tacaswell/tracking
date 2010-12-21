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

#include "image_series.h"

#include "FreeImagePlus.h"

#include "tuple.h"

#include "mm_md_parser.h"
#include "md_store.h"

using utilities::Image_series;
using utilities::Md_store;
using std::endl;
using std::cout;
using std::runtime_error;


Image_series::Image_series(const std::string& fname,int padding)
:base_name_(fname),padding_(padding)
{
  // load multi page
  FreeImage_Initialise();
  BOOL bMemoryCache = TRUE;
  src_ = fipMultiPage(bMemoryCache);
  // Open src file (read-only, use memory cache)
  src_.open(format_name(fname,1,padding_).c_str(), FALSE, TRUE);

  image_ = src_.lockPage(0);
  
  
}


void Image_series::select_plane(unsigned int plane)
{



  // clean up old file
  src_.unlockPage(image_,false);
  src_.close(0);
  
  // add some sanity checks
  cur_plane_ = plane;


  // open the next file
  BOOL bMemoryCache = TRUE;
  src_ = fipMultiPage(bMemoryCache);
  // Open src file (read-only, use memory cache)
  bool opened = src_.open(format_name(base_name_,cur_plane_,padding_).c_str(), FALSE, TRUE);
  if(!opened)
      throw(runtime_error("failed to open file: " + format_name(base_name_,cur_plane_,padding_)));
  
  
  
  
  image_ = src_.lockPage(0);
  bool isvalid = image_.isValid();
  if(!isvalid)
  {
    throw(runtime_error("could not open the image plane of " 
			+ format_name(base_name_,cur_plane_,padding_)));
  }
  

}

const WORD * Image_series::get_plane_pixels()
{
  return (WORD *) image_.accessPixels();
}

Md_store * Image_series::get_plane_md()
{

  
  

  return mm_md_p_.parse_md(image_);

}

utilities::Tuple<unsigned int,2> Image_series::get_plane_dims()
{
  Tuple<unsigned int,2> tmp;
  int rows = image_.getHeight();
  int cols = image_.getWidth();
  tmp[1] = rows;
  tmp[0] = cols;
  return tmp;
}

WORD Image_series::get_plate_scan_step()
{
  return image_.getScanWidth();
}



Image_series::~Image_series()
{
  
  src_.unlockPage(image_,false);
  src_.close(0);
  
  FreeImage_DeInitialise();

  
}


std::string utilities::format_name(const std::string & base_name,unsigned int plane,int padding)
{

  
  std::ostringstream o;
  o.width(padding);
  o.fill('0');
  o<<std::right<<plane;
  return  base_name +  o.str() + ".tif";
 

}
