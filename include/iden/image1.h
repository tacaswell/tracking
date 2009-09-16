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
#ifndef IMAGE1_H
#define IMAGE1_H

#include "ipp.h"

#include "mex.h"
/*#include <string>*/


/*modified 6/08
 *Beating this code in to a form that can be compiled in to mex files
 *tcaswell
 */

/*//This file contains definitions for the Image2D, a container class that carries*/
/*//around 32-bit floating point IPP images and other associated data*/
/*//functions for importing and exporting data are also included here*/
namespace iden
{

class Image2D
{
public:
	Image2D();
	Image2D(const int image_length, const int image_width);
	~Image2D();

	int getx(const int index1D);	/*//given the index of image data, returns x-position in pixels*/
	int gety(const int index1D);	/*//given the index of image data, returns y-position in pixels*/
	int get1Dindex(const int x, const int y);	/*//given x and y, return 1-dim index of image*/

	Ipp32f *get_image2D() const
	{return imagedata;}
	int get_stepsize() const
	{return stepsize;}
	int get_width() const
	{return width;}
	int get_length() const
	{return length;}
	int get_numberofpixels() const
	{return numberofpixels;}
	int get_height() const
	{return length;}
	IppiSize get_ROIfull() const
	{return ROIfull;}


private:
	Ipp32f *imagedata;					/*//actual image data in Ipp32f format*/
	int stepsize;						/*//stepsize for use with IPP functions (step size in bytes)*/
	int width;							/*//width of image, in pixels*/
	int length;							/*//length (or height) of image, in pixels*/
	int numberofpixels;					/*//total number of pixels in image*/
	IppiSize ROIfull;					/*//ROI of the full image, used in IPP functions, when including whole image*/

};

Image2D mat_to_IPP(const mxArray *data);

IppStatus IPP_to_mat(mxArray *data, Image2D &image_out);
}

#endif
