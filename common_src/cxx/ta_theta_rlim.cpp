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
#include <math.h>
#include "ta_disp.h"
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

using tracking::TA_disp;

// lifted from http://www.piacrossamerica.org/500pi.html

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198


void TA_disp::add_disp(const Tuplef & displacement,unsigned step)
{
  // check that steps is less than max steps
  if(step > max_step_)
    throw runtime_error("TA_disp::add_disp step out of range");
  

  // add magnitude
  r_disp_[step-1].add_data_point(displacement.magnitude());
  // add angle
  theta_[step-1].add_data_point(atan2(displacement[1],displacement[0]));
  
}


TA_disp::TA_disp(unsigned max_step,
                       unsigned r_nbins,
                       float max_r,
                       unsigned t_nbins):

  r_disp_(max_step,Histogram(r_nbins,0,max_r)),
  theta_(max_step,Histogram(t_nbins,-PI,PI)),
  max_step_(max_step)
{
}

TA_disp::~TA_disp()
{
}


  
void TA_disp::output_to_wrapper(utilities::Generic_wrapper & out,
				   const Md_store & md_store)const
{
  // strings for controlling where the histograms write out to
  const string value_str = "count";
  const string edge_str  = "edges";


  // format the base name

  // the MD_store MUST contain write_comp
  int comp_key;
  md_store.get_value("comp_key",comp_key);
  string base_name = format_name("disp",comp_key);

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
    r_disp_[j].output_to_wrapper(&out,g_name+"/r2",value_str,edge_str,
    				 (const Md_store*)NULL);
    theta_[j].output_to_wrapper(&out,g_name+"/theta",value_str,edge_str,
     				 (const Md_store*)NULL);
  }
  //if opened the wrapper, close it
  if(opened_wrapper)
    out.close_wrapper();
    

}

  
