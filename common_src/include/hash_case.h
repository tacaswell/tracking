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

#ifndef HASH_CASE
#define HASH_CASE
#include <vector>
#include <list>

#include "part_def.h"






namespace utilities{
class Array;

class Histogram2D;
class Wrapper_out;
class Wrapper_in;
class Cell;
class Counted_vector;
class Accum_case;
class Wrapper_in;
class Generic_wrapper;
class Filter;


}

namespace tracking{
class Track_shelf;
class Hash_shelf;
class track_list;
class Master_box;

class Corr;
class Corr_case;
class Accumulator;

/**
   Class to hold sets of Hash_shelf.  This is used as a structure to organize
   the hash tables by time frame.  
*/

class hash_case{

public:
  ///retrun a given shelf
  Hash_shelf * return_shelf(int n) const{
    return h_case_.at(n);
  }

  /**
     Constructor, does no work
  */
  hash_case();
  
  

    
  /**
     Initializer that actually does the work.  DO NOT USE THIS FOR NEW CODE, IT IS GOING AWAY
  */
  void  init(Master_box & mb,const utilities::Tuplef & dims, 
	     float ppb, int frames);
  
  /**
     Initializer that takes in a wrapper rather than a master_box
  */
  void init(utilities::Wrapper_in & w_in , utilities::Filter & filt, float ppb)  ;
  

  ///print out a sensible representation of the data
  void print() const;
  
  /**
     Passes functions all the way down the pyramid, this one for void,
     argument-less functions, non-const
  */
  void pass_fun_to_part(void(particle::*fun)());

  /**
     Passes functions all the way down the pyramid, this one for void,
     argument-less functions, const
  */
  void pass_fun_to_part(void(particle::*fun)() const)const;



  /**
     Passes functions to the shelves in the case, this one for void,
     argument-less functions
  */
  void pass_fun_to_shelf(void(Hash_shelf::*fun)());

  /**
     Passes functions to the shelves in the case, this one for void,
     argument-less functions
  */
  void pass_fun_to_shelf(void(Hash_shelf::*fun)()const)const;

  /**
     returns the number of frames in the hash case
  */
  int get_num_frames() const
  {
    return h_case_.size();
  }
  
  
  /**
     outputs the hash case to the Wrapper that is passed in
  */
  void output_to_wrapper(utilities::Wrapper_out &) const;
  
  /**
     passes a Corr object down the pyramid
  */
  void compute_corr(tracking::Corr &) const ;

    
  /**
     Passes a Corr_case down the pyramid

     Returns the number of frames per calculation 
  */
  unsigned int compute_corr(tracking::Corr_case &) const ;

    
  /**
     Passes an Accumulator object down the pyramid
  */
  void compute_accum(Accumulator & in)const;

  /**
     Passes an Accumulator_case down the pyramid, one accumulator per hash_shelf 
  */
  void compute_accum(utilities::Accum_case & )const;
  

  /**
     fills in the neighborhoods for all of the particles
  */
  void fill_in_neighborhood();

  /**
     Gets the average dtime between the shelves
  */
  int get_avg_dtime()const;
  
  /**
     Gets the average temperature of all the planes in the case
  */
  float get_avg_temp()const;
  
  
  ///Destructor
  ~hash_case();

#ifdef TRACKING_FLG
  /**
     Initialization function for related to tracking
   */
  void init(float box_side_len,
	    const utilities::Wrapper_in & wrapper,
	    Track_shelf & tracks,
	    utilities::Filter & filt,
	    int min_trk_length);

  
  /**
     Links particles in to tracks.  
     
     @param max_range [in] the maximum displacement a particle is
     expected to have in a time step
     
     @param tracks [out] a Track_shelf object passed in by reference.
  */
  void link(float max_range, Track_shelf& tracks);

  /**
     Compute mean forward displacement for each shelf
  */
  void compute_mean_disp();

#endif
  

protected:
  /**
     vector of pointers to shelves
  */
  std::vector<Hash_shelf*> h_case_;


  
   
  /**
     Given a list of particles fills in the n_pos_link lists and appends
     the appropriate values to the p_pos_link lists when those particles are

     @param
     tlist list of particles to find the possible next particles of
     @param in_it iterator to the vector h_case that points to the
     Hash_shelf to look for possible next particles in
     @param max_disp
     the maximum allowed displacement when looking for possible particles
  */
  void fill_pos_link_next(std::list<particle_track*>* tlist, 
			  std::vector<Hash_shelf*>::iterator in_it, float max_disp);

  
  
  /**
     Bool if the Hash_case has been initialized.
   */
  bool inited_;
	    
	    

};


}


#endif
