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
#include "particle.h"
#include "master_box_t.h"
#include "wrapper.h"
#include "track_list.h"
#include "track_shelf.h"

#include "wrapper.h"

#ifndef HASH_CASE
#define HASH_CASE

namespace tracking{
/**
   Class to hold sets of hash_shelf.  This is used as a structure to organize
   the hash tables by time frame.  This will work fine with 3d stacks because
*/

class hash_case{

public:
  ///retrun a given shelf
  hash_shelf * return_shelf(int n){
    return h_case.at(n);
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
     vector of the dimsions of the orginal images (x,y,z,...) (maybe, check this)
     @param ppb
     initial pixels per box, this can be rescaled later
     @param frames
     the number of frames.  It is assumed that the frames are numbered [0-frames)

  */
  template <class particle>
  hash_case(master_box_t<particle> & mb,vector<int> img_dims, 
	    unsigned int ppb, int frames);
  
  template <class particle>
  void  init(master_box_t<particle> & mb,vector<int> img_dims, 
	    unsigned int ppb, int frames);
  
  hash_case();
  

  ///Print out a sensible representation of the data
  void print();

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
     the maximum displacement a particle is extected to have in
     a time step
     @param tracks
     a track_shelf object passed in by reference.
  */
  void link(double max_range, track_shelf& tracks);

  /**
     Compute mean forward displacement for each shelf
   */
  void compute_mean_disp();

  ///Destructor
  ~hash_case();
protected:
  ///vector of pointers to shelves
  vector<hash_shelf*> h_case;
  //  bool lt_pair_tac(const pair<particle_track*, double> &  lh, const pair<particle_track*, double> & rh);

  
   
  /**
     Given a list of particles fills in the n_pos_link lists and appends
     the appriot values to the p_pos_link lists when those particles are
     added to the n_pos_link lists.
     @param
     tlist list of particles to find the possible next particles of
     @param in_it iterator to the vector h_case that points to the
     hash_shelf to look for possible next particles in
     @param max_disp
     the maximum allowed displacement when looking for possible partcicles
  */
  void fill_pos_link_next(std::list<particle_track*>* tlist, 
			  vector<hash_shelf*>::iterator in_it, double max_disp);

  
  

  bool inited;

};

// template<class particle>
// hash_case::hash_case(master_box_t<particle> & mb,unsigned int imsz1, 
// 		     unsigned int imsz2, unsigned int ppb, int frames){
//   inited = true;
//   mb.append_to_data_types(wrapper::d_next);
//   mb.append_to_data_types(wrapper::d_prev);
//   h_case.resize(frames);
//   for(unsigned int j = 0; j<h_case.size(); j++){
//     h_case.at(j) = new hash_shelf(imsz1,imsz2, ppb,j);
//   }
//   // cout<<
//   particle_base *p;
//   for(unsigned int j = 0; j<mb.size(); j++){
//     p = mb.get_particle(j);
//     (h_case.at(p->get_value(wrapper::d_frame)))->push(p);
//   }
// }

template <class particle>
hash_case::hash_case(master_box_t<particle> & mb,vector<int> img_dims, 
		     unsigned int ppb, int frames):inited(false){
  init(mb,img_dims,ppb,frames);

}
  


template<class particle>
void hash_case::init(master_box_t<particle> & mb,vector<int> img_dims, 
		     unsigned int ppb, int frames){

  if(inited){
    std::cout<<"can't re init"<<std::endl;
    return;
  }
    

  mb.append_to_data_types(wrapper::d_next);
  mb.append_to_data_types(wrapper::d_prev);
  h_case.resize(frames);
  for(unsigned int j = 0; j<h_case.size(); j++){
    h_case.at(j) = new hash_shelf(img_dims, ppb,j);
  }
  // cout<<
  particle_base *p;
  for(unsigned int j = 0; j<mb.size(); j++){
    p = mb.get_particle(j);
    try{//    cout<<(int)p->get_value(wrapper::d_frame)<<"-";
      (h_case.at((int)p->get_value(wrapper::d_frame)))->push(p);
    }
    catch(...){
      int yar = (int)p->get_value(wrapper::d_frame);
      cout<<"trying to put in to shelf: "<<yar<<endl;
      return;
    }
  
  }
  //  cout<<endl;
  inited = true;
}

}


#endif
