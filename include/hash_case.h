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
#include <list>
#include "hash_shelf.h"
#include "particle_base.h"
#include "master_box_t.h"
#include "wrapper.h"
#include "track_list.h"
#include "track_shelf.h"

#include "wrapper.h"

#include "tuple.h"

#ifndef HASH_CASE
#define HASH_CASE

namespace utilities{
class Array;
class Coarse_grain_array;
class Histogram2D;
}

namespace tracking{
/**
   Class to hold sets of hash_shelf.  This is used as a structure to organize
   the hash tables by time frame.  
*/

class hash_case{

public:
  ///retrun a given shelf
  hash_shelf * return_shelf(int n) const{
    return h_case_.at(n);
  }

  ///Templated constructor
  //assumse that incoming data has frames labeled sequentially
  //starting from 0
  //   template <class particle>
  //   hash_case(master_box_t<particle> & mb,unsigned int imsz1, 
  // 	     unsigned int imsz2, unsigned int ppb, int frames);
  template <class particle>
  hash_case(master_box_t<particle> & mb,unsigned int imsz1, 
	    unsigned int imsz2, unsigned int ppb, int frames);

  /**Templated constructor based on what type of particle is being
     used in the master_box the case is based off of.
     @param mb
     master_bos that the hash_case is based off of
     @param img_dims
     vector of the dimensions of the original images (x,y,z,...) (maybe, check this)
     @param ppb
     initial pixels per box, this can be rescaled later
     @param frames
     the number of frames.  It is assumed that the frames are numbered [0-frames)

  */
  template <class particle>
  hash_case(master_box_t<particle> & mb,const utilities::Tuple & img_dims, 
	    unsigned int ppb, int frames);
  hash_case();
  

  ///Print out a sensible representation of the data
  void print() const;

  /**
     Rehash all of the hash tables in this case to use the new
     part per box.  Rehashing seems to be very cheap.  This 
     looks most useful for tracking to make sure that the hash boxes
     are an apprioat size.
     @param ppb
     new value for pixels per box
  */
  void rehash(unsigned int ppb);

  

  /**
     Links particles in to tracks.
     @param max_range
     the maximum displacement a particle is expected to have in
     a time step
     @param tracks
     a track_shelf object passed in by reference.
  */
  void link(double max_range, track_shelf& tracks);

  /**
     Compute mean forward displacement for each shelf
   */
  void compute_mean_disp();

  /**
     Returns the mean displacement of as many planes as the array has rows
     starting from the plane start
   */
  void get_mean_disp(utilities::Array & mean_disp_array, int start=0);
  /**
     Returns the cumlative displacement of as many planes as the
     array has rows starting from the plane start
   */
  void get_cum_disp(utilities::Array & cum_disp_array, int start=0);

  /**
     Averages g(r) over the contained hash_shelfs 
   */
  void gofr_norm(double max_d, int nbins,
		 vector<double>& bin_count,vector<double>& bin_r) const;

  /**
     Computes D_rr, theu two point microrhelogy correlation.
     See 
     Title: Two-Point Microrheology of Inhomogeneous Soft  Materials
     Authors: Crocker, John C. and Valentine, M. T. and Weeks, Eric
     R. and Gisler, T.  and Kaplan, P. D. and Yodh, A. G. and Weitz,
     D. A.
     prl 85,8,888
   */
  void D_rr(utilities::Coarse_grain_array& D)const;

  
  /**
     comupute lots of correlations, need to clean this up
   */
  void D_lots(utilities::Coarse_grain_array & Duu,
	      utilities::Coarse_grain_array & DuuL,
	      utilities::Coarse_grain_array & DuuT,
	      utilities::Coarse_grain_array & Ddrdr,
	      // utilities::Coarse_grain_array & Dxx,
// 	      utilities::Coarse_grain_array & Dyy,
	      utilities::Coarse_grain_array & Ddudu,
	      utilities::Counted_vector const& md 
	      )const;
  

  /**
     for computing the direction of the nearest neighbor
   */
  void nearest_neighbor_array(utilities::Cell & pos_cell,
			      utilities::Cell & nn_cell, double range)const;

  /**
     looks at the first two nn 
   */
  void next_nearest_neighbor_array(utilities::Cell & pos_array,
				   utilities::Cell & nn_array,
				   utilities::Cell & nnn_array)const;
  
  /**
     pass one at 2-D gofr
   */
  void gofr2D(double max_d, utilities::Histogram2D& gofr2 ) const;

  /**
     Passes functions all the way down the pyramid, this one for void,
     argument-less functions, non-const
  */
  void pass_fun_to_part(void(particle_base::*fun)());

  /**
     Passes functions all the way down the pyramid, this one for void,
     argument-less functions, const
  */
  void pass_fun_to_part(void(particle_base::*fun)() const)const;



  /**
     Passes functions to the shelves in the case, this one for void,
     argument-less functions
  */
  void pass_fun_to_shelf(void(hash_shelf::*fun)());

  /**
     Passes functions to the shelves in the case, this one for void,
     argument-less functions
  */
  void pass_fun_to_shelf(void(hash_shelf::*fun)()const)const;

  

  ///Destructor
  ~hash_case();
protected:
  /**
     vector of pointers to shelves
   */
  vector<hash_shelf*> h_case_;


  
   
  /**
     Given a list of particles fills in the n_pos_link lists and appends
     the appropriate values to the p_pos_link lists when those particles are

     @param
     tlist list of particles to find the possible next particles of
     @param in_it iterator to the vector h_case that points to the
     hash_shelf to look for possible next particles in
     @param max_disp
     the maximum allowed displacement when looking for possible particles
  */
  void fill_pos_link_next(std::list<particle_track*>* tlist, 
			  vector<hash_shelf*>::iterator in_it, double max_disp);

  
  

  bool inited;
	    
	    
  
  template <class particle>
  void  init(master_box_t<particle> & mb,const utilities::Tuple & img_dims, 
	    unsigned int ppb, int frames);


};

template <class particle>
hash_case::hash_case(master_box_t<particle> & mb,const utilities::Tuple & img_dims, 
		     unsigned int ppb, int frames):inited(false){
  init(mb,img_dims,ppb,frames);

}
  


template<class particle>
void hash_case::init(master_box_t<particle> & mb,const utilities::Tuple & img_dims, 
		     unsigned int ppb, int frames){

  if(inited){
    std::cout<<"can't re init"<<std::endl;
    return;
  }
    

  mb.append_to_data_types(wrapper::D_NEXT);
  mb.append_to_data_types(wrapper::D_PREV);
  h_case_.resize(frames);
  h_case_.at(0) = new hash_shelf(img_dims, ppb,0);
  for(unsigned int j = 1; j<h_case_.size(); ++j){
    h_case_[j] = new hash_shelf(img_dims, ppb,j);
    h_case_[j-1]->set_next_shelf(h_case_[j]);
  }
  // cout<<
  particle *p;
  int max_sz = mb.size();
  

  int current_frame =-1;
  int current_count = 0;
  


  for( int j = 0; j<max_sz; ++j){
    p = mb.get_particle(j);
    try{
      int cf = (int)(p->get_value(wrapper::D_FRAME));
      if(cf != current_frame)
      {
	cout<<"frame "<<current_frame<<": "<<current_count<<endl;
	current_frame = cf;
	current_count = 1;
      }
      else
      {
	++current_count;
      }
      
      (h_case_.at((int)(p->get_value(wrapper::D_FRAME))))->push(p);
    }
    catch(...){
      int yar = (int)p->get_value(wrapper::D_FRAME);
      cout<<"trying to put in to shelf: "<<yar<<endl;
      return;
    }
  
  }
  cout<<"frame "<<current_frame<<": "<<current_count<<endl;

  inited = true;
}

}


#endif
