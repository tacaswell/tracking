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
   Enumeration for wrapper types.  I do not think that this is used by anything
   @todo remove this
 */
typedef enum W_TYPE{                                                                
  W_MATLAB = 0,                                                           
  W_TEXT,                                                                 
  W_NING,                                                                 
  W_HDF,                                                                  
} W_TYPE;                                                                        

/**
   Enumeration for data types (where type is the physical meaning of
   the data).  If you add to this you MUST update the count below this,
   add the D_TYPE to the V_TYPE conversion function, and D2Str function.
*/
typedef enum D_TYPE {
  D_INDEX               =0,	// index of particle in case,
				// (frame,indx) is unique and should
				// be persistent in wrappers (wrapper)
  D_XPOS  		=1,	// x-position (plu)
  D_YPOS  		=2,	// y-position (plu)
  D_FRAME  		=3,	// frame (wrapper)
  D_I  			=4,	// integrated intensity (plu)
  D_R2  		=5,	// radius of gyration (plu)
  D_E 			=6,	// eccentricity (plu)
  D_NEXT_INDX 		=7,	// index of the next particle in the
				// track (track)
  D_PREV_INDX  		=8,	// index of the previous particle in
				// the track (track)
  D_DX  		=9,	// x-shift (plu)
  D_DY  		=10,	// y-shift (plu)
  D_ORGX  		=11,	// original x-position (computed)
  D_ORGY  		=12,	// original y-position (computed)
  D_ZPOS 		=13,	// z-position (link or meta-data)
  D_TRACKID 		=14,	// track id (track)
  D_S_ORDER_PARAMETER 	=15,	// scalar order parameter (computed)
  D_MULT 		=16,	// multiplicity (plu)
  D_N_SIZE  		=17,	// neighborhood size (computed)
  D_SENTRY			// this entry must remain last, and I
				// am making assumptions about how the
				// numbering will work in the compiler
  
} D_TYPE;


/**
   Enumeration for the type of value the data is (ie float, int,complex)
 */
typedef enum V_TYPE
  {
    V_ERROR = -1,		// error type
    V_UINT = 0,			// unsigned integer
    V_INT =1,			// integer
    V_FLOAT =2,			// float
    V_COMPLEX =3,		// complex
    V_STRING =4,		// string
    V_BOOL =5,			// bool
    V_TIME =6,			// date/time
    V_GUID =7,			// guid from MM meta-data
    

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
D_TYPE str2DT_s(std::string in);
/**
   Helper function to format dset names with computation numbers attached
 */
std::string format_dset_name(D_TYPE type,int comp_num);
/**
   Helper function to format computation numbers on to strings.
 */
std::string format_name(const std::string & name ,int comp_num);


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
