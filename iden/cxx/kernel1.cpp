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
// tac 2009-09-15
// 
#include <math.h>
#include <iostream>
#include <fstream>
 
#include "ipp.h"

#include "kernel1.h"

using std::cout;
using std::endl;


using iden::Gaussian_Kernel;
using iden::Convolution_Kernel;
using iden::Tophat_Kernel;
using iden::Dilation_Kernel;

#define epsilon 0.0001

void Print2DKernel8u(const int diameter, const Ipp8u *kernel)
{
	for(int i=0;i<diameter; i++) {
		for(int j=0;j<diameter; j++) {
			cout << (float) kernel[i*diameter + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Print2DKernel32f(const int diameter, const Ipp32f *kernel)
{
	for(int i=0;i<diameter; i++) {
		for(int j=0;j<diameter; j++) {
			cout << (float) kernel[i*diameter + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


Gaussian_Kernel::Gaussian_Kernel(const int radius, const float HWHM, const int tif_width, const int tif_length):
anchor_point(0), offset(0), kernel(NULL), kernel_length(0)
{
	/*//Gaussian kernel is a one-dimensional array*/
	/*//kernellength is an odd number*/
	int kernel_length_big = 2 * radius + 1;
	int i=0, j=0;
	Ipp32f *gausskernel_big = ippsMalloc_32f((int) kernel_length_big);
	Ipp32f total = 0, delta_x = 0;
	/*//HWHM is the half-width, half-maximum of the gaussian function*/
	/*//to get the proper factor, divide by sqrt(log(2)); which can easily be derived*/
	/*//by setting: exp(-(x_half / alpha)^2) = 1/2, where x_half is the HWHM.*/
	/*//i.e. alpha = HWHM / sqrt(log(2))*/
	const float gaussian_divisor = HWHM / 0.8325546;
	for(i=0; i<kernel_length_big; i++) {
/*//		delta_x = (Ipp32f) (i-radius)/(2.0 * noise);*/
		delta_x = (Ipp32f) (i-radius)/gaussian_divisor;
		gausskernel_big[i] = exp(-delta_x * delta_x);
		total += gausskernel_big[i];
	}
	for(i=0; i<kernel_length_big; i++) {
		gausskernel_big[i] /= total;
	}
	
	/*//keep only that part of the kernel that is greater than threshold*/
	int k = 0;
	i=0;
	while(gausskernel_big[i] <= epsilon) {
		k=i;
		i++;
	}
	
	/*//Trim the Gaussian kernel so that the zeros are not carried around needlessly*/
	/*//gausskernellength is an ODD number*/
	kernel_length = kernel_length_big - 2 * k - 2;
	kernel = ippsMalloc_32f((int) kernel_length);
	for(j=0; j < kernel_length; j++) {
		kernel[j] = gausskernel_big[j+k+1];
	}
	anchor_point = (kernel_length - 1) / 2;
	int delta = kernel_length -1;
	ROI_size.width = tif_width - delta;
	ROI_size.height = tif_length - delta;
	IppiPoint ROI_startpoint = {anchor_point, anchor_point};
	offset = ROI_startpoint.y * tif_width + ROI_startpoint.x;
	
	/*//Cleanup memorys*/
	ippsFree(gausskernel_big);
	gausskernel_big = NULL;
}


Gaussian_Kernel::~Gaussian_Kernel()
{
	ippsFree(kernel);
	kernel = NULL;
}

Tophat_Kernel::Tophat_Kernel(const int radius, const int tif_width, const int tif_length):
offset(0)
{
	int maskwidth = 2 * radius + 1;
	int delta = maskwidth - 1;
	
	/*//construct regions for mask and ROI*/
	ROI_size.width = tif_width - delta;
	ROI_size.height = tif_length - delta;
	mask_size.width = maskwidth;
	mask_size.height = maskwidth;
	
	/*//the following point is both the corner for the image ROI and center of filter box*/
	anchor_point.x = radius;
	anchor_point.y = radius;
	offset = (anchor_point.y * tif_width + anchor_point.x);
	
}

Tophat_Kernel::~Tophat_Kernel()
{
}

Convolution_Kernel::Convolution_Kernel(const int radius, const int tif_width, const int tif_length):
circle_kernel(NULL), r2_kernel(NULL), ramp_x_kernel(NULL), ramp_y_kernel(NULL)
{
	
	IppStatus status;
	
	/*//circle kernel size calculations*/
	/*//separation is merely diameter, but made into an odd number*/
	int diameter = 2 * radius + 1;
	int area = diameter * diameter;
	int ramp_step = 4 * diameter;
	int convolution_kernel_step = 4 * diameter;
	IppiSize ramp_roi_size = {diameter, diameter};
	
	/*//allocate memory for kernel and ramps*/
	//Ipp32f *ramp_x = ippsMalloc_32f(area);
	//Ipp32f *ramp_y = ippsMalloc_32f(area);
	//Ipp32f *circle = ippsMalloc_32f(area);
	//Ipp32f *r2 = ippsMalloc_32f(area);
	circle_kernel = ippsMalloc_32f(area);
	r2_kernel = ippsMalloc_32f(area);
	ramp_x_kernel = ippsMalloc_32f(area);
	ramp_y_kernel = ippsMalloc_32f(area);

	//tac7/08
	sin_kernel = ippsMalloc_32f(area);
	cos_kernel = ippsMalloc_32f(area);
	Ipp32f *t = ippsMalloc_32f(area);
	
	/*//create ramps from -radius to +radius for r^2 and circular masks*/
	status = ippiImageRamp_32f_C1R(ramp_x_kernel, ramp_step, ramp_roi_size, -radius, 1, ippAxsHorizontal);
	status = ippiImageRamp_32f_C1R(ramp_y_kernel, ramp_step, ramp_roi_size, -radius, 1, ippAxsVertical);
	
	/*//square the ramps and then finally add*/
	//	status = ippiSqr_32f_C1IRSfs(ramp_x, ramp_step, ramp_roi_size, 0);
	//	status = ippiSqr_32f_C1IRSfs(ramp_y, ramp_step, ramp_roi_size, 0);
	//	status = ippiAdd_32f_C1RSfs(ramp_x, ramp_step, ramp_y, ramp_step, r2, ramp_step, ramp_roi_size,0);
	status = ippsHypot_32f_A11(ramp_x_kernel,ramp_y_kernel,r2_kernel,area);

	/*
	 * Change from 3 commands to one.  Requires pulling in an extra lib, but I need it anyway
	 * for the atan2 later.  
	 */

	status = ippiCopy_32f_C1R(r2_kernel, ramp_step, circle_kernel, ramp_step, ramp_roi_size);

	/*//trim away values greater than radius^2 for r2 kernel*/
	status = ippiThreshold_GTVal_32f_C1IR(r2_kernel, ramp_step, ramp_roi_size,radius, 0);

	/*//trim away values greater than radius^2 for circle kernel*/
	status = ippiThreshold_GTVal_32f_C1IR(circle_kernel, ramp_step, ramp_roi_size, radius, -1);
	status = ippiThreshold_GTVal_32f_C1IR(circle_kernel, ramp_step, ramp_roi_size,-1, 1);
	status = ippiThreshold_LTVal_32f_C1IR(circle_kernel, ramp_step, ramp_roi_size,1, 0);

	/*//create ramps from constant-radius to constant+radius for r^2 and circular masks*/
	//	constantoffset = 1;
	//	status = ippiImageRamp_32f_C1R(ramp_x, ramp_step, ramp_roi_size, constantoffset, 1, ippAxsHorizontal);
	//	status = ippiImageRamp_32f_C1R(ramp_y, ramp_step, ramp_roi_size, constantoffset, 1, ippAxsVertical);

	status = ippsAtan2_32f_A11(ramp_x_kernel,ramp_y_kernel,t,area);
  
	status = ippsMulC_32f_I(2,t,area);
	
	status = ippsSinCos_32f_A11(t,sin_kernel,cos_kernel,area);
	cos_kernel[radius*diameter + radius] = 0;

	/*//multiply ramps by circle kernel to eliminate all values beyond the radius*/
	status = ippiMul_32f_C1IR(circle_kernel, ramp_step, ramp_x_kernel, ramp_step, ramp_roi_size);
	status = ippiMul_32f_C1IR(circle_kernel, ramp_step, ramp_y_kernel, ramp_step, ramp_roi_size);
	status = ippiMul_32f_C1IR(circle_kernel, ramp_step, sin_kernel, ramp_step, ramp_roi_size);
	status = ippiMul_32f_C1IR(circle_kernel, ramp_step, cos_kernel, ramp_step, ramp_roi_size);
	status = ippiSqr_32f_C1IR(r2_kernel,ramp_step,ramp_roi_size);
	

	/*//Convert kernels to final forms*/
	//	status = ippiConvert_32f32f_C1R(circle, ramp_step, circle_kernel, convolution_kernel_step, ramp_roi_size);
	//	status = ippiConvert_32f32f_C1R(r2, ramp_step, r2_kernel, convolution_kernel_step, ramp_roi_size);
	//	status = ippiConvert_32f32f_C1R(ramp_x, ramp_step, ramp_x_kernel, convolution_kernel_step, ramp_roi_size);
	//	status = ippiConvert_32f32f_C1R(ramp_y, ramp_step, ramp_y_kernel, convolution_kernel_step, ramp_roi_size);


	  ippsFree(t);
	  t=NULL;
	/*//free memory*/
	  //	ippsFree(ramp_x);
	  //	ippsFree(ramp_y);
	  //	ippsFree(circle);
	  //	ippsFree(r2);
	  //	ramp_x = NULL;
	  //	ramp_y = NULL;
	  //	circle = NULL;
	  //	r2 = NULL;

	/*//setup size parameters*/
	kernel_size.width = diameter;
	kernel_size.height = diameter;
	kernel_step = convolution_kernel_step;

	ROI_size.width = tif_width - diameter + 1;
	ROI_size.height = tif_length - diameter + 1;
	offset = (radius * tif_width + radius);
}

Convolution_Kernel::~Convolution_Kernel()
{
	ippsFree(circle_kernel);
	ippsFree(r2_kernel);
	ippsFree(ramp_x_kernel);
	ippsFree(ramp_y_kernel);
	ippsFree(cos_kernel);
	ippsFree(sin_kernel);
	circle_kernel = NULL;
	r2_kernel = NULL;
	ramp_x_kernel = NULL;
	ramp_y_kernel = NULL;
	cos_kernel = NULL;
	sin_kernel = NULL;
}

Dilation_Kernel::Dilation_Kernel(const int radius, const int tif_width, const int tif_length):
dilation_kernel(NULL), offset(0)
{
	
	IppStatus status;
	
	/*//circle kernel size calculations*/
	/*//separation is merely diameter, but made into an odd number*/
	int diameter = 2 * radius + 1;
	int area = diameter * diameter;
	int ramp_step = 2 * diameter;
	int dilation_kernel_step = diameter;
	IppiSize ramp_roi_size = {diameter, diameter};
	
	/*//allocate memory for kernel and ramps*/
	Ipp16s *ramp_x = ippsMalloc_16s(area);
	Ipp16s *ramp_y = ippsMalloc_16s(area);
	Ipp16s *circle = ippsMalloc_16s(area);
	dilation_kernel = ippsMalloc_8u(area);
	
	/*//create ramps from -radius to +radius for r^2 and circular masks*/
	status = ippiImageRamp_16s_C1R(ramp_x, ramp_step, ramp_roi_size, -radius, 1, ippAxsHorizontal);
	status = ippiImageRamp_16s_C1R(ramp_y, ramp_step, ramp_roi_size, -radius, 1, ippAxsVertical);
	
	/*//square the ramps and then finally add*/
	status = ippiSqr_16s_C1IRSfs(ramp_x, ramp_step, ramp_roi_size, 0);
	status = ippiSqr_16s_C1IRSfs(ramp_y, ramp_step, ramp_roi_size, 0);
	status = ippiAdd_16s_C1RSfs(ramp_x, ramp_step, ramp_y, ramp_step, circle, ramp_step, ramp_roi_size,0);
	
	/*//trim away values greater than radius^2 for circle kernel*/
	status = ippiThreshold_GTVal_16s_C1IR(circle, ramp_step, ramp_roi_size,radius * radius, -1);
	status = ippiThreshold_GTVal_16s_C1IR(circle, ramp_step, ramp_roi_size,-1, 1);
	status = ippiThreshold_LTVal_16s_C1IR(circle, ramp_step, ramp_roi_size,1, 0);

	/*//Convert kernel to 8-bit unsigned integer*/
	status = ippiConvert_16s8u_C1R(circle, ramp_step, dilation_kernel, dilation_kernel_step, ramp_roi_size);

	/*//free memory from ramps*/
	/*//free memory*/
	ippsFree(ramp_x);
	ippsFree(ramp_y);
	ippsFree(circle);
	ramp_x = NULL;
	ramp_y = NULL;
	circle = NULL;
	
	/*//construct regions for mask and ROI*/
	ROI_size.width = tif_width - diameter + 1;
	ROI_size.height = tif_length - diameter + 1;
	mask_size.width = diameter;
	mask_size.height = diameter;
	
	/*//the following point is both the corner for the image ROI and center of circle kernel*/
	anchor_point.x = radius;
	anchor_point.y = radius;
	offset = (anchor_point.y * tif_width + anchor_point.x);
	
}

Dilation_Kernel::~Dilation_Kernel()
{
	ippsFree(dilation_kernel);
	dilation_kernel = NULL;
}
