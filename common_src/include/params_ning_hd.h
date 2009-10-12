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
#include "params_ning.h"
#include "wrapper_i_ning_hd.h"

#ifndef PARAMS_NING_HD
#define PARAMS_NING_HD
namespace tracking{
  /**
     Paramter class for the hacked down version of the file ning sent me
*/
class params_ning_hd :public params_file{
public:
  params_ning_hd(int a ,std::map<utilities::D_TYPE,int> contents)
    :params_file(a,contents)
  {
    fname = string("new_ning.txt");
  }
  
  Wrapper_in* make_wrapper_in(){
    return new wrapper_i_ning_hd(this);
  }
  

};
}

#endif
