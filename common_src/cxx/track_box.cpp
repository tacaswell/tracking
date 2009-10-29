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

#include "part_def.h"

#include "pair.h"

#include "gnuplot_i.hpp"
using namespace tracking;
using utilities::Array;
using utilities::Tuple;

int track_box::running_count_ = 0;

track_box::track_box(particle_track * first){
  t_first_ = first;
  t_last_ = first;

  id_ = running_count_++;
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
// void track_box::set_track() const{
//   const particle_track * current_particle = t_first_;
  
//   while(current_particle !=NULL){
//     current_particle -> output_to_wrapper();
//     current_particle = current_particle->get_next();
//   }
// }

void track_box::extract_raw_disp(Array & output) const{
  output.clear(length_);
  const particle_track* working_part = t_first_;
  for(int j = 0; j<length_;++j)
    {
      output.push((working_part->get_raw_forward_disp()));
      working_part = working_part->get_next();
    }
  
}



void track_box::extract_corrected_disp(Array & output) const{
  output.clear(length_);
  const particle_track* working_part = t_first_;
  for(int j = 0; j<length_;++j)
    {
      output.push((working_part->get_corrected_forward_disp()));
      working_part = working_part->get_next();
    }
  
}

track_box::~track_box(){
  particle_track* cur_part = t_first_;
  particle_track* next_part = NULL;
  while(cur_part != NULL)
    {
      next_part = cur_part->get_next();
      cur_part-> clear_track_data();
      cur_part = next_part;
    }
}


void track_box::plot_intensity() const
{
  std::vector<float> inten_data(length_);
  std::vector<float> x_data(length_);
  std::vector<float> y_data(length_);
  std::vector<float> z_data(length_);
  const particle_track * cur_part = t_first_;
  float x_mean =0,y_mean=0;
  for(int j = 0; j< length_;++j)
  {
    cur_part->get_value(utilities::D_I,inten_data[j]);
    cur_part->get_value(utilities::D_ZPOS,z_data[j]);
    Tuple pos = cur_part->get_position();
    cur_part = cur_part->get_next();
    x_data[j] = pos[0];
    y_data[j] = pos[1];

    
    y_mean +=y_data[j];
    x_mean +=x_data[j];
  }
  y_mean/=length_;
  x_mean/=length_;
  
    
  for(int j =0;j<length_;++j)
  {
    x_data[j] -= x_mean;
    y_data[j] -= y_mean;

  }
  
  
  Gnuplot g(z_data,inten_data,"intensity","linespoints","z","I");
  g.set_grid().replot();
  
  Gnuplot g2(x_data,y_data,"position", "linespoints");
  g2.set_grid().replot();
  
  wait_for_key();
  g.remove_tmpfiles();
  g2.remove_tmpfiles();
      

}
