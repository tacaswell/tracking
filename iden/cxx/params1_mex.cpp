//Peter J. Lu
//Copyright 2008 Peter J. Lu. 
//http://www.peterlu.org
//plu@fas.harvard.edu
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
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of freeimage-license, the
//licensors of this Program grant you additional permission to convey
//the resulting work.  Corresponding Source for a non-source form of
//such a combination shall include the source code for the parts of
//FreeImage used as well as that of the covered work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

//copied form https://plutarc.svn.sourceforge.net/svnroot/plutarc/trunk/matlab_wrapper rev9
// Modified by Thomas Caswell tcaswell@uchicago.edu 09/2009-

#include "params1.h"
#if MATLAB_MEX_FILE
#include "mex.h"
#endif 

#include <stdlib.h>
#include <string>

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::endl;
using std::ostream;


using iden::Params;


Params::Params(const int numargs,const mxArray *arguments):
  feature_radius_(0), hwhm_length_(0), 
  dilation_radius_(0), pctle_threshold_(0), mask_radius_(0)
{
  /*	file_extension = ".tif";
   *	infile_stem = arguments[1];
   *	outfile_stem = arguments[2];
   *
   *	start_frameofstack = atoi(arguments[3]);
   *	end_frameofstack = atoi(arguments[4]);
   *	start_stack = atoi(arguments[5]);
   *	end_stack = atoi(arguments[6]);
   */
  double *args = mxGetPr(arguments);
    
  feature_radius_ = (int)args[0];
  hwhm_length_ = args[1];
  cout<<hwhm_length_<<endl;

  dilation_radius_ = (int)args[2];
  mask_radius_ = (int)args[3];
  pctle_threshold_ = args[4];

}
