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

#ifndef IMAGE_STACK
#define IMAGE_STACK

#include "image_base.h"
#include "mm_md_parser.h"
namespace utilities
{

class Image_stack:public Image_base
{
public:
  Image_stack(std::string fname);
  ~Image_stack();
  void select_plane(unsigned int plane);
  const void * get_plane_pixels() const;
  Md_store * get_plane_md(const MD_parser & parser) const;
  Tuple<unsigned int,2> get_plane_dims()const;
  WORD get_scan_step() const;
  int get_frame_count() const;
  PIX_TYPE get_pixel_type() const;
  
  /**
     Initialization function

     Opens up the file and checks the data type
   */
  void initialize();
  /**
     Cleans up the file, but does not take the FreeImage down
   */
  void deinitialize();
  

private:
  std::string fname_;
  unsigned int cur_plane_;
  
  bool stack_open_;
  
  fipMultiPage src_;
  
  fipImage image_;
  

  bool convert_to_grey_;

  PIX_TYPE pix_type_;
  
  /**
     Converts the image to 8 bit grey scale
   */
  bool to_grey();
  
  
};

  
  
  
  
}
#endif
