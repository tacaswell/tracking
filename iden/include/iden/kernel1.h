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
#ifndef KERNEL1_H
#define KERNEL1_H
  
#include "ipp.h"
#include "params1.h"

/*//This file contains the code for creating the different kernels*/

/* modified 7/2008 by Thomas Caswell to add computation of 'eccentricity'
 */
namespace iden
{

void Print2DKernel8u(const int diameter, const Ipp8u *kernel);
void Print2DKernel32f(const int diameter, const Ipp32f *kernel);

class Tophat_Kernel
{
public:
	Tophat_Kernel();
	Tophat_Kernel(const int radius, const int tif_width, const int tif_length);
	~Tophat_Kernel();
	
	IppiSize get_ROI_size() const
	{return ROI_size;}
	IppiSize get_mask_size() const
	{return mask_size;}
	IppiPoint get_anchor_point() const
	{return anchor_point;}
	int get_offset() const
	{return offset;}
	
private:
	IppiSize ROI_size;
	IppiSize mask_size;
	IppiPoint anchor_point;
	int offset;
	
};

class Gaussian_Kernel
{
public:
	Gaussian_Kernel();
	Gaussian_Kernel(const int radius, const float hwhm, const int tif_width, const int tif_length);
	~Gaussian_Kernel();
	
	Ipp32f *get_gaussian_kernel() const
	{return kernel;}
	int get_anchor_point() const
	{return anchor_point;}
	IppiSize get_ROI_size() const
	{return ROI_size;}
	int get_offset() const
	{return offset;}
	int get_kernel_length() const
	{return kernel_length;}
	
private:
	Ipp32f *kernel;
	int anchor_point;
	int kernel_length;
	IppiSize ROI_size;
	int offset;
};

class Dilation_Kernel
{
public:
	Dilation_Kernel();
	Dilation_Kernel(const int radius, const int tif_width, const int tif_length);
	~Dilation_Kernel();

	Ipp8u *get_dilation_kernel() const
	{return dilation_kernel;}
	
	IppiSize get_ROI_size() const
	{return ROI_size;}
	IppiSize get_mask_size() const
	{return mask_size;}
	IppiPoint get_anchor_point() const
	{return anchor_point;}
	int get_offset() const
	{return offset;}
	
private:
	Ipp8u *dilation_kernel;		/*//1 inside radius; 0 otherwise; for dilation*/
	IppiSize ROI_size;
	IppiSize mask_size;
	IppiPoint anchor_point;
	int offset;					/*//offset for starting point ROI in image*/
};

class Convolution_Kernel
{
public:
	Convolution_Kernel();
	Convolution_Kernel(const int radius, const int tif_width, const int tif_length);
	~Convolution_Kernel();

	Ipp32f *get_circle_kernel() const
	{return circle_kernel;}
	Ipp32f *get_r2_kernel() const
	{return r2_kernel;}
	Ipp32f *get_ramp_x_kernel() const
	{return ramp_x_kernel;}
	Ipp32f *get_ramp_y_kernel() const
	{return ramp_y_kernel;}

	//tac 7/08
	Ipp32f *get_sin_kernel() const
	{return sin_kernel;}
	Ipp32f *get_cos_kernel() const
	{return cos_kernel;}



	IppiSize get_kernel_size() const
	{return kernel_size;}
	int get_kernel_step() const
	{return kernel_step;}
	IppiSize get_ROI_size() const
	{return ROI_size;}
	int get_offset() const
	{return offset;}

private:
	Ipp32f *circle_kernel;		/*//1 inside radius; 0 otherwise*/
	Ipp32f *r2_kernel;			/*//r^2 inside radius; 0 otherwise*/
	Ipp32f *ramp_x_kernel;		/*//linear gradient in x inside radius; 0 otherwise*/
	Ipp32f *ramp_y_kernel;		/*//linear gradient in y inside radius; 0 otherwise*/
	
	//tac7/08
	Ipp32f *sin_kernel;               //sin function based on angle
	Ipp32f *cos_kernel;               //cos function based on angle
	
	IppiSize kernel_size;
	IppiSize ROI_size;
	int kernel_step;
	int offset;					/*//offset for starting point ROI in image*/
	int constantoffset;			/*//constant offset to be added to ramp masks for calculating position*/
};
}



#endif
