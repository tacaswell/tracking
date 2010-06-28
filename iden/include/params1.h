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
#ifndef PARAMS1_H
#define PARAMS1_H

#include <fstream>
#include <string>
#ifdef MATLAB_MEX_FILE
#include "mex.h"
#endif
/*
 * modified 06/08 by tcaswell to make compatable with matlab mex code
 */

/*//This file contains the code for managing Params, the class that holds parameters from the command-line*/
namespace iden
{

std::string make_file_number(const int framenumber, const int stacknumber);

class Params
{
public:

#ifdef MATLAB_MEX_FILE
  /**
     Regular used constructor that reads command-line parameters
  */
  Params(const int numargs, const mxArray *arguments);	
#endif
  /** 
      Constructor that simply checks for enough arguments
  */
  Params(const int numargs);			
  /** 
      Constructor 
  */
  Params(int,float,int,float,int,float);
  
  const Params &operator=(const Params &);	/*//overloaded assignment operator*/

  void PrintOutParameters(std::ostream &out) const;

  ~Params();
  int get_feature_radius() const
  {return feature_radius_;}
  float get_hwhm_length() const
  {return hwhm_length_;}
  int get_dilation_radius() const
  {return dilation_radius_;}
  float get_pctle_threshold() const
  {return pctle_threshold_;}
  int get_mask_radius() const
  {return mask_radius_;}
  int get_top_cut() const
  {return top_cut_;}
  
	
private:
  
  int feature_radius_;		/*//characteristic radius of particles, in pixels*/
  float hwhm_length_;		/*//hwhm (half-width, half-maximum for gaussian kernel), in pixels*/
  int dilation_radius_;	/*//radius for dilation mask, in pixels*/
  float pctle_threshold_;	/*//percentile brightness to cutoff images*/
  int mask_radius_;		/*//radius for kernels for calculating mass, x and y positions, r^2, etc.*/
  float top_cut_;		// the percent of bright pixels to truncate off the top
};
}

#endif
