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
#ifndef IMAGE_BASE
#define IMAGE_BASE

#include <string>
#include "FreeImagePlus.h"

namespace utilities
{
template<class T,int length>
class Tuple;


class Md_store;
}

namespace iden
{
class MD_parser;
/**
   Enumeration of pixel sizes
*/
typedef enum PIX_TYPE
  {
    U16=0,
    U8,
    S16,
    F32,
    ERROR
  } PIX_TYPE;
  
/**
   This is an ABC for different file layouts, ie to make dealing with
   multi-part files and the spew that comes out of stream to disk easy
   to change between.  Th
 */
class Image_base
{
public:
  
  /**
     Empty constructor
   */
  Image_base()  {  };
  /**
     empty destructor
   */
  virtual ~ Image_base(){};
  
  /**
     Set the plane w
   */
  virtual void  select_plane(unsigned int plane) = 0;
  
  /**
     Returns the data of the current plane.  This pointer is managed
     internally and should not be deleted.  This pointer is only good
     until the object is closed or until the plane is changed, which
     ever comes first.  To determine how to cast this pointer call
     get_pixel_type.
   */
  virtual const void * get_plane_pixels() const = 0;

  

  /**
     Returns the current plane's meta data.  Returns a pointer that
    needs to be cleaned up.  
   */
  virtual utilities::Md_store * get_plane_md(const MD_parser & parser) const = 0;
  
  /**
     Returns the plane dimensions
   */
  virtual utilities::Tuple<unsigned int,2> get_plane_dims() const = 0;
  
  /**
     returns the plane scan_step
   */
  virtual WORD get_scan_step() const = 0;
  
  /**
     Returns the number of planes in the image
   */
  virtual int get_frame_count() const = 0;
  
  /**
     Returns the size of the image pixels
   */
  virtual PIX_TYPE get_pixel_type() const = 0;
  
};

  
}
#endif
