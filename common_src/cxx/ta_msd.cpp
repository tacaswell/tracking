//Copyright 2010 Thomas A Caswell
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
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

#include "ta_msd.h"
#include "md_store.h"
#include "generic_wrapper.h"

using std::runtime_error;
using std::string;
using std::cout;
using std::endl;

using std::vector;

using utilities::format_name;
using utilities::Tuplef;
using utilities::Generic_wrapper;
using utilities::Md_store;
using utilities::Histogram;

using tracking::TA_msd;



void TA_msd::add_disp(Tuplef displacement,unsigned step)
{
  // check that steps is less than max steps
  if(step > max_step_)
    throw runtime_error("TA_msd::add_disp step out of range");
  
  summed_displacement_[step-1] += displacement.magnitude();
  ++(segment_count_[step-1]);
  
}


TA_msd::TA_msd(unsigned max_step):
  summed_displacement_(max_step,0),
  segment_count_(max_step,0),
  max_step_(max_step)
{}

TA_msd::~TA_msd()
{}


  
void TA_msd::output_to_wrapper(utilities::Generic_wrapper & out_wrapper,
				   const Md_store & md_store)const
{

  int comp_key;
  comp_key = md_store.get_value("comp_key",comp_key);
  // strings for controlling where the histograms write out to
  const string msd_str = "msd";
  const string seg_str  = "seg_count";
  const string g_name = format_name("mean_squared_disp",comp_key);
  

    // house keeping
  bool opened_wrapper = false;
  if(!out_wrapper.is_open())
  {
    out_wrapper.open_wrapper();
    opened_wrapper = true;
    
  }
  unsigned int n_bins = summed_displacement_.size();
  
  // open group
  out_wrapper.open_group(g_name);
  // add meta data
  out_wrapper.add_meta_data(&md_store);
  
  // shove in real data
  vector<float> tmp_mean(max_step_);
  for (unsigned int j = 0; j<max_step_;++j)
    tmp_mean[j] = summed_displacement_[j]/segment_count_[j];
  
  const float * tmpf = &(tmp_mean.front());
  out_wrapper.add_dset(1,&n_bins,utilities::V_FLOAT,tmpf,msd_str);
  const unsigned int * tmpi = &(segment_count_.front());
  out_wrapper.add_dset(1,&n_bins,utilities::V_INT,tmpi,seg_str);
  
  
  
  // close group
  out_wrapper.close_group();

  
  // house keeping
  if(opened_wrapper)
    out_wrapper.close_wrapper();
  

}

  
