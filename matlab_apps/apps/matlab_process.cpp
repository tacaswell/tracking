//Copyright 2008,2009 Thomas A Caswell
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
//

//copied form https://plutarc.svn.sourceforge.net/svnroot/plutarc/trunk/matlab_wrapper rev9
// Modified by Thomas Caswell tcaswell@uchicago.edu 09/2009-
#include "mex.h"
#include "ipp.h"


#include "image1.h"
#include "params1.h"
#include "data_proc1.h"
#include "kernel1.h"
#include "fileops1.h"

#include "iden.h"

/*//header files for Intel and TIFF code*/

/*//standard C++ header files*/
#include <time.h>
#include <string>


#include <iostream>


/*//defined constants*/



using iden::Params;
using iden::Image2D;
using std::cout;
using std::endl;



extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] )
     
{ 


  
  if(nrhs!=2){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }

 //mexPrintf("Hello, world!\n"); 
 IppStatus status;
 int k,n,m,i,j;
 int step;
 double *data1;
 double *data2;
 



 
 Params parm = Params(mxGetN(prhs[1]) ,prhs[1]);


 


  
 Image2D image_in(mxGetN(prhs[0]),mxGetM(prhs[0]));


 image_in.set_data(prhs[0]); 
 

 image_in.trim_max(parm.get_top_cut());
 
 cout<<"cutting: "<<parm.get_top_cut()*100<<"% off the top"<<endl;
 
 
 

    
 Image2D image_bpass(image_in.get_height(), image_in.get_width());	
//     /*//result of bandpassing original image data*/

 Image2D image_bpass_thresh(image_in.get_height(), image_in.get_width());	
 //     /*//result of setting lower threshold on bandpassed image*//
 Image2D image_localmax(image_in.get_height(), image_in.get_width());
//     /*//holds 1 in pixel locations with local max, 0 otherwise*/
    
 RecenterImage(image_in);
    
//     /*//Convolve kernels with image (gauss and top-hat); then subtract the two (i.e. bandpass filter them)*/

 status = BandPass_2D(image_in, image_bpass, parm.get_feature_radius(), parm.get_hwhm_length());
	
 status = FindLocalMax_2D(image_bpass, image_bpass_thresh, image_localmax, (int)parm.get_pctle_threshold(),
			  parm.get_dilation_radius());
	
 RecenterImage(image_bpass_thresh);

//     /*//find 1-D indices of local maxima*/
// 	int counter = 0;
// 	/*//	Ipp32f (*particledata)[8] = ParticleStatistics(image_localmax, image_in, Parameters.get_mask_radius(),*/
// mtrace();

 int counter =0;
 Ipp32f (*particledata)[9] = ParticleStatistics(image_localmax, image_bpass_thresh, 
	   parm.get_mask_radius(),parm.get_feature_radius(), counter);
 //mexPrintf("count %d\n",counter);
 // muntrace();   


 if(nlhs>1)
   {
     plhs[1] = mxCreateDoubleMatrix(image_in.get_width(), image_in.get_height(), mxREAL);
     image_bpass_thresh.get_data(plhs[1]);
     if(nlhs>2)
       {
	 plhs[2] = mxCreateDoubleMatrix(image_in.get_width(), image_in.get_height(), mxREAL);
	 image_localmax.get_data(plhs[2]);
	 if(nlhs>3)
	   {
	     plhs[3] = mxCreateDoubleMatrix(image_in.get_width(), image_in.get_height(), mxREAL);
	     image_bpass.get_data(plhs[3]);
	     
	   }
       }
   }


   
 plhs[0] = mxCreateDoubleMatrix(9,counter,mxREAL);
 double *data_out = mxGetPr(plhs[0]);
 Ipp32f *tmp;
  for(j = 0;j<counter;j++)
   {
     tmp = particledata[j];
     for(i=0;i<9;i++)
       {
	 data_out[j*9+i] = (double)tmp[i];

       }
   }

  delete particledata;
  particledata = NULL;

//  delete &image_in;
//  delete &image_bpass;
//  delete &image_bpass_thresh;
//  delete &image_localmax;
//
//  image_in =NULL;
//  image_bpass=NULL;
//  image_bpass_thresh=NULL;
//  image_localmax=NULL;
//



// 	/*//print out results if in testmode */
// 	string filename = Parameters.get_outfilestem() + make_file_number(framenumber,stacknumber);
// /*//	cout << filename + ": " << counter << " total particles located." << endl;*/

// 	if(Parameters.get_testmode() == 1) {	
// 		/*//print out all images*/
// 		PrintOutputImages(image_in, image_bpass_thresh, image_localmax, Parameters, framenumber, stacknumber);
// 	}

// 	if(Parameters.get_testmode() == 2 && (stacknumber) % 100 == 1) {
// 		/*//print out images for stack 1, 101, 201, etc.*/
// 		PrintOutputImages(image_in, image_bpass_thresh, image_localmax, Parameters, framenumber, stacknumber);
// 	}
// 	PrintParticleData(files.outdatafile, particledata, 0, counter, framenumber, stacknumber);	

// 	if(Parameters.get_testmode() == 3) {
// 		/*//print the number of particles found in a 2D slice*/
// 		cout << framenumber << "\t" << counter << endl;
// 	}

// 	delete [] particledata;
// 	particledata = NULL;


   
//     //copy image back to matlab form
    

/*Ipp32f x[5*5]={0};*/
//    Ipp32f *x;
//    x = ippiMalloc_32f_C1(m,n,&step);
//    IppiSize roi = {2,3};
//    
//    for(j=0;j<m*n;j++)
//      {
//	x[j%m+(j/m)*step/4] = data1[j];
//      }
//    ippiSet_32f_C1R(-1,x+3*step/4 + 2,step,roi);
//    roi.width = n-2;
//    ippiAddRandUniform_Direct_32f_C1IR(x,step,roi,1,3,(uint*)&j);
//    /* Create a pointer to the output data */
//  
//    
//    
//    /* Put data in the output array */
//    for (j = 0; j < m*n; j++)
//      {
//	data2[j] = (double)x[j%m +(j/m)*step/4 ];
//      }
//    ippiFree(x);
//    //mexPrintf("the step is %d \n",step);
  
 
 
}
