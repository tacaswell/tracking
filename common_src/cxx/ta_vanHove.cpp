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

#include "ta_vanHove.h"
#include "md_store.h"
#include "generic_wrapper.h"

using std::runtime_error;
using std::string;
using std::cout;
using std::endl;


using utilities::format_name;
using utilities::Tuplef;
using utilities::Generic_wrapper;
using utilities::Md_store;
using utilities::Histogram;

using tracking::TA_vanHove;



void TA_vanHove::add_disp(Tuplef displacement,unsigned step)
{
  // check that steps is less than max steps
  if(step > max_step_)
    throw runtime_error("TA_vanHove::add_disp step out of range");
  

  // add x to xdisp
  x_disp_[step-1].add_data_point(displacement[0]);
  // add y to ydips
  y_disp_[step-1].add_data_point(displacement[1]);
}


TA_vanHove::TA_vanHove(unsigned max_step,
		       unsigned nbins,
		       float max_range):
  x_disp_(max_step,Histogram(nbins,-max_range,max_range)),
  y_disp_(max_step,Histogram(nbins,-max_range,max_range)),
  max_step_(max_step)
{
  

}

TA_vanHove::~TA_vanHove()
{
}


  
void TA_vanHove::output_to_wrapper(utilities::Generic_wrapper & out,
				   const Md_store & md_store)const
{
  // strings for controlling where the histograms write out to
  const string value_str = "disp_count";
  const string edge_str  = "disp_edges";


  // format the base name

  // the MD_store MUST contain write_comp
  int write_comp_num;
  md_store.get_value("write_comp",write_comp_num);
  string base_name = format_name("vanHove",write_comp_num);

  // make sure that the wrapper is open
  bool opened_wrapper = false;
  
  if(!out.is_open())
  {
    cout<<"opening wrapper"<<endl;
    out.open_wrapper();
    opened_wrapper = true;
  }

  


  // make the top level group
  out.open_group(base_name);
  // add meta data to it
  out.add_meta_data(&md_store);
  // close the group
  out.close_group();
  
  for(unsigned j = 0;j<max_step_;++j)
  {
    // format the group for this step
    string g_name = base_name +format_name("/step",j+1);
    // create the top level group for this step
    //
    // Doing it this way is needed because of the way that the
    // histogram class deals with groups, shoving MD in to the group
    // directly above where the data sets are saved.
    out.open_group(g_name);
    // close the group
    out.close_group();
  
    
    // output the two histograms
    x_disp_[j].output_to_wrapper(&out,g_name+"/x",value_str,edge_str,
    				 (const Md_store*)NULL);
    y_disp_[j].output_to_wrapper(&out,g_name+"/y",value_str,edge_str,
     				 (const Md_store*)NULL);
  }
  //if opened the wrapper, close it
  if(opened_wrapper)
    out.close_wrapper();
    

}

  
