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
#include "track_shelf.h"
#include "histogram.h"
#include "track_box.h"
#include "particle.h"

#include <iostream>
using namespace tracking;
using utilities::Histogram;

track_shelf::~track_shelf(){
  for(map<int,track_box*>::iterator it = track_map.begin();
      it!=track_map.end(); it++)
    delete it->second;
}


void track_shelf::add_new_track(particle_track* first_part){
  if(first_part ==NULL)
    throw "null particle";
  track_box * tmp_box = new track_box(first_part);
  track_map.insert(pair<int,track_box*>(tmp_box->get_id(), tmp_box));

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



void track_shelf::set_shelf(){
  for(map<int,track_box* >::iterator it = track_map.begin();
      it!= track_map.end(); it++)
    ((*it).second)->set_track();

	
}

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
