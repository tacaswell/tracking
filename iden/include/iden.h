//Copyright 2008,2009,2012 Thomas A Caswell
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


#ifndef IDEN_OBJ
#define IDEN_OBJ
#include <string>

#include "md_store.h"

namespace tracking
{
class hash_case;
class Master_box;

}

namespace utilities
{
class Md_store;
}

namespace iden
{
class Wrapper_i_plu;
class Image_base;
class MD_parser;
class Image2D;


/**
   Class to encapsulate all of the image processing for particle identification
 */
class Iden
{
public:
  Iden(const utilities::Md_store& in):params_(in)
  {
  };			// needs arguements
  ~Iden(){};
  
  /**
     \deprecated don't use this
   */
  void set_image_src(Image_base * image);

  void set_md_parser(MD_parser * parser);

  /**
     Set the parameters used by this iden object to extract features
   */
  void set_params(const utilities::Md_store& param_in);
  
  /**
     \deprecated don't use this
   */
  Wrapper_i_plu * fill_wrapper(unsigned int frames=0,unsigned int start=0);

  /**
     \deprecated don't use this
   */
  Wrapper_i_plu * fill_wrapper_avg(unsigned int frames=0,unsigned int start=0);
  
  void process_frame(const Image2D & img_in,
                     unsigned int frame_number,
                     const utilities::Md_store * md_store_in,
                     Wrapper_i_plu & wrapper_out) const;
  void process_frames(const Image_base & imgs,
                      unsigned int start_frame, 
                      unsigned int frames,
                      Wrapper_i_plu & wrapper_out) const;

  void process_frames_avg(const Image_base & imgs,
                          unsigned int avg_count,
                          unsigned int start_frame, 
                          unsigned int frames,
                          Wrapper_i_plu & wrapper_out) const;
private:
  /**
     The source of the images to process
   */
  Image_base * img_src_;
  

  /**
     Parser for metadata
   */
  MD_parser * parser_;
  
  /**
     Parameter object, holds the parameters for the image processing
   */
  utilities::Md_store params_;
  
};

  


}

#endif
