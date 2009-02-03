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
#include <vector>
#include "particle_track.h"

#ifndef TRACK_BOX
#define TRACK_BOX
namespace tracking{
/**
   Box class for dealing with tracks.  Essentially roling my own
   linked list class
*/
class track_box{
public:
  ///retruns the nth particle of the track.
  ///returns null if n > length
  particle_track * at(int n);

  /**
     Returns a pointer to the first particle in the track
   */
  particle_track * get_first(){
    return t_first;
  }

  /**
     Returns a pointer to the last particle in the track
   */
  particle_track * get_last(int n){
    return t_last;
  }


  ///adds the particle next the end of the track
  void push(particle_track* next);
  ///Extracts velocity vector from track, details of implementation
  ///pending
  void extract_velocity();
  
  ///Constructor
  track_box(particle_track * first);
    
  ///Prints outs some representation of the track
  virtual void print();

  void push_back(particle_track* next);

  int get_id(){return id;}

  virtual ~track_box(){};
  
  /**
     outputs the particles in the track to what ever output
     wrapper they are pointing at.  This function needs to be
     renamed
   */
  void set_track();

  /**
     Returns the length of the track
   */
  int get_length(){return length;};
protected:
  ///Pointer to first particle in track
  particle_track * t_first;
  //pointer to last particle in track
  particle_track * t_last;
  ///length of path
  int length;
  ///the number of tracks identified used for unique id's
  static int running_count;
  ///unique ID of the track
  int id;

};
}

#endif
