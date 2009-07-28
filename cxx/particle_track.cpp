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
#include "particle_base.h"
#include "particle_track.h"
#include "track_box.h"
#include <typeinfo>
#include <iostream>
#include "exception.h"
#include "tuple.h"
#include "hash_shelf.h"
using namespace tracking;
using std::set;
using utilities::Ll_range_error;
using utilities::Null_field;
/*
particle_track::particle_track(wrapper_i_base * i_data, 
			       wrapper_o_base* o_out, int i_ind, 
			       track_box* i_track)
  :particle_base(i_data,  o_out,i_ind),next(NULL),prev(NULL),track(i_track)
  ,n_pos_link(NULL),p_pos_link(NULL){



}
*/
particle_track::particle_track(int i_ind)
  :particle_base(i_ind),
   next_(NULL),
   prev_(NULL),
   track(NULL) ,
   shelf_(NULL),
   n_pos_link(NULL),
   p_pos_link(NULL){

}

particle_track::particle_track(int i_ind,utilities::Tuple pos,unsigned int frame)
  :particle_base(i_ind,pos),
   next_(NULL),
   prev_(NULL),
   track(NULL) ,
   shelf_(NULL),
   n_pos_link(NULL),
   p_pos_link(NULL)
{
}



void particle_track::print_t(int more)const{
  particle_base::print();
  if(more-->0&&next_!=NULL)
    next_->print_t(more);
  
}

// void particle_track::print()const{
//   cout<<"'";
//   particle_base::print();


// }


void particle_track::set_next(particle_track* n_next){
  if(next_!=NULL)
    throw "nuking the list";
  next_ = n_next;
//   forward_disp_[0] = (n_next -> get_value(wrapper::d_xpos))
//     - get_value(wrapper::d_xpos);
//   forward_disp_[1] = (n_next -> get_value(wrapper::d_ypos))
//     - get_value(wrapper::d_ypos);
  forward_disp_ = (n_next -> get_position()) - position_;
  

}


void particle_track::set_prev(particle_track* n_prev){
  if(prev_!=NULL)
    throw "nuking the list";
  prev_ = n_prev;
}


const particle_track* particle_track::step_forwards(int n)const{
  if(n==0)
    return this;

  if (next_ == NULL)
    throw Ll_range_error("past front");

    
  //check for obo bugs
  return next_->step_forwards(--n);
}

const particle_track* particle_track::step_backwards(int n)const{
  if(n==0)
    return this;

  if (prev_ == NULL)
    throw Ll_range_error("past back");
  
  //check for obo bugs
  return prev_->step_backwards(--n);
}

bool particle_track::step_forwards(int n, const particle_track* & dest)const{

  if (next_ == NULL)
  {
    dest = NULL;
    return false;
  }

  if(n==1)
  {
    dest = next_;
    return true;
  }
  if(n<1)
  {
    dest = this;
    return true;
  }
  
  
//check for obo bugs
  return next_->step_forwards(--n,dest);
}


bool particle_track::step_backwards(int n, const particle_track* & dest)const{

  if (next_ == NULL)
  {
    dest = NULL;
    return false;
  }

  if(n==1)
  {
    dest = next_;
    return true;
  }
  if(n<1)
  {
    dest = this;
    return true;
  }
    
  
  //check for obo bugs
  return next_->step_backwards(--n,dest);
}


void particle_track::set_track(track_box* i_track){
  if(track!=NULL)
    throw "moving between lists!";
  track = i_track;

}

track_box* particle_track::get_track()const{
  if(track ==NULL)
  {
    throw Null_field("Particle does not have a track");   
  }
  return track;
}


int particle_track::get_track_id()const{
  if (track ==NULL)
  {
    throw Null_field("Particle does not have a track");   
  }
  return track->get_id();
}

particle_track::~particle_track(){
  //just to be safe
  delete n_pos_link;
  delete p_pos_link;
  p_pos_link = NULL;
  n_pos_link = NULL;
}


double particle_track::get_value(wrapper::p_vals type) const{
  if(type == wrapper::d_next){
    if(next_==NULL)
      return -1;
    //    return next->get_value(wrapper::d_unqid);
    return next_->get_value(wrapper::d_index);
  }
  if(type == wrapper::d_prev){
    if(prev_==NULL)
      return -1;
    // tac 2009-04-10
    // changed to match the other one
    return prev_->get_value(wrapper::d_index);
  }
  if(type ==wrapper::d_trackid)
    {

      if(track==NULL)
	return -1;
      return track->get_id() ;
      
    }
  
  // tac 2009-07-17
  // added special cases to avoid call to wrappers
  if(type ==wrapper::d_xpos)
  {
    return position_[0];
  }
  if(type == wrapper::d_ypos)
  {
    return position_[1];
  }
  if(type == wrapper::d_frame)
  {
    return frame_;
  }
  else
  {
    return particle_base::get_value(type);
  }
  
}


double particle_track::distancesq_corrected(const particle_track* part_in)const{

  if (shelf_ ==NULL)
    {
      throw "shelf not defined";
      return 0;
    }

  return (
	  (position_ - shelf_->get_cum_forward_disp()) 
	  - (
	     (part_in->position_ )  - ((part_in->shelf_)->get_cum_forward_disp()))
	  ).magnitude_sqr();

  //   double X =get_value(wrapper::d_xpos) - part_in->get_value(wrapper::d_xpos);
  //   double Y =get_value(wrapper::d_ypos) - part_in->get_value(wrapper::d_ypos);
  //   //  double Z =get_value(wrapper::d_zpos) - part_in->get_value(wrapper::d_zpos);
  //   return X*X + Y*Y ;//+ Z*Z;
}

 const utilities::Tuple particle_track::get_corrected_forward_disp()const
{
  if (shelf_ ==NULL)
    {
      throw "shelf not defined";
      return utilities::Tuple();
    }
  return forward_disp_ - shelf_->get_mean_forward_disp();
}

void particle_track::clear_track_data(){
  next_ = NULL;
  prev_ = NULL;
  track = NULL;
}

bool particle_track::has_track()const
{
  if(track == NULL)
  {
    return false;
  }
  return true;
}

bool particle_track::has_next()const
{
  if(next_ == NULL)
  {
    return false;
  }
  return true;
}
  
const utilities::Tuple particle_track::get_corrected_pos()const
{
  if (shelf_ ==NULL)
    {
      throw "shelf not defined";
      return utilities::Tuple();
    }
  return position_ - shelf_->get_cum_forward_disp();
}
