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
#include "tuple.h"
#include <utility>

// namespace utilities{
// class Tuple;
// }

#ifndef PARTICLE_TRACK
#define PARTICLE_TRACK
namespace tracking{
class track_box;
class hash_shelf;
/**
   Class for use in particles once they have been tracked.  This class carries
   additional information about which track the particle is in, where in
   the track it is.  If a particle isn't in a track, the track
   information should be set to -1
 */
class particle_track :public particle_base{

public:
  
  ///Friends with track_box to allow it to screw with the linked list
  ///directly with out having to go through the getter/setter functions
  ///to remove an uneeded level of indrection, still keep the sanity check
  ///on not nuking the list for most uses.
  //  friend class track_box;
  friend class track_list;
  friend class hash_case;


  
  particle_track(int i_ind,int frame);

  /**
     constructor that does not require any calls to the wrapper to get
     set up.
   */
  particle_track(int i_ind,utilities::Tuple pos,unsigned int frame);


  ///Destructor
  ~particle_track();


  /**
     recursive print function for printing out the sequence of a
     track.
     @param more
     how many more particles to descend
  */
  void print_t(int more)const;

  /**
    Returns a pointer to the next particle in the track.
    If this is the last particle in the track than NULL
    will be returned.
  */
  const particle_track* get_next()const{return next_;}

  /**
    Returns a pointer to the next particle in the track.
    If this is the last particle in the track than NULL
    will be returned that is not const protected
  */
  particle_track* get_next(){return next_;}

  /**
    Returns a pointer to the previous particle in the track.
    If this is the first particle in the track than NULL
    will be returned.
   */
  const particle_track* get_prev()const{return prev_;}

  /**
     Returns the uncorrected forward displacement
   */
  const utilities::Tuple& get_raw_forward_disp()const
  {return forward_disp_;}

  /**
     Returns the corrected forward displacement
   */
  const utilities::Tuple get_corrected_forward_disp()const;

  
  /**
     returns the particle n from the current particle forward down the
     list where n=0 is the current particle.  If the end of the track
     is reached   throws an error.
   */
  const particle_track* step_forwards(int n)const;
  
  /**
     returns the particle n from the current particle backwards up the
     list where n=0 is the current particle.  If the front of the
     track is reached throws an error
   */
  const particle_track* step_backwards(int n)const;

  
  /**
    Returns true if there is a particle n steps forward and
    passes a pointer to it out as a reference, if there is
    not a particle, returns false.
   */
   bool step_forwards(int n,const particle_track* & next)const;
  
  /**
    Returns true if there is a particle n steps backwards and
    passes a pointer to it out as a reference, if there is
    not a particle, returns false.
   */
   bool step_backwards(int n,const particle_track* & next)const;
  

  

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
  track_box* get_track()const;

  /**
     Retruns the identifer of the track_box the particle is in
   */
  int get_track_id()const;
  /**
     Returns the value of type for this particle.
   */
  virtual float get_value(utilities::D_TYPE type) const;

  /**
     Returns pointer to the shelf the particle resides in
   */
  
  const hash_shelf* get_shelf()const{
    return shelf_;
  }

  
  /**
     Returns the displacement squared corrected for the net drift
   */
  float distancesq_corrected(const particle_track* part_in)const;

  /**
     Sets what shelf the particle is in
    
     This needs to have checks added
   */
  void set_shelf(hash_shelf* shelf)
  {
    shelf_ = shelf;
  } 

  /**
     Clears all of the data associated with the tracking
   */
  virtual void clear_track_data();
  
  /**
     returns if the track is set
   */
  bool has_track()const;
  
  /**
     returns if a particle_base.has a next
   */
  bool has_next()const;
  /**
     returns the position with the cumulative distribution subtracted off
   */
  const utilities::Tuple get_corrected_pos()const;
  

protected:
  ///next particle in track
  particle_track* next_;
  ///prev particle in track
  particle_track* prev_;
  /**
     Pointer to which track the particle is in. If the
     particle is not in a track this is NULL
   */
  track_box * track;

  /**
     Pointer to the shelf that the particle lives in.  This is
     redundent with the plane information, but this is more elegent
     and makes looking up the planes cumlative displacemet easier
   */
  hash_shelf* shelf_;
  
  /**
     pointer to a list of particles to be used durring the tracking
     process
   */
  std::list<std::pair<particle_track*, float> >* n_pos_link;
  /**h
     pointer to a list of particles to be used durring the tracking
     process
  */
  std::list<std::pair<particle_track*, float> >* p_pos_link;

  /**
     The foward displacement vector
   */
  utilities::Tuple forward_disp_;


  
};
}
#endif
