//Copyright 2008,2009 Thomas A Caswell
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

#include "params1.h"
namespace tracking
{
class hash_case;
class Master_box;

}
namespace utilities
{
class Wrapper_i_plu;
class Pair;

}


namespace iden
{

/**
   Class to encapsulate all of the image processing for particle identification
 */
class Iden
{
public:
  Iden(Params& in):params_(in)
  {
  };			// needs arguements
  ~Iden(){};
  
  void set_fname(const std::string &);
  void set_params(const Params& param_in);
  
  void fill_wrapper(utilities::Wrapper_i_plu &,utilities::Pair,int frames=0,int start=0);
  
private:
  /**
     name of the file to be working on
   */
  std::string fname_;
  /**
     Parameter object, holds the parameters for the image processing
   */
  Params params_;
  
};

  


}

#endif
