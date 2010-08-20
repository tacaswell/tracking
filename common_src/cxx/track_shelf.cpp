//Copyright 2008-2010 Thomas A Caswell
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

#include "counted_vector.h"
#include "generic_wrapper.h"
#include "array.h"
#include "cell.h"
#include "exception.h"
#include "wrapper_o.h"
#include "md_store.h"

using std::list;
using std::pair;
using std::cout;
using std::endl;
using std::vector;
using std::runtime_error;
using std::logic_error;
using std::string;

using utilities::Histogram;
using utilities::Md_store;

using utilities::Ll_range_error;
using utilities::Array;
using utilities::Cell;

using utilities::Wrapper_out;
using utilities::Tuple;

using namespace tracking;

typedef list<Track_box*> tr_list ;


Track_shelf::~Track_shelf(){
  for(tr_list::iterator it = tracks_.begin();
      it!=tracks_.end(); ++it)
    delete *it;
  std::cout<<"track dead"<<std::endl;
}


void Track_shelf::add_new_track(particle_track* first_part){
  if(first_part ==NULL)
    throw runtime_error("null particle");
  tracks_.push_back(new Track_box(first_part));
  ++track_count_;
  

}
void Track_shelf::add_track(Track_box * track)
{
  tracks_.push_back(track);
  
  ++track_count_;
  
}


void Track_shelf::remove_short_tracks(int min_length){
  int tmp_length;
  
  // beware of tricksy iterator nonsense here
  for(tr_list::iterator it = tracks_.begin();
      it!=tracks_.end(); )
  {
    tmp_length = (*it)->get_length();

    if( tmp_length< min_length)	
      remove_track_internal_(it++); // this needs to be done this way
				    // to prevent from getting all
				    // manner of allocation bugs*
    else
      ++it;
    
  }
  
  
  
  
}

void Track_shelf::print(){
 for(tr_list::iterator it = tracks_.begin();
     it!=tracks_.end(); it++)
   {
     //     cout<<"track id: "<<it->first<<endl;
     (*it)->print();
   }
}



// void Track_shelf::set_shelf(){
//   for(list<int,Track_box* >::iterator it = tracks_.begin();
//       it!= tracks_.end(); it++)
//     (*it)->set_track();
// }

void Track_shelf::remove_track_internal_(  tr_list::iterator it)
{
  if(it == tracks_.end())
    throw runtime_error("not in list");
  delete *it;
  *it = NULL;
  tracks_.erase(it);
  --track_count_;
  
}

void Track_shelf::track_length_histogram(Histogram & hist_in){
  for(tr_list::iterator it = tracks_.begin();
      it!=tracks_.end(); it++)
    hist_in.add_data_point((*it)->get_length());
}

void Track_shelf::msd(vector<double> & msd_vec,vector<int> & entry_count)const{
  //this exception needs to get it's own class or something
  if(msd_vec.size()!=entry_count.size())
    throw runtime_error("Vector size's don't match, change this exception");

  int max_time_step = msd_vec.size();


  double disp_sq_sum;
  int tmp_count;

  const particle_track* current = NULL;
  const particle_track* next = NULL;

  bool not_past_end = false;

  

  for(tr_list::const_iterator working_track = tracks_.begin();
      working_track!=tracks_.end(); working_track++)
    {
      
      //      cout<<"Track legnth: "<<(*working_track)->get_length()<<endl;
      for(int j = 0; j<((*working_track)->get_length()-1) && j < max_time_step;j++){
	tmp_count =0;
	disp_sq_sum = 0;
	
	not_past_end = true;

	current = (*working_track)->get_first();
	not_past_end = current->step_forwards(j+1,next);
	while(not_past_end){
	  disp_sq_sum += current->distancesq(next);
	  ++tmp_count;
	  
	  current = next;
	  not_past_end = current->step_forwards(j+1,next);
	}
	msd_vec.at(j) += disp_sq_sum/tmp_count;
	++(entry_count.at(j));
	
      }
    }


  vector<double>::iterator it = msd_vec.begin();
  vector<int>::iterator it2 = entry_count.begin();
  for(;it<msd_vec.end();it++, it2++)
    {
      if(*it2 >0)
	{
	  (*it) = (*it)/(double)(*(it2));
	}
    }
  
}



void Track_shelf::msd_corrected(utilities::Counted_vector & msd)const
{
  //this exception needs to get it's own class or something

  int max_time_step = msd.get_length();
  const particle_track* current = NULL;
  const particle_track* next = NULL;

  

  bool not_past_end = false;
  for(tr_list::const_iterator working_track = tracks_.begin();
      working_track!=tracks_.end(); working_track++)
  {
      
    //      cout<<"Track legnth: "<<(*working_track)->get_length()<<endl;
    int track_length = ((*working_track)->get_length()-1);
    
    for(int j = 0; j< track_length && j < max_time_step;j++)
    {
      int tmp_count=0;
      double tmp_dist_sq_sum=0;

      not_past_end = true;
      current = (*working_track)->get_first();
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

void Track_shelf::msd_corrected(utilities::Counted_vector & md,
				utilities::Counted_vector & msd,
				utilities::Counted_vector & msd_sq)const
{


  int max_time_step = msd.get_length();
  const particle_track* current = NULL;
  const particle_track* next = NULL;

  

  bool not_past_end = false;
  for(tr_list::const_iterator working_track = tracks_.begin();
      working_track!=tracks_.end(); working_track++)
  {
      

    int track_length = ((*working_track)->get_length()-1);
    
    for(int j = 0; j< track_length && j < max_time_step;j++)
    {
      int tmp_count=0;
      double tmp_dist_sum=0;
      double tmp_dist_sq_sum=0;
      double tmp_dist_sq_sq_sum=0;

      not_past_end = true;
      current = (*working_track)->get_first();
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



void Track_shelf::disp_sq_hist(int time_step ,utilities::Histogram & disp_sq_hist) const
{
  const particle_track* current = NULL;
  const particle_track* next = NULL;
  bool not_past_end = false;
  
  //const particle_track* next = NULL;
  if(time_step<1)
    throw runtime_error("Track_shelf::disp_sq_hist: Time step less than 1");
  
  
  
  for(tr_list::const_iterator working_track = tracks_.begin();
      working_track!=tracks_.end(); working_track++)
  {
    
    current = (*working_track)->get_first();
    not_past_end = current->step_forwards(time_step,next);
    while(not_past_end)
    {
      disp_sq_hist.add_data_point(current->distancesq_corrected(next));
      current = next;
      not_past_end = current->step_forwards(time_step,next);
      
    }
    
  }
}


void Track_shelf::set_raw_disp_to_cell(Cell & output)const{
  tr_list::const_iterator working_track = tracks_.begin();
  Array tmp(1);
  for(int j = 0; j<output.get_length();j++)
    {
      (*(working_track++))->extract_raw_disp(tmp);
      output.add_array(tmp);
    }

}


void Track_shelf::set_corrected_disp_to_cell(Cell & output)const{
  tr_list::const_iterator working_track = tracks_.begin();
  Array tmp(1);
  for(int j = 0; j<output.get_length();j++)
    {
      (*(working_track++))->extract_corrected_disp(tmp);
      output.add_array(tmp);
    }

}



void Track_shelf::pass_fun_to_track(void(Track_box::*fun)()const)const
{
  tr_list::const_iterator myend =  tracks_.end();
  for(tr_list::const_iterator it = tracks_.begin();
      it!=myend;++it)
  {
    ((*it)->*fun)();
  }
}


void Track_shelf::split_to_parts(Track_shelf & output_shelf)
{

  if(&output_shelf == this)
    throw runtime_error("need to provide a second shelf to put trimmed tracks into");
  

  tr_list::const_iterator myend =  tracks_.end();
  for(tr_list::const_iterator it = tracks_.begin();
      it!=myend;++it)
  {
    (*it)->split_to_parts(output_shelf);
  }
  
}



void Track_shelf::output_link_to_wrapper(Wrapper_out & wrapper,
					 const Tuple<float,3>& scale_t,
					 const utilities::Tuple<float,3> & dim) const
{

 
  

  wrapper.add_meta_data("dims",dim);
  wrapper.add_meta_data("number-of-planes",1);
  wrapper.open_group(0,track_count_);
  tr_list::const_iterator myend =  tracks_.end();
  

  
  for(tr_list::const_iterator it = tracks_.begin();it!=myend;++it)
  {

    wrapper.set_all_values((*it),scale_t);
    
    
  }
  wrapper.close_group();

}

void Track_shelf::renumber()
{
  unsigned int j = 0;
  for(tr_list::iterator it = tracks_.begin();
      it!=tracks_.end(); ++it)
    (*it)->set_track_id(j++);
}

void Track_shelf::output_to_wrapper(utilities::Generic_wrapper & wrapper,const Md_store * md_store) const
{
  // check to make sure that all the required meta data is in the store

  // \todo really implement md checking
  
  int write_comp;
  md_store->get_value("write_comp",write_comp);


  vector<unsigned int> lengths(track_count_),start_plane(track_count_),start_part(track_count_);
  unsigned int local_track_count = 0;

  // loop over tracks to get out
  // 1) the size
  // 2) the starting plane
  // 3) the starting particle
  list<Track_box*>::const_iterator it_end = tracks_.end();
  for(list<Track_box*>::const_iterator it = tracks_.begin();
      it != it_end;++it)
  {
    // check to make sure we don't run out the top
    if(!(local_track_count<track_count_))
      throw logic_error("track_shelf :: trying to add more tracks than the track shelf claims to know about");
    
    // get the pointer to the track we are working on
    Track_box* cur_track = *it;
    // get it's id
    unsigned int track_id = cur_track->get_id();
    // make sure it's ID plays nice (enforcing pigeon hole property)
    if(track_id != local_track_count)
      throw logic_error("track_shelf :: track ids are not monotonically incrementing by one, remember to run renumber first");
    
    // get the track length
    lengths[track_id] = cur_track->get_length();
    // get pointer to the first particle
    particle * tmp_part = cur_track->get_first();
    // get the start plane and particle index
    start_part[track_id] = tmp_part->get_ind();
    start_plane[track_id] = tmp_part->get_frame();
    // mark that we added a particle
    ++local_track_count;
  }
  
  // make sure that the wrapper is open
  bool opened_wrapper = false;
  
  if(!wrapper.is_open())
  {
    wrapper.open_wrapper();
    opened_wrapper = true;
  }
  // open group
  string g_name = utilities::format_name("tracking",write_comp);
  wrapper.open_group(g_name);
  
  // shove in meta-data
  wrapper.add_meta_data(md_store);
  
  // shove in real data
  const unsigned int * tmp_ptr;
  tmp_ptr = &lengths.front();
  wrapper.add_dset(1,&local_track_count,utilities::V_UINT,tmp_ptr,"length");

  tmp_ptr = &start_plane.front();
  wrapper.add_dset(1,&local_track_count,utilities::V_UINT,tmp_ptr,"start_plane");

  tmp_ptr = &start_part.front();
  wrapper.add_dset(1,&local_track_count,utilities::V_UINT,tmp_ptr,"start_particle");

  // tidy up
    
  wrapper.close_group();

  if(opened_wrapper)
    wrapper.close_wrapper();

}
