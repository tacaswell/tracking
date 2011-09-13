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
// change this to boost eventually 
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <string>
#include "image_series.h"

#include "FreeImagePlus.h"

#include "tuple.h"

#include "md_parser.h"
#include "md_store.h"



using utilities::Image_series;
using utilities::Md_store;
using utilities::format_name;

using utilities::PIX_TYPE;
using std::invalid_argument;

using std::endl;
using std::cout;
using std::runtime_error;
using std::string;


Image_series::Image_series():
  cur_plane_(-1),
  image_open_(false),
  padding_(0),
  planecount_(0),
  pix_type_(ERROR)
  
{}

const char * selector_name = NULL;

int selector(const struct dirent * d)
{
  const char* name = (d->d_name);
  // if is the . or .. entry, barf
  if(name[0] == '.')
    return 0;

  int test_len = strlen(name);
  int base_len = strlen(selector_name);
  // if the test filename is shorter than the base name, barf
  if(test_len<base_len)
    return 0;
  
  // if the test string does not begin with the base name, barf
  if(memcmp(name,selector_name,base_len))
    return 0;
  

  // make sure that the string contains tif or TIF
  if(strstr(name,"tif") || strstr(name,"TIF"))
    return 1;
  
  
  // return zero of not a tiff image
  return 0;
  
  
}

bool Image_series::init(const string & base_name )
{
  // strip the path off of the base_name
  
  const char * tmp;
  int slen = (base_name.length()+1);
  
  char * base_name_temp = new char[slen*sizeof(char)];
  
  memcpy(base_name_temp,base_name.c_str(),slen);
  tmp = dirname(base_name_temp);
  dirname_ = string(tmp);
  
  memcpy(base_name_temp,base_name.c_str(),slen);
  tmp = basename(base_name_temp);
  basename_ = string(tmp);
  
  delete [] base_name_temp;
  base_name_temp = NULL;
  

  
  
  // sort out how many there are
  selector_name = basename_.c_str();
  
  
  dirent ** namelist;
  planecount_ = scandir(dirname_.c_str(),&namelist,selector,alphasort);
  
  

  for(unsigned int j = 0; j<planecount_;++j)
    free (namelist[j]);
  free(namelist);
  // sort out padding
  padding_ = 1;
  int div = 10;
  while(planecount_/div)
    ++padding_,div*=10;
  
  
  
  
  select_plane(0);
  

  return true;
  
}

void Image_series::select_plane(unsigned int plane)
{

  
  // add some sanity checks
  if(!(plane < planecount_))
    throw runtime_error("trying to acces a plane that does not exist");
  
  cur_plane_ = plane;

  // nuke the old data
  image_.clear();
  
  // to deal with the fact that the naming from mm starts at 1, not 0
  int read_plane = cur_plane_ + 1;
 
  // open new file
  bool suc = false;
  suc = image_.load(format_name(dirname_,basename_,padding_,read_plane).c_str());
  if(!suc)
    throw(runtime_error("failed to open file: " + format_name(dirname_,basename_,padding_,read_plane)));
  
  
  FREE_IMAGE_TYPE img_type = image_.getImageType();
  
  
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
    if(!image_.isGrayscale())
      to_grey();
    
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
  
  
  
}

const void * Image_series::get_plane_pixels() const
{
  return (void *) image_.accessPixels();
}

Md_store * Image_series::get_plane_md(const MD_parser & parser) const
{
  return parser.parse_md(image_);
}

utilities::Tuple<unsigned int,2> Image_series::get_plane_dims() const
{
  Tuple<unsigned int,2> tmp;
  int rows = image_.getHeight();
  int cols = image_.getWidth();
  tmp[1] = rows;
  tmp[0] = cols;
  return tmp;
}

WORD Image_series::get_scan_step() const
{
  return image_.getScanWidth();
}

int Image_series::get_frame_count() const
{
  return planecount_;
  
}


Image_series::~Image_series()
{
  image_.clear();
  
  FreeImage_DeInitialise();

  
}

std::string utilities::format_name(const std::string & dirname,
			const std::string & basename,
			int padding,
			unsigned int plane)

{
  
  std::ostringstream o;
  o.width(padding);
  o.fill('0');
  o<<std::right<<plane;
  return  dirname + '/' + basename +  o.str() + ".tif";
  
}


utilities::PIX_TYPE Image_series::get_pixel_type() const
{
  unsigned pix_size = image_.getBitsPerPixel();
  if(pix_size == 16)
    return utilities::U16;
  if(pix_size == 8)
    return utilities::U8;
  return utilities::ERROR;
  
}

bool Image_series::to_grey()
{
  return image_.convertToGrayscale();
}
