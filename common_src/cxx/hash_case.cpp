//Copyright 2008,2009,2010 Thomas A Caswell
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
#include "hash_case.h"
#include "hash_shelf.h"
#include <limits.h>
#include <stdexcept> // out_of_range exception
#include <iostream>
#include <vector>
#include <set>
#include "master_box_t.h"

#include "track_box.h"
#include "track_shelf.h"

#include "particle_track.h"
#include "array.h"
#include "cell.h"
#include "enum_utils.h"


#include "corr.h"
#include "md_store.h"

#include "wrapper_i.h"
#include "filter.h"
#include "generic_wrapper.h"
using namespace tracking;


using utilities::Tuplei;
using utilities::Tuplef;
using utilities::Md_store;
using utilities::Wrapper_in;
using utilities::Filter;
using utilities::Generic_wrapper;
using utilities::D_TYPE;



using std::list;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::set;

using std::runtime_error;
using std::logic_error;

void hash_case::print() const{
  for(unsigned int j = 0; j<h_case_.size();j++){
    h_case_.at(j)->print();
    cout<<endl;
  }
}



hash_case::hash_case()
  :inited_(false){

}
  



void hash_case::init(Master_box & mb,const utilities::Tuplef & dims, 
		     float ppb, int frames){

  if(inited_){
    std::cout<<"can't re init"<<std::endl;
    return;
  }
    

  
  // mb.append_to_data_types(utilities::D_NEXT);
  // mb.append_to_data_types(utilities::D_PREV);
  
  
  h_case_.resize(frames);

  // do frame 0
  h_case_.at(0) = new Hash_shelf(dims, ppb,0,false);
  h_case_[0]->md_store_ = mb.get_Md_store(0);
  // do frame 1 to N
  for(unsigned int j = 1; j<h_case_.size(); ++j){
    h_case_[j] = new Hash_shelf(dims, ppb,j,false);
    h_case_[j-1]->set_next_shelf(h_case_[j]);
    h_case_[j]->md_store_ = mb.get_Md_store(j);
    
  }

  particle *p;
  int max_sz = mb.size();
  


  for( int j = 0; j<max_sz; ++j)
  {
    p = mb.get_particle(j);
    try
    {
      int cf;
      (p->get_value(utilities::D_FRAME,cf));
      
	
      h_case_.at(cf)->push(p);
#ifdef TRACKING_FLG
      p->set_shelf(h_case_.at(cf));
#endif
    }
    
    catch(std::exception & e)
    {
      std::cout<<e.what()<<endl;
      int yar ;
      (int)p->get_value(utilities::D_FRAME,yar);
      std::cout<<"trying to put in to shelf: "<<yar<<std::endl;
      throw;
      
    }
  
  }


  inited_ = true;
}



hash_case::~hash_case(){
  for(unsigned int j = 0; j<h_case_.size(); j++){
    delete h_case_.at(j);
  }
  std::cout<<"hc dead"<<std::endl;
}

#ifdef TRACKING_FLG
void hash_case:: compute_mean_disp(){
  Tuplef cum_disp;
  for(vector<Hash_shelf*>::iterator it = h_case_.begin();
      it<h_case_.end(); ++it){
    (*it)->compute_mean_forward_disp(cum_disp);  
  }
}
#endif	// PTYPE ==1

int hash_case::get_avg_dtime()const
{
  float dtime_sum = 0;
  int dtime_tmp = 0;
  unsigned int frame_count = 0;
  
  vector<Hash_shelf*>::const_iterator it_end = h_case_.end();
  // the ++ is to skip the first frame which does not have a
  // a dtime as it has no previous frame to compare too
  for(vector<Hash_shelf*>::const_iterator it = ++(h_case_.begin());
      it != it_end; ++it)
  {
    const Md_store * cur_store = (*it)->md_store_;
    if(cur_store)
    {
      dtime_sum += cur_store->get_value("dtime",dtime_tmp);
      ++frame_count;
    }
  }
  return (int) floor(dtime_sum/frame_count + 0.5); 
}



float hash_case::get_avg_temp()const
{
  float temperature_sum = 0;
  float temperature_tmp = 0;
  unsigned int frame_count = 0;
  
  vector<Hash_shelf*>::const_iterator it_end = h_case_.end();
  // the ++ is to skip the first frame which does not have a
  // a temperature as it has no previous frame to compare too
  for(vector<Hash_shelf*>::const_iterator it = ++(h_case_.begin());
      it != it_end; ++it)
  {
    const Md_store * cur_store = (*it)->md_store_;
    if(cur_store)
    {
      temperature_sum += cur_store->get_value("temperature",temperature_tmp);
      ++frame_count;
    }
  }
  return temperature_sum/frame_count; 
}

#ifdef TRACKING_FLG
void hash_case::init(float box_side_len,
		     const Wrapper_in & wrapper,
		     Track_shelf & tracks,
		     Filter & filt,
		     int min_trk_length
		     )
{
  // particle init stuff


  set<D_TYPE> data_types = wrapper.get_data_types();
  

  // make sure that sure that we have the correct data dtypes

  if(data_types.find(utilities::D_PREV_INDX) == data_types.end() ||
     data_types.find(utilities::D_NEXT_INDX) == data_types.end() ||
     data_types.find(utilities::D_TRACKID) == data_types.end())
    throw runtime_error("wrapper does not contain correct data types");
  

  particle::intialize_wrapper_in(&wrapper);
  particle::intialize_data_types(&data_types);


  // hash box init internal stuf
  if(inited_){
    std::cout<<"can't re init"<<std::endl;
    return;
  }
  int num_frames =wrapper.get_num_frames();
  h_case_.resize(num_frames);
  Tuplef dims = wrapper.get_dims();
  
  // do frame 0
  h_case_.at(0) = new Hash_shelf(dims, box_side_len,0,true);
  h_case_[0]->md_store_ = wrapper.get_Md_store(0);
  // do frame 1 to N
  for(unsigned int j = 1; j<h_case_.size(); ++j){
    h_case_[j] = new Hash_shelf(dims, box_side_len,j,true);
    h_case_[j-1]->set_next_shelf(h_case_[j]);
    h_case_[j]->md_store_ = wrapper.get_Md_store(j);
  }

  // loop over frames in wrapper
  for( int k = 0;k<num_frames;++k)
  {
    // loop over contents of frame
    int num_entries= wrapper.get_num_entries(k);
    
    for( int j = 0; j<num_entries; ++j)
    {
      // see if it has a previous particle
      int prev_index;
      wrapper.get_value(prev_index,j,utilities::D_PREV_INDX,k);
      if(prev_index < 0)
      {
	int next_indx;
	wrapper.get_value(next_indx,j,utilities::D_NEXT_INDX,k);
	// see if it has no previous particle and does have a next
	if(next_indx >= 0)
	{
	  // get track id
	  int cur_track_id = -1 ,track_id=-1;
	  wrapper.get_value(track_id,j,utilities::D_TRACKID,k);
	  
	  // get list of indexes in track
	  int cur_part = j;
	  int prev_part = j;
	  int cur_frame = k;
	  list<pair<int,int> > track_index_list; 
	  
	  do
	  {
	    // check trackid
	    wrapper.get_value(cur_track_id,cur_part,utilities::D_TRACKID,cur_frame);
	    if(cur_track_id != track_id)
	      throw logic_error("track ids do not match");
	    
	    // add to list
	    track_index_list.push_back(pair<int,int>( cur_part,cur_frame));
	    // increment frame	
	    ++cur_frame;
	    // see if we have a no next particle, if 
	    if(next_indx <0)
	      break;
	    // if there is a next particle process it
	    else
	    {
	      
	      // check to make sure the next particle thinks the current one precedes it
	      wrapper.get_value(prev_part,next_indx,utilities::D_PREV_INDX,cur_frame);
	      if(prev_part!=cur_part)
		throw logic_error("hash_case::init with tracks: tracks are not continuously");
	    
	      // step walking indexes
	      prev_part = cur_part;
	      cur_part = next_indx;
	      wrapper.get_value(next_indx,cur_part,utilities::D_NEXT_INDX,cur_frame);
	    }
	    
	  }
	  // conditions to bail if aren't met, but this should always
	  // evaluate to true as the break should always exit the loop
	  // for us
	  while(cur_part >=0 && cur_frame < num_frames);
	  
	  // check trk_length, if happy add
	  if(( cur_frame - k) >=min_trk_length)
	  {
	    
	    // make track
	    Track_box * cur_track = new Track_box(NULL);
	    list<pair<int,int> >::const_iterator til_end = track_index_list.end();
	    
	    for (list<pair<int,int> >::const_iterator it = track_index_list.begin();
		 it != til_end; ++it)
	    {
	      // get values from the iterator
	      cur_part = (*it).first;
	      cur_frame = (*it).second;
	      
	      particle *p = new particle(cur_part,cur_frame);
	      // add to shelf
	      h_case_[cur_frame]->push(p);
	      // add to track
	      cur_track->push_back(p);
	    
	    }
	    tracks.add_track(cur_track);	   
	  }
	}
	else // if neither link
	{
	  // run through filter
	  if(filt(j,k))
	  {
	    // add to hash shelf
	    h_case_[k]->push(new particle(j,k));
	  }
	}
      }
    }
  }
  inited_ = true;
  
}

#endif	// PTYPE = 1
