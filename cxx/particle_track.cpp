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
#include "particle.h"
#include "track_box.h"
#include <typeinfo>
#include <iostream>
#include "exception.h"
#include "touple.h"
using namespace tracking;
using std::set;
using utilities::Ll_range_error;
/*
particle_track::particle_track(wrapper_i_base * i_data, 
			       wrapper_o_base* o_out, int i_ind, 
			       track_box* i_track)
  :particle_base(i_data,  o_out,i_ind),next(NULL),prev(NULL),track(i_track)
  ,n_pos_link(NULL),p_pos_link(NULL){



}
*/
particle_track::particle_track(wrapper_i_base * i_data, 
			       wrapper_o_base* o_data, 
			       int i_ind,  
			       set<wrapper::p_vals>* contents_in)
  :particle_base(i_data,o_data,i_ind,contents_in),
   next(NULL),
   prev(NULL),
   track(NULL) ,
   n_pos_link(NULL),
   p_pos_link(NULL){

}



void particle_track::print_t(int more){
  particle_base::print();
  if(more-->0&&next!=NULL)
    next->print_t(more);
  
}

void particle_track::print(){
  particle_base::print();

}


void particle_track::set_next(particle_track* n_next){
  if(next!=NULL)
    throw "nuking the list";
  next = n_next;
  forward_disp_[0] = (n_next -> get_value(wrapper::d_xpos))
    - get_value(wrapper::d_xpos);
  forward_disp_[1] = (n_next -> get_value(wrapper::d_ypos))
    - get_value(wrapper::d_ypos);


}


void particle_track::set_prev(particle_track* n_prev){
  if(prev!=NULL)
    throw "nuking the list";
  prev = n_prev;
}


particle_track* particle_track::step_forwards(int n){
  if(n==0)
    return this;

  if (next == NULL)
    throw Ll_range_error("past front");

    
  //check for obo bugs
  return next->step_forwards(--n);
}

particle_track* particle_track::step_backwards(int n){
  if(n==0)
    return this;

  if (prev == NULL)
    throw Ll_range_error("past back");
  
  //check for obo bugs
  return prev->step_backwards(--n);
}

void particle_track::set_track(track_box* i_track){
  if(track!=NULL)
    throw "moving between lists!";
  track = i_track;

}

 track_box* particle_track::get_track(){
  return track;
}

int particle_track::get_track_id(){
  return track->get_id();
}

particle_track::~particle_track(){
  //just to be safe
  delete n_pos_link;
  delete p_pos_link;
  p_pos_link = NULL;
  n_pos_link = NULL;
}


double particle_track::get_value(wrapper::p_vals type){
  if(type == wrapper::d_next){
    if(next==NULL)
      return -1;
    //    return next->get_value(wrapper::d_unqid);
    return next->get_value(wrapper::d_index);
  }
  if(type == wrapper::d_prev){
    if(prev==NULL)
      return -1;
    return prev->get_value(wrapper::d_unqid);
  }
  if(type ==wrapper::d_trackid)
    {
      if(track==NULL)
	return -1;
      return track->get_id() ;
    }
  return particle_base::get_value(type);
}
