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
#include <list>
#include <set>
#include <vector>
#include "particle_track.h"
//#include "track_shelf.h"

#ifndef TRACK_LIST
#define TRACK_LIST
namespace tracking{

class Track_shelf;
/**
   List wrapper class for us in the track linking portion of the code.
   It is a list of particle_track pointers and functions to do the looping
   needed in the track locatign algorithm

   \todo
   need to overhaul to use list objets so that the pointer returning code in Hash_shelf
   can be removed
*/

class track_list{

public:

  ///finds the links between the list of particles held by the pointer
  ///in this object and the list being handed in.  The list handed in
  ///is destroyed! be aware of this .
  void link_next(std::list<particle_track*>* new_next);

  ///Sets up the object to start linking tracks.  The object passed in
  ///is DESTROYED (eventually), be aware of this!
  track_list(std::list<particle_track*>* first_list,
	     float i_max_disp,Track_shelf* in_tracks);

  ~track_list();
  
protected:
  ///Pointer to list of 'previous' particles
  std::list<particle_track*> * p_list;
  ///Pointer to std::list of 'next' particles
  std::list<particle_track*> * n_list;
  
  ///pointer to copy of current 'next' for next iteration 
  std::list<particle_track*>* store_list;
  
  ///List for the 'previous' particles in the subnetwork
  //std::set<particle_track*,bool(*)(particle_track*,particle_track*)> p_sub_net;
  std::set<particle_track*> p_sub_net;
  ///list for the 'next' particles in the subnetworks
  //std::set<particle_track*,bool(*)(particle_track*,particle_track*)> n_sub_net;
  std::set<particle_track*> n_sub_net;
  Track_shelf* tracks;

  ///maximum distance a particle may move
  float max_disp;
  ///square of the maximum distance a particle may move
  float max_disp_sq;

  
  /**
     Does the perumtation on the subnetwork lists and sets all the
     particles
   */
  void link_sub_ntwrk();
  
  
  ///finds and removes trivial bonds
  void trivial_bonds();

  ///sets up the first subnetwork that is found
  ///in the particle lists
  void set_up_sub_ntwrk();

  void clean_lists();
  
  ///for removing stuff from lists and cleaning up the possible link lists
  std::list<particle_track*>::iterator erase(std::list<particle_track*>* in,
					std::list<particle_track*>::iterator it);
  
  ///for sorting lists of particles
  static bool part_lt(particle_track* a, particle_track* b);
  

  void recur_fnc_np(std::vector<std::pair<particle_track*, particle_track*> >* min,
		    std::vector<std::pair<particle_track*, particle_track*> >* cur,
		    float disp,float& min_disp,
		    std::set<particle_track*>::iterator it,
		    const std::set<particle_track*>::iterator& itend);

  void recur_fnc_pn(std::vector<std::pair<particle_track*, particle_track*> >* min,
		    std::vector<std::pair<particle_track*, particle_track*> >* cur,
		    float disp,float & min_disp,
		    std::set<particle_track*>::iterator it,
		    const std::set<particle_track*>::iterator& itend);

  void  link_pairs(std::vector<std::pair<particle_track*, particle_track*> >& in);
  
};

// tac 2009-10-19
// took this out in September because I got rid of the unq_id
// inline bool track_list::part_lt(particle_track* a, particle_track* b){
//   return (a->unq_id_)<(b->unq_id_);
//}
}

#endif
