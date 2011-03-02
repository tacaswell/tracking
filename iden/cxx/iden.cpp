//Copyright 2008-2010 Thomas A Caswell
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
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "boost/date_time/posix_time/posix_time.hpp"


#include "iden.h"
#include "wrapper_i_plu.h"

#include "image1.h"
#include "data_proc1.h"

#include "ipp.h"


#include "tuple.h"

#include "image_base.h"


#include "md_store.h"
using std::string;


using std::cout;
using std::endl;


using tracking::hash_case;

using iden::Iden;
using iden::Image2D;

using utilities::Wrapper_i_plu;
using utilities::Tuple;


using std::runtime_error;

using utilities::Md_store;

using namespace boost::posix_time;
using namespace boost::gregorian;

Wrapper_i_plu * Iden::fill_wrapper(unsigned int frames,unsigned int start)
{
  
  
  Wrapper_i_plu * wrapper = NULL;
  unsigned int img_frames = img_src_->get_frame_count();

  if(start > img_frames)
    throw runtime_error("Iden: start is larger than the number of frames in image");
  
  if(start < 0)
    throw runtime_error("Iden: start is negetive, wtf");
  

  if(frames == 0)
  {
    frames = img_frames-start;
  }
  else if(start + frames > img_frames)
  {
    cout<<"start "<<start<<endl;
    cout<<"frames "<<frames<<endl;
    cout<<"img_frames"<<img_frames<<endl;
    
    throw runtime_error("Iden: asking for more frames than the stack has");
  }
  
  img_src_->select_plane(start);
  

  
  WORD scan_step = img_src_->get_scan_step();
  Tuple<unsigned int,2> dims = img_src_->get_plane_dims();
  unsigned int cols = dims[0];
  unsigned int rows = dims[1];
  
  
  
  // create a wrapper
  wrapper = new Wrapper_i_plu(frames,dims);
  wrapper->set_Md_store_size(frames);  

  // set up parser for meta-morph tiffs
  
  ptime prev_time,cur_time;
  string time_str;
  
  // loop over frames
  for(unsigned int j = start;j<(frames+start);++j)
  {
    

    // load frame
    img_src_->select_plane(j);
    
    // get data about image
    const WORD * data_ptr = img_src_->get_plane_pixels();
    
    
    // shove data in to image object
    Image2D image_in(rows,cols);
    image_in.set_data(data_ptr, rows,cols,scan_step);

    
    // trim off the top .1% of the pixels to deal with outliers
    ///TODO make this a parameter
    image_in.trim_max(params_.get_top_cut());
    
    
    // extract meta data
    // this does not need to be cleaned up, that will be handled by the wrapper
    Md_store * md_store = img_src_->get_plane_md();
    int dtime;
    
    // deal with time
    if(j == start)
    {
      prev_time = time_from_string(md_store->get_value("acquisition-time-local",time_str));
      dtime = 0;
    }
    
    else
    {
      cur_time  = time_from_string(md_store->get_value("acquisition-time-local",time_str));
      dtime = (cur_time-prev_time).total_milliseconds();
      prev_time = cur_time;
    }
    md_store->add_element("dtime",dtime);
    
    
    

    
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
			     params_.get_pctle_threshold(),
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

    // set data in wrapper
    wrapper->add_frame_data(particledata,j-start,counter);
    // set metadata in wrapper
    // the wrapper takes responsibility for the object
    wrapper->set_Md_store(j-start,md_store);
    //    cout<<j<<endl;
    // clean up wrapper
    
    
  }
  
  
  return wrapper;
  

}

void Iden::set_params(const Params & param_in)
{
  params_ = Params(param_in);
}
void Iden::set_image_src(utilities::Image_base * img_src)
{
  img_src_ = img_src;
  

}

Wrapper_i_plu * Iden::fill_wrapper_avg(unsigned int avg_count,unsigned int frames,unsigned int start)
{
  
  Wrapper_i_plu * wrapper = NULL;
  unsigned int img_frames = img_src_->get_frame_count();


  if(start > img_frames)
    throw runtime_error("Iden: start is larger than the number of frames in image");

  if(frames == 0)
  {
    frames = img_frames-start;
  }
  else if(start + frames > img_frames)
  {
    cout<<"start "<<start<<endl;
    cout<<"frames "<<frames<<endl;
    cout<<"img_frames"<<img_frames<<endl;
    
    
    throw runtime_error("Iden: asking for more frames than the stack has");
  }

  
  if(avg_count > frames)
    throw runtime_error("Iden: avg_count is greater than the number for frames");
  

 


  unsigned int wrapper_frames = frames/avg_count;
  
  
  img_src_->select_plane(start);
  

  WORD scan_step = img_src_->get_scan_step();
  Tuple<unsigned int,2> dims = img_src_->get_plane_dims();
  unsigned int cols = dims[0];
  unsigned int rows = dims[1];
  
  
  // create the wrapper
  wrapper = new Wrapper_i_plu(wrapper_frames,dims);
  wrapper->set_Md_store_size(wrapper_frames);

  
  float scx=0,scy=0;
  string exp_unit,cal_units;
  bool cal_state = false;
  ptime prev_time,cur_time;

  // loop over frames
  for(unsigned int j = 0;j<wrapper_frames;++j)
  {
    float exposure_sum = 0,x = 0,y = 0,z=0,tmp = 0;
    
    
    
    string time_str;
    unsigned int dtime=0;
    
    

    Image2D image_in(rows,cols);
    for(unsigned int k = 0; k<avg_count;++k)
    {
      // load frame
      img_src_->select_plane(j*avg_count + k +start);
            
      // get data about image
      const WORD * data_ptr = img_src_->get_plane_pixels();
      
      // shove data in to image object
      image_in.add_data(data_ptr, rows,cols,scan_step);
      
      // extract meta data from tiff
      Md_store * md_store = img_src_->get_plane_md();

        
      // deal with time
      if(k ==0)
	cur_time  = time_from_string(md_store->get_value("acquisition-time-local",time_str));
    
      exposure_sum += md_store->get_value("Exposure",tmp);
      if(j == 0)
      {
	md_store->get_value("spatial-calibration-x",scx);
	md_store->get_value("spatial-calibration-y",scy);
	md_store->get_value("Exposure units",exp_unit);
	md_store->get_value("spatial-calibration-state",cal_state);
	md_store->get_value("spatial-calibration-units",cal_units);
	prev_time = cur_time;
	
      }
      
      
     
  
      x += md_store->get_value("stage-position-x",tmp);
      y += md_store->get_value("stage-position-y",tmp);      
      z += md_store->get_value("z-position",tmp);  
      // clean up meta data from tiff
      delete md_store;
      

    }
    dtime = (cur_time-prev_time).total_milliseconds();

    prev_time = cur_time;
    
    
    // average 
    x /=avg_count;
    y /=avg_count;
    z /=avg_count;

    Md_store *avg_md_store = new Md_store();
    avg_md_store->add_element("Exposure",exposure_sum);
    avg_md_store->add_element("Exposure units","string",exp_unit.c_str());
    avg_md_store->add_element("stage-position-x",x);
    avg_md_store->add_element("stage-position-y",y);      
    avg_md_store->add_element("z-position",z);  
    avg_md_store->add_element("spatial-calibration-state",cal_state);
    avg_md_store->add_element("spatial-calibration-units","string",cal_units.c_str());
    avg_md_store->add_element("spatial-calibration-x",scx);
    avg_md_store->add_element("spatial-calibration-y",scy);
    avg_md_store->add_element("pixel-size-x",cols);
    avg_md_store->add_element("pixel-size-y",rows);
    avg_md_store->add_element("dtime",dtime);
    avg_md_store->add_element("acquisition-time-local","string",time_str.c_str());
    
    
    
    // object for holding band passed image
    Image2D image_bpass(rows,cols);
    // object for holding the thresholded band passed image
    Image2D image_bpass_thresh(rows,cols);
    // object that holds the local max
    Image2D image_localmax(rows,cols);

    
    // trim off the top fraction of pixels
    image_in.trim_max(params_.get_top_cut());

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
    
    wrapper->add_frame_data(particledata,j-start,counter);
    wrapper->set_Md_store(j-start,avg_md_store);
    

    
    
    //    cout<<j<<endl;
  }
  
  return wrapper;
  

}
