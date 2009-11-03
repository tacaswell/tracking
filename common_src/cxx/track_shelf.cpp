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

#include <iostream>
#include <cmath>

#include "track_shelf.h"
#include "histogram.h"
#include "track_box.h"
#include "particle_track.h"
#include "svector.h"
#include "counted_vector.h"
#include "generic_wrapper_base.h"
#include "array.h"
#include "cell.h"
#include "exception.h"


using std::map;
using std::pair;
using std::cout;
using std::endl;
using std::vector;


using utilities::Histogram;
using utilities::Svector;
using utilities::Ll_range_error;
using utilities::Array;
using utilities::Cell;

using namespace tracking;

typedef  map<int,track_box*> tr_map;

track_shelf::~track_shelf(){
  for(map<int,track_box*>::iterator it = track_map.begin();
      it!=track_map.end(); it++)
    delete it->second;
  std::cout<<"track dead"<<std::endl;
}


void track_shelf::add_new_track(particle_track* first_part){
  if(first_part ==NULL)
    throw "null particle";
  track_box * tmp_box = new track_box(first_part);
  track_map.insert(pair<int,track_box*>(tmp_box->get_id(), tmp_box));
  ++track_count_;
  

}

void track_shelf::add_to_track(int track, particle_track* next_particle){
  map<int,track_box*>::iterator it = track_map.find(track);
  if(it == track_map.end())
    throw "not in map";
  if(next_particle ==NULL)
    throw "null particle";
  (it->second)->push_back(next_particle);
}

void track_shelf::remove_track(int track){
  map<int,track_box*>::iterator it = track_map.find(track);
  remove_track_internal_(it);
}

track_box* track_shelf::get_track(int track){
  map<int,track_box*>::iterator it = track_map.find(track);
  if(it == track_map.end())
    throw "not in map";
  return it->second;
}

void track_shelf::remove_short_tracks(int min_length){
  int tmp_length;
  
  for(map<int,track_box*>::iterator it = track_map.begin();
      it!=track_map.end(); ){
      tmp_length = ((*it).second)->get_length();
      if( tmp_length< min_length){
	remove_track_internal_(it++);
      }
      else{
	it++;
      }
  }
  
  
}

void track_shelf::print(){
 for(map<int,track_box*>::iterator it = track_map.begin();
     it!=track_map.end(); it++)
   {
     cout<<"track id: "<<it->first<<endl;
     (it->second)->print();
   }
}



// void track_shelf::set_shelf(){
//   for(map<int,track_box* >::iterator it = track_map.begin();
//       it!= track_map.end(); it++)
//     ((*it).second)->set_track();
// }

void track_shelf::remove_track_internal_(  map<int,track_box*>::iterator it)
{
  if(it == track_map.end())
    throw "not in map";
  delete it->second;
  it->second = NULL;
  track_map.erase(it);
}

void track_shelf::track_length_histogram(Histogram & hist_in){
  for(map<int,track_box*>::iterator it = track_map.begin();
      it!=track_map.end(); it++)
    hist_in.add_data_point(((*it).second)->get_length());
}

void track_shelf::msd(Svector<double> & msd_vec,Svector<int> & entry_count)const{
  //this exception needs to get it's own class or something
  if(msd_vec.data.size()!=entry_count.data.size())
    throw "Vector size's don't match, change this exception";

  int max_time_step = msd_vec.data.size();


  double disp_sq_sum;
  int tmp_count;

  const particle_track* current = NULL;
  const particle_track* next = NULL;

  bool not_past_end = false;

  

  for(map<int,track_box*>::const_iterator working_track = track_map.begin();
      working_track!=track_map.end(); working_track++)
    {
      
      //      cout<<"Track legnth: "<<(*working_track).second->get_length()<<endl;
      for(int j = 0; j<((*working_track).second->get_length()-1) && j < max_time_step;j++){
	tmp_count =0;
	disp_sq_sum = 0;
	
	not_past_end = true;

	current = (*working_track).second->get_first();
	
	while(not_past_end){
	  try{
	    next = current->step_forwards(j+1);
	    disp_sq_sum += current->distancesq(next);
	    ++tmp_count;
	    current = next;
	  }
	  catch(Ll_range_error & e){
	    not_past_end = false;
	    msd_vec.data.at(j) += disp_sq_sum/tmp_count;
	    ++(entry_count.data.at(j));
	  }
	}
      }
    }

  vector<double>::iterator it = msd_vec.data.begin();
  vector<int>::iterator it2 = entry_count.data.begin();
  for(;it<msd_vec.data.end();it++, it2++)
    {
      if(*it2 >0)
	{
	  (*it) = (*it)/(double)(*(it2));
	}
    }
  
}


void track_shelf::msd_corrected(Svector<double> & msd_vec,Svector<int> & entry_count)const{
  //this exception needs to get it's own class or something
  if(msd_vec.data.size()!=entry_count.data.size())
    throw "Vector size's don't match, change this exception";

  int max_time_step = msd_vec.data.size();


  double disp_sq_sum;
  int tmp_count;

  const particle_track* current = NULL;
  const particle_track* next = NULL;

  bool not_past_end = false;

  

  for(map<int,track_box*>::const_iterator working_track = track_map.begin();
      working_track!=track_map.end(); working_track++)
    {
      
      //      cout<<"Track legnth: "<<(*working_track).second->get_length()<<endl;
      for(int j = 0; j<((*working_track).second->get_length()-1) && j < max_time_step;j++)
      {
	tmp_count =0;
	disp_sq_sum = 0;
	
	not_past_end = true;
	
	current = (*working_track).second->get_first();
	
	while(not_past_end){
	  try{
	    next = current->step_forwards(j+1);
	    disp_sq_sum += current->distancesq_corrected(next);
	    ++tmp_count;
	    current = next;
	  }
	  catch(Ll_range_error & e){
	      not_past_end = false;
	      msd_vec.data.at(j) += disp_sq_sum;
	      (entry_count.data.at(j))+=tmp_count;
// 	      msd_vec.data.at(j) += disp_sq_sum/tmp_count;
// 	      ++(entry_count.data.at(j));
	  }
	}
      }
    }
  

  vector<double>::iterator it = msd_vec.data.begin();
  vector<int>::iterator it2 = entry_count.data.begin();
  for(;it<msd_vec.data.end();it++, it2++)
    {
      if(*it2 >0)
	{
	  (*it) = (*it)/(double)(*(it2));
	}
    }
}



void track_shelf::msd_corrected(utilities::Counted_vector & msd)const
{
  //this exception needs to get it's own class or something

  int max_time_step = msd.get_length();
  const particle_track* current = NULL;
  const particle_track* next = NULL;

  

  bool not_past_end = false;
  for(map<int,track_box*>::const_iterator working_track = track_map.begin();
      working_track!=track_map.end(); working_track++)
  {
      
    //      cout<<"Track legnth: "<<(*working_track).second->get_length()<<endl;
    int track_length = ((*working_track).second->get_length()-1);
    
    for(int j = 0; j< track_length && j < max_time_step;j++)
    {
      int tmp_count=0;
      double tmp_dist_sq_sum=0;

      not_past_end = true;
      current = (*working_track).second->get_first();
      not_past_end = current->step_forwards(j+1,next);
      while(not_past_end)
      {

	tmp_dist_sq_sum += current->distancesq_corrected(next);
	++tmp_count;
	current = next;
	not_past_end = current->step_forwards(j+1,next);

      }
      msd.batch_add_to_element(j, tmp_dist_sq_sum,tmp_count);
      //msd.add_to_element(j, tmp_dist_sq_sum/tmp_count);
    }
  }
}

void track_shelf::msd_corrected(utilities::Counted_vector & md,
				utilities::Counted_vector & msd,
				utilities::Counted_vector & msd_sq)const
{


  int max_time_step = msd.get_length();
  const particle_track* current = NULL;
  const particle_track* next = NULL;

  

  bool not_past_end = false;
  for(map<int,track_box*>::const_iterator working_track = track_map.begin();
      working_track!=track_map.end(); working_track++)
  {
      

    int track_length = ((*working_track).second->get_length()-1);
    
    for(int j = 0; j< track_length && j < max_time_step;j++)
    {
      int tmp_count=0;
      double tmp_dist_sum=0;
      double tmp_dist_sq_sum=0;
      double tmp_dist_sq_sq_sum=0;

      not_past_end = true;
      current = (*working_track).second->get_first();
      not_past_end = current->step_forwards(j+1,next);
      while(not_past_end)
      {

	double tmp_dist_sq = current->distancesq_corrected(next);
	tmp_dist_sum += sqrt(tmp_dist_sq);
	tmp_dist_sq_sum += tmp_dist_sq;
	tmp_dist_sq_sq_sum += (tmp_dist_sq*tmp_dist_sq);
	++tmp_count;
	current = next;
	not_past_end = current->step_forwards(j+1,next);
	// // version 3
	// 	not_past_end = false;
	
      }
      if(tmp_count>0)
      {
	
	//       // version 3
	//       md.add_to_element(j, tmp_dist_sum);
	//       msd.add_to_element(j, tmp_dist_sq_sum);
	//       msd_sq.add_to_element(j, tmp_dist_sq_sq_sum);
      
	//       // version 2
	//       md.add_to_element(j, tmp_dist_sum/tmp_count);
	//       msd.add_to_element(j, tmp_dist_sq_sum/tmp_count);
	//       msd_sq.add_to_element(j, tmp_dist_sq_sq_sum/tmp_count);
	// version 1

	md.batch_add_to_element(j, tmp_dist_sum,tmp_count);
	msd.batch_add_to_element(j, tmp_dist_sq_sum,tmp_count);
	msd_sq.batch_add_to_element(j, tmp_dist_sq_sq_sum,tmp_count);
      }
      
    }
  }
}



void track_shelf::msd_hist(int time_step ,utilities::Histogram & in) const
{
  const particle_track* current = NULL;
  //const particle_track* next = NULL;
  if(time_step<1)
  {
    throw "nonsense input";
  }
  
  for(map<int,track_box*>::const_iterator working_track = track_map.begin();
      working_track!=track_map.end(); working_track++)
  {
    current = (*working_track).second->get_first();
    bool more_track = current->has_next();
    while(more_track)
    {
      in.add_data_point((current->get_corrected_forward_disp()).magnitude_sqr());
      more_track = current->step_forwards(time_step,current);
    }	 
  }
}


void track_shelf::set_raw_disp_to_cell(Cell & output)const{
  tr_map::const_iterator working_track = track_map.begin();
  Array tmp(1);
  for(int j = 0; j<output.get_length();j++)
    {
      ((*(working_track++)).second)->extract_raw_disp(tmp);
      output.add_array(tmp);
    }

}


void track_shelf::set_corrected_disp_to_cell(Cell & output)const{
  tr_map::const_iterator working_track = track_map.begin();
  Array tmp(1);
  for(int j = 0; j<output.get_length();j++)
    {
      ((*(working_track++)).second)->extract_corrected_disp(tmp);
      output.add_array(tmp);
    }

}


void track_shelf::initial_corrected_pos_to_wrapper(utilities::Generic_wrapper_base * data_out_wrapper)const{
  tr_map::const_iterator working_track = track_map.begin();
  const particle_track *  working_track_ptr;

  data_out_wrapper->initialize_wrapper();
  while(working_track != track_map.end())
  {
    data_out_wrapper->start_new_row();
    // note trickiness
    working_track_ptr = ((*(working_track++)).second)->get_first();
    const utilities::Tuple i_pos = working_track_ptr->get_corrected_pos();
    
    data_out_wrapper->append_to_row(i_pos[0]);
    data_out_wrapper->append_to_row(i_pos[1]);
    data_out_wrapper->append_to_row(working_track_ptr->get_frame());
    
    data_out_wrapper->finish_row();
  }
  
  data_out_wrapper->finalize_wrapper();


  
}

void track_shelf::corrected_tracks_out(Cell & output, utilities::Generic_wrapper_base * data_out_wrapper)const{
  tr_map::const_iterator working_track = track_map.begin();
  const particle_track *  working_track_ptr;
  Array tmp(1);
  data_out_wrapper->initialize_wrapper();
  
  while(working_track != track_map.end())
  {
    // Deal with the initial position data 
    data_out_wrapper->start_new_row();

    working_track_ptr = ((*(working_track)).second)->get_first();
    const utilities::Tuple i_pos = working_track_ptr->get_corrected_pos();
    
    data_out_wrapper->append_to_row(i_pos[0]);
    data_out_wrapper->append_to_row(i_pos[1]);
    data_out_wrapper->append_to_row(working_track_ptr->get_frame());
    
    data_out_wrapper->finish_row();
    
    // Deal with the displacements
    ((*(working_track)).second)->extract_corrected_disp(tmp);
    output.add_array(tmp);

    // increment the iterator 
    ++working_track;
    
  }
  
  data_out_wrapper->finalize_wrapper();


}



void track_shelf::pass_fun_to_track(void(track_box::*fun)()const)const
{
  map<int,track_box*>::const_iterator myend =  track_map.end();
  for(map<int,track_box*>::const_iterator it = track_map.begin();
      it!=myend;++it)
  {
    ((it->second)->*fun)();
  }
}

  
