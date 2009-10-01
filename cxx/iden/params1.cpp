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

#include "iden/params1.h"
#include "mex.h"
#include <stdlib.h>
#include <string>

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::endl;
using std::ostream;


using iden::Params;


Params::~Params()
{
}
Params::Params(int fr,float hwhm,int di,float pt,int mr):
  feature_radius_(fr),
  hwhm_length_(hwhm),dilation_radius_(di),pctle_threshold_(pt),mask_radius_(mr)
{
}


  
const Params &Params::operator =(const Params &Parameters)
{
  if (&Parameters != this) {
    /*	infile_stem = Parameters.get_infilestem();
	outfile_stem = Parameters.get_outfilestem();
	file_extension = Parameters.get_file_extension();
	start_frameofstack = Parameters.get_start_frameofstack();
	end_frameofstack = Parameters.get_end_frameofstack();
	start_stack = Parameters.get_start_stack();
	end_stack = Parameters.get_end_stack();
    */
    feature_radius_ = Parameters.get_feature_radius();
    hwhm_length_ = Parameters.get_hwhm_length();
    dilation_radius_ = Parameters.get_dilation_radius();
    pctle_threshold_ = Parameters.get_pctle_threshold();
    mask_radius_ = Parameters.get_mask_radius();
		
  }
  else {
    cout << "Error: Parameters cannot be assigned to self!" << endl;
  }
  return *this;
}


void Params::PrintOutParameters(std::ostream &out) const
{
  out<<"feature_radius_: " <<get_feature_radius() <<std::endl;
  out<<"hwhm_length_: "    <<get_hwhm_length()    <<std::endl;
  out<<"dilation_radius_: "<<get_dilation_radius()<<std::endl;
  out<<"pctle_threshold_: "<<get_pctle_threshold()<<std::endl;
  out<<"mask_radius_: "    <<get_mask_radius()    <<std::endl;
}

  
