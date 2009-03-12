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

#include "touple.h"

// namespace utilities{
// class Touple;
// }

#ifndef PARTICLE_TRACK
#define PARTICLE_TRACK
namespace tracking{
class track_box;

/**
   Class for use in particles once they have been tracked.  This class carries
   additional information about which track the particle is in, where in
   the track it is.  If a particle isn't in a track, the track
   information should be set to -1
 */
class particle_track :public particle_base{
protected:
  ///next particle in track
  particle_track* next;
  ///prev particle in track
  particle_track* prev;
  /**
     Pointer to which track the particle is in. If the
     particle is not in a track this is NULL
   */
  track_box * track;
  
  /**
     pointer to a list of particles to be used durring the tracking
     process
   */
  std::list<pair<particle_track*, double> >* n_pos_link;
  /**h
     pointer to a list of particles to be used durring the tracking
     process
  */
  std::list<pair<particle_track*, double> >* p_pos_link;

  /**
     The foward displacement vector
   */
  utilities::Touple forward_disp_;
public:
  
  ///Friends with track_box to allow it to screw with the linked list
  ///directly with out having to go through the getter/setter functions
  ///to remove an uneeded level of indrection, still keep the sanity check
  ///on not nuking the list for most uses.
  //  friend class track_box;
  friend class track_list;
  friend class hash_case;

  /**
     Constructor based on the particle_base constructor, needs to be finished
     to do something clever with data teh comes in pre-tracked
     @param i_data
     @param o_data
     @param i_ind
     @param i_track
  */
  //nuke this eventually
  particle_track(wrapper_i_base * i_data, wrapper_o_base* o_out, 
		 int i_ind, track_box* i_track = NULL);

  particle_track(wrapper_i_base * i_data, wrapper_o_base* o_out, 
		int i_ind,  std::set<wrapper::p_vals>* contents_in);


  ///Destructor
  ~particle_track();

  /**
     calls base class print
   */
  void print();
  /**
     recursive print function for printing out the sequence of a
     track.
     @param more
     how many more particles to decend
  */
  void print_t(int more);

  /**
    Returns a pointer to the next particle in the track.
    If this is the last particle in the track than NULL
    will be returned.
  */
  particle_track* get_next(){return next;}

  /**
    Returns a pointer to the previous particle in the track.
    If this is the first particle in the track than NULL
    will be returned.
   */
  particle_track* get_prev(){return prev;}


  utilities::Touple* get_forward_disp(){return &forward_disp_;}

  /**
     returns the particle n from the current particle forward down the
     list where n=0 is the current particle.  If the end of the track
     is reached   throws an error.
   */
  particle_track* step_forwards(int n);
  
  /**
     returns the particle n from the current particle backwards up the
     list where n=0 is the current particle.  If the front of the
     track is reached throws an error
   */
  particle_track* step_backwards(int n);

  

  /**
     sets what track the particle belongs to.  Each particle can only
     belong to one track.  This code will do sanity checks to make
     sure that tracks are not accidentally moved from track to track.
     If track is already non-NULL then the error "moving between
     lists!" is thrown.
     @param i_track
     pointer to the track_box that represents the track this particle
     is to be a part of
   */
  void set_track(track_box* i_track);

  /**
     Sets the pointer to the next particle.  If next isn't NULL before
     being set, then this function thows the error "nuking the list"
     to prevent accidental destruction of the track.
     @param n_nest
     pointer to the particle to be set as next
   */
  void set_next(particle_track* n_next);

  /**
     Sets the pointer to the previous particle.  If prev isn't NULL
     before being set, then this function thows the error "nuking the
     list" to prevent accidental destruction of the track.
     @param n_prev
     pointer to the particle to be set as prev
   */
  void set_prev(particle_track* n_prev);


  /**
     get what track the particle belongs to.  Each particle can only
     belong to one track.
   */
  track_box* get_track();

  /**
     Retruns the identifer of the track_box the particle is in
   */
  int get_track_id();
  /**
     Returns the value of type for this particle.x
   */
  virtual double get_value(wrapper::p_vals type);
  

};
}
#endif
