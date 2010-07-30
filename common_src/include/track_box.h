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



#ifndef TRACK_BOX
#define TRACK_BOX
#include <vector>
#include "tuple.h"
namespace utilities{
class Array;
class Wrapper_out;


}

namespace tracking{
// forward declaration
class particle_track;
class Track_shelf;


/**
   Box class for dealing with tracks.  Essentially roling my own
   linked list class
*/
class Track_box{
public:
  ///retruns the nth particle of the track.
  ///returns null if n > length
  const particle_track * at(int n) const;

  /**
     Returns a pointer to the first particle in the track
   */
  particle_track * get_first() const{
    return t_first_;
  }

  /**
     Returns a pointer to the last particle in the track
   */
  particle_track * get_last() const{
    return t_last_;
  }




  /**
     Extracts the raw posistion along the track
   */
  void extract_raw_pos(utilities::Array & output) const;
  /**
     gets the posistion corrected for drift
   */
  void extract_corrected_pos(utilities::Array & output) const;
    
  /**
     Extracts the raw frame to frame displanement along the track
   */
  void extract_raw_disp(utilities::Array & output) const;
  /**
     gets the displacement corrected for drift
   */
  void extract_corrected_disp(utilities::Array & output) const;
  
  ///Constructor
  Track_box(particle_track * first);
    
  ///Prints outs some representation of the track
  virtual void print();
  ///adds the particle next the end of the track
  void push_back(particle_track* next);

  int get_id() const
  {
    return id_;
  }

  virtual ~Track_box();
  
  /**
     outputs the particles in the track to what ever output
     wrapper they are pointing at.  This function needs to be
     renamed
   */
  void set_track() const;

  /**
     Returns the length of the track
   */
  int get_length()const
  {
    return length_;
  };
  
  /**
     plots the intensity as a function of position along the track
   */
  void plot_intensity() const;
  /**
     Outputs the particles in a track-centric format
   */
  void output_to_wrapper(utilities::Wrapper_out & wrapper) const;
  
  /**
     splits the track into segments that correspond to a single particle
     sliced in z.  The resulting new tracks are stored in the self
   */
  void split_to_parts(Track_shelf & shelf);

  /**
     returns the intensity weighted average position used for link3D.  This really
     should be sub-classed
   */
  void average_cord(utilities::Tuple<float,3> & pos,float & I) const ;
  

protected:
  ///Pointer to first particle in track
  particle_track * t_first_;
  //pointer to last particle in track
  particle_track * t_last_;
  ///length of path
  int length_;
  ///the number of tracks identified used for unique id's
  static int running_count_;
  ///unique ID of the track
  unsigned int id_;

  
  /**
     non-const version of at()
   */
  particle_track * at(int n) ;


    
  
  /**
     Splits the track at the given index and returns a pointer to the back
     half of the track.
   */
  Track_box * split_track(int indx);
  
  /**
     Trims the track from the ind_start and keeps
     the next length particles.

     @param ind_start first particle to keep
     @param length number of particles to keep
   */
  void trim_track(int ind_start, int length);

  

};
}

#endif
