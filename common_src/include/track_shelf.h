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
#ifndef TRACK_SHELF
#define TRACK_SHELF

#include <list>
#include <vector>
#include "tuple.h"
//forward declare histogram
namespace utilities{
class Histogram;
class Cell;
class Counted_vector;
class Generic_wrapper;
class Wrapper_out;
class Md_store;


}

namespace tracking{
//forward declare classes
class Track_box;
class particle_track;
class Master_box;
class Trk_accumulator;
class hash_case;

/**
   Top level class for holding the track boxes. (which are the head of
   the linked lists made of particle_track objects).
*/
class Track_shelf{

public:
  /**
     Add a track to the shelf starting from a particle.
     @param first_part
     The first particle in the track
   */
  void add_new_track(particle_track* first_part);
  
  /**
    adds a pointer to a track_box to the shelf.  The shelf takes
    responsibility for deleting the track when done.
   */
  void add_track(Track_box * track);

  /**
     Removes all tracks from the shelf that are shorter than n.
     This does not clear the link information in the particle_track
     objects.
     @param min_length
     the minimum length of tracks left in the shelf after 
   */
  void remove_short_tracks(int min_length);

  /**
     Print Track_shelf to stdout
   */
  void print();

  /**
     Sets all of the particles in shelf to their respective wrappers
  */
  void set_shelf();

  /**
     Generates a histogram of the lengths of all the tracks in the Track_shelf
   */
  void track_length_histogram(utilities::Histogram & in);


  /**
     Generates the MSD plot for all the tarcks in this shelf.

     @deprecated This computation should be done with a Trk_accumulator
   */
  void msd(std::vector<double> & msd_vec,std::vector<int> & entry_count)const;

  
  /**
     Generates the corrected MSD plot for all the tarcks in this shelf.
     
     @deprecated This computation should be done with a Trk_accumulator
   */
  void msd_corrected(utilities::Counted_vector & msd)const;
  
  /**
     Generates the corrected MSD, mean displacement and <|d|^4> plot for all the tarcks in this shelf.
     
     @deprecated This computation should be done with a Trk_accumulator
   */
  void msd_corrected(utilities::Counted_vector & md,
		     utilities::Counted_vector & msd,
		     utilities::Counted_vector & msd_sq)const;
  

  /*
     Computes the average fourier transform of the tracks
   */
  //void track_fft();
  
  /**
     generates a histogram of the squared
     displacement of the specified time step 

     @param time_step integer greater than zero less than maximum
     track length 
     @param disp_sq_hist histogram to be filled.  Is not
     cleared.

     @deprecated This computation should be done with a Trk_accumulator
   */
  void disp_sq_hist(int time_step,
		    utilities::Histogram & disp_sq_hist
		    )const;

  /**
     sets the tracks into the cell that is passed in
   */
  void set_raw_disp_to_cell(utilities::Cell & output) const;

  /**
     sets the corrected tracks into the cell that is passed in
   */
  void set_corrected_disp_to_cell(utilities::Cell & output) const;

  /**
     returns the number of tracks in shelf
   */
  unsigned int get_track_count() const{
    return track_count_;
    
  }

  /**
     Pass a function to all of the Track_box objects contained
   */
  void pass_fun_to_track(void(Track_box::*fun)()const)const;

  /**
     Outputs the particles in a track-centric format.  Assumes that
     the wrapper is already initialized.

     @param wrapper output wrapper, already initialized
   */
  void output_to_wrapper(utilities::Wrapper_out & wrapper) const;


  
  /**
     This function assumes that the tracks have been renumbered,  such that
     the pigeon hole principle applies.  If not, it will throw an error.
     It also assumes that the tracks are in order in the shelf.
     
     @param wrapper where to dump the data to.  
     @param md_store meta data store object
   */
  void output_to_wrapper(utilities::Generic_wrapper & wrapper,
			 const utilities::Md_store * md_store) const;

  
  /**
     outputs the contents of the shelf treating each track as a single
     particle.  for use with 3D linking.  This should be sub-classed
   */
  void output_link_to_wrapper(utilities::Wrapper_out & wrapper, 
			      const utilities::Tuple<float,3> & scale_t,
			      const utilities::Tuple<float,3> & dim) const;

  
  /**
     Splits all the tracks in the self in to sections that represent single
     particles when the tracking is linking between z-planes
   */
  void split_to_parts(Track_shelf & out_put_shelf);
  

  /**
     Renumbers the tracks sequentially.  Track id numbers are only used
     when outputting the tracks.
   */
  void renumber();
  
  /**
     Initializes the tracks in the track shelf based on read in trakid
   */
  void init(Master_box & mb);



  /**
     Initializes the tracks in the track shelf based on read in trakid
   */
  void init(hash_case & hc);

  /**
     Shove data in to a track accumulator
   */
  void compute_corrected_TA(Trk_accumulator & ta)const;
  
  /**
     Shove data in to a track accumulator, makes cuts based on the
     number of neighbors a particles has
   */
  
  void compute_corrected_TA_ncuts(Trk_accumulator & ta,int min_neighbors,int max_neighbors)const;
  
  /**
     Returns a pointer to track with id track_id.  raises
     runtime_error of track not found

     @param [in] track_id the id of the track to return
   */
  Track_box * get_track(unsigned int track_id) const ;
  
  
  ///Constructor
  Track_shelf():track_count_(0){};

  ///Destructor.  Destroys all of the tracks contained in the shelf
  ~Track_shelf();
protected:
  /** 
      list to keep track of all the created tracks.  The shelf is
      responsible for cleaning up the tracks at the end.  A list is used
      so that addition/removal is fast.  Random access isn't important.
      Tracks can now be renumbered.
  */
  std::list<Track_box*> tracks_;
  
  /**
     internal private function to do track removal to make maintianign code simpler
   */
  void remove_track_internal_(  std::list<Track_box*>::iterator it);

  /**
     number of tracks in the self
   */
  unsigned int track_count_;
  
  
};
}


#endif
