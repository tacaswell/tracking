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

#include "track_shelf.h"

#include "gnuplot_i.hpp"

#include <stdexcept>
using std::runtime_error;
using std::logic_error;


using gnuplot::Gnuplot;
using gnuplot::GnuplotException;
using gnuplot::wait_for_key;


using namespace tracking;
using utilities::Array;
using utilities::Tuplei;
using utilities::Tuplef;
using utilities::Tuple;


using std::vector;
using std::pair;

using std::cout;
using std::endl;


typedef unsigned int unt;


int Track_box::running_count_ = 0;

// Track_box::Track_box(particle_track * first){
//   t_first_ = first;
//   t_last_ = first;

//   id_ = running_count_++;
//   if(first != NULL){
//     first->set_track(this);
//     length_ = 1;
//   }
// }


Track_box::Track_box(particle_track * first)
  :t_first_(first),t_last_(first),length_(0){
  id_ = running_count_++;
  if(t_first_ != NULL)
  {
    t_last_ = t_first_->reset_track(this,length_);
  }
}


void Track_box::print(){
  cout<<"Track: "<<id_<<" length: "<<length_<<endl<<"-----"<<endl;
  
  t_first_->print_t(length_);
}

const particle_track* Track_box::at(int n)const{
  if (n>length_)
    return NULL;
  const particle_track * tmp = NULL;
  if(n<length_/2)
    t_first_->step_forwards(n,tmp);
  else
    t_last_->step_backwards(length_ - n-1,tmp);
  
  return tmp;
}

particle_track* Track_box::at(int n){
  if (n>length_)
    return NULL;
  particle_track * tmp = NULL;
  if(n<length_/2)
    t_first_->step_forwards(n,tmp);
  else
    t_last_->step_backwards(length_ - n-1,tmp);
  
  return tmp;
}


void Track_box::push_back(particle_track * input_part){
  if( input_part==NULL)
    return;

  // if the track is initialized and has a last particle
  if(t_last_)
  {
    if(!(t_first_))
      throw logic_error("encountered a track with a last, but no first particle, flip out");
    
    // set the current last to point to the input particle
    t_last_->set_next(input_part);
    // set the input particle to point back to the old last particle
    // (still pointed to by t_last_)
    input_part->set_prev(t_last_);
  }
  // else, assume we need to initialize it
  else
  {
    // set length to 0 (look at what reset_track does)
    length_ = 0;
    // make the 'input particle the first particle in track
    t_first_ = input_part;
  }
  // update the last particle in the track
  t_last_ = input_part->reset_track(this,length_);
}


void Track_box::extract_raw_disp(Array & output) const{
  output.clear(length_);
  const particle_track* working_part = t_first_;
  const particle_track* next_part = working_part->get_next();;
  for(int j = 0; j<length_;++j)
  {
    output.push((working_part->get_disp(next_part)));
    working_part = next_part;
    next_part = working_part->get_next();;
  }
  
}



void Track_box::extract_corrected_disp(Array & output) const{
  output.clear(length_);
  const particle_track* working_part = t_first_;
  const particle_track* next_part = working_part->get_next();;
  for(int j = 0; j<length_;++j)
  {
    output.push((working_part->get_corrected_disp(next_part)));
    working_part = next_part;
    next_part = working_part->get_next();;
  }
  
}

Track_box::~Track_box(){
  particle_track* cur_part = t_first_;
  particle_track* next_part = NULL;
  while(cur_part != NULL)
    {
      next_part = cur_part->get_next();
      cur_part-> clear_track_data();
      cur_part = next_part;
    }
}


void Track_box::plot_intensity() const
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
    Tuplef pos = cur_part->get_position();
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


void Track_box::split_to_parts(Track_shelf & shelf)
{

  const int buffer = 2;
  const int d_range = 1;
  
  
  
  
  // extract data from track
  vector<float> I(length_);
  const particle_track * cur_part = t_first_;
  
  for(int j = 0;j<length_;++j)
  {
    // fill I
    cur_part->get_value(utilities::D_I,I[j]);
    cur_part = cur_part->get_next();
    
  }
  
  // identify local maximum
  int l_max_count = 0;
  
  int d_range_sz = d_range +1;
  vector<float> window;
  window.clear();
  window.resize(d_range_sz);
  
  vector<bool> local_maxes;
  local_maxes.resize(length_);
  

  // the strange bounds excludes the ends from being local maximums
  for(int j = 1; j<(length_-1);++j)
  {

   int wind_bot = (j-d_range)<0?0:(j-d_range);
   int wind_top = (j+d_range+1)>length_?length_:(j+d_range+1);
   
    // assume the current 
    local_maxes[j] = true;
    
    for(int k = wind_bot;k<wind_top;++k)
    {
      if(k==j)
	continue;
      else if(I[k] > I[j])
      {
	local_maxes[j] = false;
      }
    }
    if(local_maxes[j])
      ++l_max_count;
  }
  //  cout<<"l_max_count"<<l_max_count<<endl;
  
  vector<int> peak_indx;
  peak_indx.reserve(l_max_count);
  for(int j = 0;j<length_;++j)
    if(local_maxes[j])
      peak_indx.push_back(j);
  
      

  // sort out which local maximum to keep
  for(int j = 0; j<(l_max_count-1);++j)
  {
    // see if the next peak is too close
    if((peak_indx[j+1] - peak_indx[j])<2*buffer)
    {
      // see if the next peak is to close
      if(j<(l_max_count-2) && ((peak_indx[j+2] - peak_indx[j+1])<2*buffer))
      {
	// deal with 3 peak case
	// if the middle peak is not highest, kill it
	if(I[peak_indx[j+1]]<I[peak_indx[j]] ||
	   I[peak_indx[j+1]]<I[peak_indx[j+2]])
	{
	  peak_indx.erase(peak_indx.begin()+j+1);
	  --l_max_count;
	}
	// else kill the outer two
	else
	{
	  // remove the second one first to not screw up the indexing of the other one
	  peak_indx.erase(peak_indx.begin()+j+2);
	  peak_indx.erase(peak_indx.begin()+j);
	  --j;
	  --(--l_max_count);

	}
      }
      else
      {
	// keep peak with higher intensity
	if(I[peak_indx[j]]>I[peak_indx[j+1]])
	{
	  peak_indx.erase(peak_indx.begin()+j+1);
	  --l_max_count;
	}
	else
	{
	  peak_indx.erase(peak_indx.begin()+j);
	  --j;
	  --l_max_count;
	}
	
      }
    }
      
  }
    
  // set up full regions
  vector<pair<int,int> > regions;
  regions.resize(l_max_count);
  for(int j= 0; j<l_max_count;++j)
  {
    int tmp =peak_indx[j]-buffer;
    regions[j].first = tmp<0?0:tmp;

    tmp = peak_indx[j]+buffer+1;
    regions[j].second = tmp>length_?length_:tmp;
    // trim regions
    if(j>0)
    {
      if(regions[j-1].second > regions[j].first)
      {

	
// 	cout<<"pre_trim"<<endl;
// 	cout<<regions[j-1].first<<","<<regions[j-1].second<<endl;
// 	cout<<regions[j].first<<","<<regions[j].second<<endl;


	
	--regions[j-1].second;
	++regions[j].first;
	
// 	cout<<"post_trim"<<endl;
// 	cout<<regions[j-1].first<<","<<regions[j-1].second<<endl;
// 	cout<<regions[j].first<<","<<regions[j].second<<endl;


      }
    }
    
  }
  // do actual cuts
  for(vector<pair<int,int> >::reverse_iterator rit = regions.rbegin();
      rit<regions.rend();++rit)
  {
    const pair<int,int> tmp_pair = *rit;
    
    Track_box * tmp_bx = split_track(tmp_pair.first);
    tmp_bx->trim_track(0,tmp_pair.second-tmp_pair.first);
    shelf.add_track(tmp_bx);
  }
  
      
}

Track_box* Track_box::split_track(int split_point)
{
  // get first particle of new track
  particle_track * back_first = at(split_point);

  // set pointer to next on last particle of first segment to null
  t_last_ = (back_first->get_prev());
  if(split_point !=0)
    t_last_->clear_next();
  else if (split_point==0)
    t_first_=NULL;
  
  length_ = split_point;
  // set prev pointer of first particle of second section to null
  back_first->clear_prev();
  
  // reduce length_ of first track
  

  // make second track, return

  return new Track_box(back_first);
  
}



void Track_box::trim_track(int start,int length)
{
  // set new first
  t_first_ = at(start);
  // walk backwards clearing track data



  
  // get new last particle
  if(! t_first_->step_forwards(length - 1,t_last_))
    throw runtime_error("track_box: stepped out of track");
  
  // set new length
  length_ = length;
  t_first_->clear_data_backward();
  t_last_->clear_data_forward();
  
}

void Track_box::average_cord(Tuple<float,3> & cord, float & tot_I)const
{
  tot_I = 0;
  float x_m=0,y_m=0,z_m=0;
  
  const particle_track * cur_part = t_first_;
  float I_tmp =0;
  float z_tmp =0;
  Tuplef xy;
  
  while(cur_part != NULL)
  {
    xy = cur_part-> get_position();
    cur_part->get_value(utilities::D_I,I_tmp);
    cur_part->get_value(utilities::D_ZPOS,z_tmp);
    
    x_m += xy[0] * I_tmp;
    y_m += xy[1] * I_tmp;
    z_m += z_tmp   * I_tmp;
    tot_I += I_tmp;
    cur_part = cur_part->next_;
    
  }
  x_m /=tot_I;
  y_m /=tot_I;
  z_m /=tot_I;
  
  cord[0] = x_m;
  cord[1] = y_m;
  cord[2] = z_m;
}

