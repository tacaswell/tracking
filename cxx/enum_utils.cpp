//Copyright 2009 Thomas A Caswell
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
#include <string>

#include "enum_utils.h"
namespace utilities
{


std::string DT2str_s(D_TYPE in)
{
  std::string out;
  
  switch(in)
  {
  case D_INDEX: 
    out = std::string("index");
    return out;
    break;
  case D_XPOS: 
    out = std::string("x") ; 
    return out; 
  case D_YPOS: 
    out = std::string("y") ; 
    return out; 
  case D_FRAME: 
    out = std::string("frame") ; 
    return out; 
  case D_I: 
    out = std::string("intensity") ; 
    return out; 
  case D_R2: 
    out = std::string("R2") ; 
    return out; 
  case D_E: 
    out = std::string("eccentricty") ; 
    return out;
  case D_NEXT: 
    out = std::string("next_part") ; 
    return out;
  case D_PREV: 
    out = std::string("prev_part") ; 
    return out; 
  case D_DX: 
    out =  "x_shift";
    break; 
  case D_DY: 
    out =  "y_shift";
    break; 
  case D_ORGX: 
    out =  "org_x";
    break; 
  case D_ORGY: 
    out = "org_y";
    break; 
  case D_ZPOS: 
    out = std::string("z") ; 
    return out;
  case D_UNQID: 
    out = std::string("unique_id") ; 
    return out;
  case D_TRACKID: 
    out = std::string("track_id") ; 
    return out;
  case D_S_ORDER_PARAMETER: 
    out = std::string("scaler_order_parameter") ; 
    return out;
  case D_MULT:
    out = "multiplicity";
    return out;
  default:
    throw("unknown type");
    break;
  }
}

D_TYPE DT2str_s(std::string in)
{
  throw "function not written yet";
  return D_TRACKID;
}

V_TYPE v_type(D_TYPE in)
{
  switch(in)
  {
    // integer data types
  case D_INDEX: 
  case D_FRAME: 
  case D_NEXT:
  case D_PREV: 
  case D_UNQID:
  case D_TRACKID:
  case D_ORGX: 
  case D_ORGY: 
  case D_MULT:
    return V_INT;

    // float data types
  case D_XPOS: 
  case D_YPOS: 
  case D_I: 
  case D_R2: 
  case D_E:
  case D_DX: 
  case D_DY: 
  case D_ZPOS:
    return V_FLOAT;

    // complex data types
  case D_S_ORDER_PARAMETER:
    return V_COMPLEX;

    // default to break stuff
  default:
    return V_ERROR;
  }
}


}
