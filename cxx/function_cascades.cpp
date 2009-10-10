//Copyright 2009 Thomas A Caswell
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
//see website for additional permissions under GNU GPL version 3 section 7
#include<iostream>
#include<cmath>

#include "hash_case.h"
#include "hash_shelf.h"
#include "hash_box.h"
#include "particle_base.h"
#include "wrapper_o.h"
#include "corr.h"

using tracking::hash_box;
using tracking::hash_shelf;
using tracking::hash_case;
using tracking::particle_base;

using utilities::Wrapper_out;
using tracking::Corr;

using std::set;
using std::vector;
using std::cout;
using std::endl;
using std::ceil;



void hash_case::output_to_wrapper(Wrapper_out & wrapper) const
{

  wrapper.initialize_wrapper();
  for(vector<hash_shelf*>::const_iterator current_shelf= h_case_.begin();
      current_shelf!=h_case_.end();++current_shelf)				// loop over frames
  {
    (*current_shelf)->output_to_wrapper(wrapper);
  }
  wrapper.finalize_wrapper();


}
void hash_shelf::output_to_wrapper(Wrapper_out & wrapper) const
{
  cout<<"frame "<<plane_number_<<"contains ";
  
  cout<<particle_count_<<"particles"<<endl;
  
  wrapper.open_frame(plane_number_,particle_count_);
  for(vector<hash_box*>::const_iterator current_box = hash_.begin();
      current_box != hash_.end();++current_box)
  {
    (*current_box)->output_to_wrapper(wrapper);
  }
  wrapper.close_frame();
}
void hash_box::output_to_wrapper(Wrapper_out & wrapper) const
{

  for(vector<particle_base*>::const_iterator current_part = contents_.begin();
      current_part!=contents_.end();++current_part)
  {
    wrapper.set_all_values(*current_part);
  }
}







void hash_case::pass_fun_to_part(void(particle_base::*fun)())
{
  vector<hash_shelf*>::iterator myend =  h_case_.end();
  for(vector<hash_shelf*>::iterator it = h_case_.begin();
      it!=myend;++it)
  {
    (*it)->pass_fun_to_part(fun);
    
  }
}
void hash_shelf::pass_fun_to_part(void(particle_base::*fun)())
{
  vector<hash_box*>::iterator myend =  hash_.end();
  for(vector<hash_box*>::iterator it = hash_.begin();
      it!=myend;++it)
  {
    (*it)->pass_fun_to_part(fun);
    
  }
}
void hash_box::pass_fun_to_part(void(particle_base::*fun)())
{
  vector<particle_base*>::iterator myend = contents_.end();
  for(vector<particle_base*>::iterator it = contents_.begin();
      it!=myend;++it)
  {
    ((*it)->*fun)();
  }
}








void hash_case::pass_fun_to_part(void(particle_base::*fun)()const)const
{
  vector<hash_shelf*>::const_iterator myend =  h_case_.end();
  for(vector<hash_shelf*>::const_iterator it = h_case_.begin();
      it!=myend;++it)
  {
    (*it)->pass_fun_to_part(fun);
    
  }
}
void hash_shelf::pass_fun_to_part(void(particle_base::*fun)()const)const
{
  vector<hash_box*>::const_iterator myend =  hash_.end();
  for(vector<hash_box*>::const_iterator it = hash_.begin();
      it!=myend;++it)
  {
    (*it)->pass_fun_to_part(fun);
    
  }
}
void hash_box::pass_fun_to_part(void(particle_base::*fun)()const)const
{
  vector<particle_base*>::const_iterator myend = contents_.end();
  for(vector<particle_base*>::const_iterator it = contents_.begin();
      it!=myend;++it)
  {
    ((*it)->*fun)();
  }
}






void hash_case::pass_fun_to_shelf(void(hash_shelf::*fun)())
{
  vector<hash_shelf*>::iterator myend =  h_case_.end();
  for(vector<hash_shelf*>::iterator it = h_case_.begin();
      it!=myend;++it)
  {
    ((*it)->*fun)();
    
  }
}
void hash_case::pass_fun_to_shelf(void(hash_shelf::*fun)()const)const
{
  vector<hash_shelf*>::const_iterator myend =  h_case_.end();
  for(vector<hash_shelf*>::const_iterator it = h_case_.begin();
      it!=myend;++it)
  {
    ((*it)->*fun)();
    
  }
}





void hash_case::compute_corr(Corr & in) const
{
  
  for(vector<hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
    (*shelf_it)->compute_corr(in);
    //    cout<<"finished shelf"<<endl;
  }
}

void hash_shelf::compute_corr(Corr & in)const
{

  float range = in.get_max_range();
  unsigned int buffer = (unsigned int)((int)range%(int)ppb_==0?range/ppb_:(1+range/ppb_));
  
  // the hack to have asymmetric buffers is here
  for(unsigned int x = buffer; x<(hash_dims_[0]-buffer-1);++x){
    for(unsigned int y = buffer; y<(hash_dims_[1]-buffer-1);++y){
      get_box(x,y)->compute_corr(in);
      
    }
  }

}
void hash_box::compute_corr(Corr & in )const
{

  if(in.get_max_range()<particle_base::get_neighborhood_range())
  {
    vector<particle_base*>::const_iterator myend = contents_.end();
    for(vector<particle_base*>::const_iterator it = contents_.begin();
	it!=myend;++it)
    {
      in.compute(*it,(*it)->get_neighborhood () );
    }
  }
  else
  {
    
    

    if(shelf_ ==NULL || hash_indx_ == -1)
      throw "hash_box: box not part of a shelf";

    vector <const particle_base *> nhood;
    shelf_->get_region(hash_indx_,nhood,(int)ceil(in.get_max_range()));

    
    // vector<particle_base*>::const_iterator myend = contents_.end();
    //     for(vector<particle_base*>::const_iterator it = contents_.begin();
    // 	it!=myend;++it)
    int max_j = contents_.size();
    for(int j = 0; j<max_j;++j)
    {
      in.compute(contents_[j],nhood);
    }
  }
}




void hash_case::fill_in_neighborhood() 
{
  
  for(vector<hash_shelf*>::iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
    (*shelf_it)->fill_in_neighborhood();
  }
}

