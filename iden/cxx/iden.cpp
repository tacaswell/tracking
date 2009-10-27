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
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting
//work.


#include "iden.h"
#include "wrapper_i_plu.h"
#include "image1.h"
#include "data_proc1.h"

#include "ipp.h"
#include "FreeImagePlus.h"

using std::string;

#include <iostream>
using std::cout;
using std::endl;


using tracking::hash_case;

using iden::Iden;
using iden::Image2D;

using utilities::Wrapper_i_plu;
using utilities::Pair;



void Iden::fill_wrapper(Wrapper_i_plu & wrapper, Pair dims,int frames,int start)
{


  // load multi page
  
  FreeImage_Initialise();
  BOOL bMemoryCache = TRUE;
  fipMultiPage src(bMemoryCache);
  // Open src file (read-only, use memory cache)
  src.open(fname_.c_str(), FALSE, TRUE);
  int img_frames = src.getPageCount();

  if(start > img_frames)
    throw "Iden: start is larger than the number of frames in image";
  
  if(start < 0)
    throw "Iden: start is negetive, wtf";
  

  if(frames == 0)
  {
    frames = img_frames-start;
  }
  else if(start + frames > img_frames)
  {
    cout<<"start "<<start<<endl;
    cout<<"frames "<<frames<<endl;
    cout<<"img_frames"<<img_frames<<endl;
    
    src.close(0);
    throw "Iden: asking for more frames than the stack has";
  }
  
  
  // replace the wrapper with an entirely new one
  wrapper = Wrapper_i_plu(frames,dims);

  fipImage image;
  
  // step in bytes on input image
 
  // loop over frames
  for(int j = start;j<(frames+start);++j)
  {
    

    // load frame
    image = src.lockPage(j);
    // get data about image
    WORD * data_ptr = (WORD *) image.accessPixels();
    WORD scan_step = image.getScanWidth();
    int rows = image.getHeight();
    int cols = image.getWidth();


    


    // shove data in to image object
    Image2D image_in(rows,cols);
    image_in.set_data(data_ptr, rows,cols,scan_step);

    // clear the input data
    src.unlockPage(image,false);

    
    // object for holding band passed image
    Image2D image_bpass(rows,cols);
    // object for holding the thresholded band passed image
    Image2D image_bpass_thresh(rows,cols);
    // object that holds the local max
    Image2D image_localmax(rows,cols);


    RecenterImage(image_in);

    IppStatus status;
    
    status = BandPass_2D(image_in,
			 image_bpass,
			 params_.get_feature_radius(),
			 params_.get_hwhm_length());


    
    status = FindLocalMax_2D(image_bpass,
			     image_bpass_thresh,
			     image_localmax,
			     (int)params_.get_pctle_threshold(),
			     params_.get_dilation_radius());
    

    
    RecenterImage(image_bpass_thresh);
    // image_in.display_image();
    //     image_bpass.display_image();
    //     image_bpass_thresh.display_image();
    //    image_localmax.display_image();
    // get out massive nx9 array
    int counter;
    Ipp32f (*particledata)[9] =
      ParticleStatistics(image_localmax,
			 image_bpass_thresh, 
			 params_.get_mask_radius(),
			 params_.get_feature_radius(),
			 counter);
    
    // put it in a wrapper

    //     cout<<"---------------"<<endl;
    //     cout<<counter<<endl;
    //     cout<<"---------------"<<endl;
    
    wrapper.add_frame_data(particledata,j-start,counter);
    //    cout<<j<<endl;
  }
  
  
  src.close(0);
  
  FreeImage_DeInitialise();


}

void Iden::set_params(const Params & param_in)
{
  params_ = Params(param_in);
}

void Iden::set_fname(const std::string & in)
{
  fname_ = string(in);

  // try to open the file
  // look to see if the file has the meta data for the parameters
}

