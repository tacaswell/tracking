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

#include "mm_md_parser.h"
#include "md_store.h"



using utilities::Image_series;
using utilities::Md_store;
using utilities::format_name;

using std::endl;
using std::cout;
using std::runtime_error;
using std::string;


Image_series::Image_series():padding_(0)
{}

const char * selector_name = NULL;

int selector(const struct dirent * d)
{
  const char* name = (d->d_name);
  if(name[0] == '.')
    return 0;
  // add check to make sure that the name matches the base name, at
  // this point I am going to trust that data was collected in a clan
  // way
  
  return 1;
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
  
  
  
  
  
  // make sure that they are there
  // eventually 
  
  // load multi page
  FreeImage_Initialise();
  BOOL bMemoryCache = TRUE;
  src_ = fipMultiPage(bMemoryCache);
  // Open src file (read-only, use memory cache)
  src_.open(format_name(dirname_,basename_,padding_,1).c_str(), FALSE, TRUE);

  image_ = src_.lockPage(0);



  return true;
  
}

void Image_series::select_plane(unsigned int plane)
{



  // clean up old file
  src_.unlockPage(image_,false);
  src_.close(0);
  
  // add some sanity checks
  if(!(plane < planecount_))
    throw runtime_error("trying to acces a plane that does not exist");
  
  cur_plane_ = plane;

  // to deal with the fact that the naming from mm starts at 1, not 0
  plane = cur_plane_ + 1;
  
  // open the next file
  BOOL bMemoryCache = TRUE;
  src_ = fipMultiPage(bMemoryCache);
  // Open src file (read-only, use memory cache)
  
  bool opened = src_.open(format_name(dirname_,basename_,padding_,plane).c_str(), FALSE, TRUE); 
  if(!opened)
    throw(runtime_error("failed to open file: " + format_name(dirname_,basename_,padding_,plane)));
  
  
  
  
  image_ = src_.lockPage(0);
  bool isvalid = image_.isValid();
  if(!isvalid)
  {
    throw(runtime_error("could not open the image plane of " 
			+ format_name(dirname_,basename_,padding_,plane)));
    
			
  }
  

}

const WORD * Image_series::get_plane_pixels() const
{
  return (WORD *) image_.accessPixels();
}

Md_store * Image_series::get_plane_md() const
{
  return mm_md_p_.parse_md(image_);
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
  
  src_.unlockPage(image_,false);
  src_.close(0);
  
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


utilities::PIX_SIZE Image_series::get_pixel_size() const
{
  unsigned pix_size = image_.getBitsPerPixel();
  if(pix_size == 16)
    return utilities::U16;
  if(pix_size == 8)
    return utilities::U8;
  return utilities::ERROR;
  
}
