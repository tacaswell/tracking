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

#ifndef ENUM_UTILS
#define ENUM_UTILS

#include<string>

namespace utilities
{

/**
   Enumeration for wrapper types
 */
typedef enum W_TYPE{                                                                
  W_MATLAB = 0,                                                           
  W_TEXT,                                                                 
  W_NING,                                                                 
  W_HDF,                                                                  
} W_TYPE;                                                                        

/**
   Enumeration for data types (where type is the physical meaning of
   the data)
*/
typedef enum D_TYPE {
  D_INDEX=0, 
  D_XPOS, 
  D_YPOS, 
  D_FRAME, 

  D_I, 
  D_R2, 
  D_E,
  
  D_NEXT,
  D_PREV, 
  
  D_DX, 
  D_DY, 
  D_ORGX, 
  D_ORGY, 
  
  D_ZPOS,
  D_UNQID,

  D_TRACKID,
  D_S_ORDER_PARAMETER,
  D_MULT,
} D_TYPE;


/**
   Number of enumerated data types.  This needs to be right for the
   flat wrappers to work properly
 */
const int D_TYPE_COUNT = 18;

/**
   Enumeration for the type of value the data is (ie float, int,complex)
 */
typedef enum V_TYPE
  {
    V_ERROR = -1,
    V_INT = 0,
    V_FLOAT,
    V_COMPLEX,
    V_STRING
  }V_TYPE;
/**
   Helper function to map between D_TYPE and V_TYPE
 */
V_TYPE v_type(D_TYPE in);


/**
   Helper functions to convert D_TYPEs in to short name strings
 */
std::string DT2str_s(D_TYPE in);
/**
   Helper functions to convert short name strings in to D_TYPEs 
 */
D_TYPE DT2str_s(std::string in);
/**
   Helper function to format dset names with computation numbers attached
 */
std::string format_dset_name(D_TYPE type,int comp_num);

/**
   Helper function to make v_types to strings
 */
std::string VT2str_s(V_TYPE);

/**
   Helper function to make string in to V_TYPE
 */
V_TYPE str2VT_s(const std::string& in);
/**
   Helper function to make string in to V_TYPE
 */
V_TYPE str2VT_s(const char  * const in);

}


#endif
