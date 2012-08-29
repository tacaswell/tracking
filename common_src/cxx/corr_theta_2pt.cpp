//Copyright 2009-2010 Thomas A Caswell
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

//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

// lifted from http://www.piacrossamerica.org/500pi.html

#include <math.h>
#include "corr_theta_2pt.h"
#include "particle_track.h"
#include "generic_wrapper.h"
#include <stdexcept>
using std::runtime_error;


using std::vector;
using std::string;

using utilities::Generic_wrapper;


using tracking::Corr_theta_2pt;
using tracking::particle;
using utilities::Tuplef;

using utilities::Md_store;

const float PI = acos(-1);


float Corr_theta_2pt::get_max_range()const
{
  return sqrt(max_r_range2_);
}

void Corr_theta_2pt::compute(const particle * p_in,const vector<const particle*> & nhood)
{
    
  unsigned int max_j = nhood.size();
  
//   vector<const particle*>::const_iterator p_end = nhood.end();
//   for(vector<const particle*>::const_iterator cur_part = nhood.begin();
//       cur_part!=p_end;++cur_part)
  for(unsigned int j = 0; j<max_j;++j)
  {
    //    const particle* part_ptr= *curr_part;
    const particle* part_ptr= nhood[j];
    //    if(p_in == nhood[j])
    if(p_in == part_ptr)
      continue;
    
    const Tuplef displacement = p_in->get_disp(part_ptr);
    
    float tmp_d = displacement.magnitude_sqr();
    
    if(tmp_d< max_r_range2_ && tmp_d > min_r_range2_)
    {
      theta_.add_data_point(atan2(displacement[1],displacement[0]));
      
    }
  }
  

  

}



Corr_theta_2pt::Corr_theta_2pt(int t_nbins,float min_r,float max_r):
  theta_(t_nbins,-PI,PI),
  min_r_range2_(min_r*min_r),
  max_r_range2_(max_r*max_r)

{

}

void Corr_theta_2pt::out_to_wrapper(Generic_wrapper & output_wrapper,
			       const std::string & g_name,
			       const utilities::Md_store * md_store)const
{
  bool opened_wrapper = false;
  
  vector<float> tmp;
  

  if(!output_wrapper.is_open())
  {
    output_wrapper.open_wrapper();
    opened_wrapper = true;
    
  }
  
  output_wrapper.open_group(g_name);
    
  if(md_store)
    output_wrapper.add_meta_data(md_store);
  
  
  output_wrapper.add_meta_data("max_r_range",(float)sqrt(max_r_range2_));
  output_wrapper.add_meta_data("min_r_range",(float)sqrt(min_r_range2_));
  output_wrapper.add_meta_data("plane_count",plane_count_);
  output_wrapper.add_meta_data("temperature",temperature_sum_/plane_count_);
  
  // strings for controlling where the histograms write out to
  const string value_str = "count";
  const string edge_str  = "edges";

  theta_.output_to_wrapper(&output_wrapper,g_name+"/theta",value_str,edge_str,
     				 (const Md_store*)NULL);  
  output_wrapper.close_group();
  
  if(opened_wrapper)
    output_wrapper.close_wrapper();

}
  
