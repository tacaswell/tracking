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
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting


#include "corr_case.h"
#include "generic_wrapper.h"
#include "corr_gofr.h"
#include "enum_utils.h"
#include "md_store.h"
using utilities::Generic_wrapper;
using utilities::Md_store;

using std::string;
using std::vector;
using std::map;
using std::pair;

using tracking::Corr;
using tracking::Corr_case;




Corr_case::Corr_case(const tracking::Corr_gofr*,
		     int comp_count, 
		     float max_range, 
		     const int n_bins,int comp_num,int dset_num,int read_comp_num):
  corr_vec_(comp_count)
{
  
  for(int j = 0; j<comp_count;j++)
    corr_vec_[j] = new Corr_gofr(n_bins,max_range,comp_num,dset_num,read_comp_num);
  
  prams_float_.insert(pair<string,float>("max_range",max_range));
  prams_int_.insert(pair<string,int>("nbins",n_bins));
  prams_int_.insert(pair<string,int>("dset",dset_num));
  prams_int_.insert(pair<string,int>("read_comp",read_comp_num));
  

}


Corr_case::~Corr_case()
{
  for(unsigned int j = 0; j<corr_vec_.size();j++)
  {
    delete corr_vec_[j];
    corr_vec_[j] = NULL;
  }
}


void Corr_case::out_to_wrapper(Generic_wrapper & in,
			       const string & base_name,
			       const Md_store * md_store)const
{

  std::cout<<"outputting the whole case"<<std::endl;
  
  in.open_wrapper();
  in.open_group(base_name);
  // adding top level meta-data
  for(map<string,float>::const_iterator it = prams_float_.begin();
      it != prams_float_.end();++it)
    in.add_meta_data((*it).first,(*it).second);
  for(map<string,int>::const_iterator it = prams_int_.begin();
      it != prams_int_.end();++it)
    in.add_meta_data((*it).first,(*it).second);
  if(md_store)
    in.add_meta_data(md_store);
  
  in.close_group();
  
  for(unsigned int j = 0; j<corr_vec_.size();++j)
  {
    string g_name = base_name + "/" + utilities::format_name("comp",j);
    
    corr_vec_[j]->out_to_wrapper(in,g_name,md_store);
    

  }
  in.close_wrapper();
  
  
}
