//Copyright 2009-2010 Thomas A Caswell
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
#include <sstream>

#include "enum_utils.h"
using std::string;

namespace utilities
{

const int format_padding_ = 7;

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
    out = std::string("eccentricity") ; 
    return out;
  case D_NEXT_INDX: 
    out = std::string("next_part") ; 
    return out;
  case D_PREV_INDX: 
    out = std::string("prev_part") ; 
    return out; 
  case D_DX: 
    out =  std::string("x_shift");
    return out;
  case D_DY: 
    out =  std::string("y_shift");
    return out;
    break; 
  case D_ORGX: 
    out =  "org_x";
    return out;
    break; 
  case D_ORGY: 
    out = "org_y";
    return out;
    break; 
  case D_ZPOS: 
    out = std::string("z") ; 
    return out;
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
  case D_N_SIZE:
    out = "neighborhood_size";
    return out;
  default:
    throw("unknown type");
  }
  return out;
  
  
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
  case D_NEXT_INDX:
  case D_PREV_INDX: 
  case D_TRACKID:
  case D_ORGX: 
  case D_ORGY: 
  case D_MULT:
  case D_N_SIZE:
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


std::string format_dset_name(D_TYPE type,int comp_num)
{
  std::ostringstream o;
  o.width(format_padding_);
  o.fill('0');
  o<<std::right<<comp_num;
  return  DT2str_s(type) +"_"+  o.str();

}


std::string format_name(const std::string & name ,int comp_num)
{
  std::ostringstream o;
  o.width(format_padding_);
  o.fill('0');
  o<<std::right<<comp_num;
  return  name +"_"+  o.str();

}




/**
   Helper function to make string in to V_TYPE
*/
V_TYPE str2VT_s(const std::string& type_str)
{

  if(type_str.compare("float") == 0)
    return utilities::V_FLOAT;
  else if(type_str.compare("int") == 0)
    return utilities::V_INT;
  else if(type_str.compare("string") == 0)
    return utilities::V_STRING;
  else
    return utilities::V_ERROR;
}

/**
   Helper function to make string in to V_TYPE
*/
V_TYPE str2VT_s(const char *const  in)
{
  string tmp(in);
  return str2VT_s(tmp);
}

std::string VT2str_s(V_TYPE type)
{
  switch(type)
  {
  case V_INT:
    return "int";
  case V_FLOAT:
    return "float";
  case V_COMPLEX:
    return "complex";
  case V_STRING:
    return "string";
  case V_ERROR:
  default:
    return "type unknown";


  }
  
}


}
