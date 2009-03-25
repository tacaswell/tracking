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
#include "track_box.h"
#include "particle_track.h"
#include "array.h"
using namespace tracking;
using utilities::Array;
int track_box::running_count = 0;

track_box::track_box(particle_track * first){
  t_first_ = first;
  t_last_ = first;

  id = running_count++;
  if(first != NULL){
    first->set_track(this);
    length_ = 1;
  }
}

void track_box::print(){
  t_first_->print_t(length_);
}

const particle_track* track_box::at(int n)const{
  if (n>length_)
    return NULL;
  if(n<length_/2)
    return t_first_->step_forwards(n);
  else
    return t_last_->step_backwards(n);

}


void track_box::push_back(particle_track * next){
  if( next==NULL)
    return;

  //use the level of indrection so that we don't
  //have to reimplement the sanity checking
  //at this point
  t_last_->set_next(next);
  next->set_prev(t_last_);
  next->set_track(this);
  t_last_ = next;
  //increment last incase something goes wrong
  ++length_;
}


//this needs some sanity checking/error handling
void track_box::set_track(){
  particle_track * current_particle = t_first_;
  
  while(current_particle !=NULL){
    current_particle -> set_particle();
    current_particle = current_particle->get_next();
  }

    
}

void track_box::extract_raw_disp(Array & output) const{
  output.clear(length_);
  particle_track* working_part = t_first_;
  for(int j = 0; j<length_;++j)
    {
      output.push((working_part->get_forward_disp()));
      working_part = working_part->get_next();
    }
  
}
