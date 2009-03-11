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
#include <vector>

#include "hash_box.h"

#include "master_box_t.h"

#include "touple.h"


#ifndef HASH_SHELF
#define HASH_SHELF

namespace tracking{
/**
   Class to implemnt the hash table
 */

class hash_shelf{
public:
  void push(particle_base * p);

  ///Constructor for un-tempalted master box class, to be removed
 //  hash_shelf(master_box & mb,unsigned int imsz1, 
// 	     unsigned int imsz2, unsigned int ppb);

  ///Templated constructor for dealing with templated master_box_t objects
  template <class particle>
  hash_shelf(master_box_t<particle> & mb, int imsz1, 
	      int imsz2, unsigned int ppb);

  ///Constructor that makes an empty hash_shelf
  hash_shelf(unsigned int imsz1, unsigned int imsz2, 
	     unsigned int ppb,int i_frame);
  ///Constructor that makes an empty hash_shelf
  hash_shelf(vector<int>imgsz, unsigned int ppb, int i_frame);

  ///Changed the pixels per box and re-hashes the shelf
  void rehash(unsigned int PPB);
  
  ///Generates the hash value based on a pointer to a particle object
  virtual unsigned int hash_function(particle_base* p);

  ///Destructor
  virtual ~hash_shelf(){};

  ///Print function
  virtual void print();

  ///@name Getter Functions
  ///@{

  ///returns the box at (n,m)
  hash_box* get_box(int n, int m){
    return &(hash.at(hash_dims[1]*n + m));
  }
  
  ///Retruns the hash for the particle p


  void get_region( int n,  int m, 
		  hash_box* box, int range=1);

  void get_region( int n,
		   hash_box* box, int range=1);
  virtual void get_region( particle_base* n,
		   hash_box* box, int range=1);
  ///@}

  ///@name g(r)
  ///@{

  ///returns the fully normalized g(r)
  void gofr_norm(double max_d, int nbins,
	    vector<double>& bin_count,vector<double>& bin_r);
  /**
     returns the total number of particles.  The bin_c is
     normalized by area and density, but not averaged
   */
  int gofr(double max_d, int nbins,
	    vector<double>& bin_count,vector<double>& bin_r);
  ///@}

  /**
     Generates and returns a pointer to all the particles in
     this shelf in a list form.  This makes new objects on the
     heap, be aware of this for memory leaks.
   */
  std::list<particle_track*> * shelf_to_list();
  
  /**
     Computes the mean displacement of a frame.  This is done to deal
     with possible flows in the sample.  If a particle has no tracking
     data it is ignored, if it has two then the mean of the forwrad
     and backward dispacements are added, if the particle only has one
     link then half of the displacement is added.
  */
  void compute_mean_(){};

private:
  ///Initialization function
  //  void init(unsigned int X, unsigned int Y, unsigned int PPB);
  void init2();

protected:
  //change all of this to be pointers to hash_boxes, to keep
  //consistent with everythign else


  ///Main data structure.  This is an vector of
  ///hash boxes.  For simplicity the strcuture is stored as
  ///a 1-D array and the class takes care of the 1D<->2D conversion
  vector<hash_box> hash;
  
  ///@name grid properties.
  /// These should not be allowed to change once
  /// the hash table is instantiated 
  //@{ 
  
  ///Vector to store the dimensions of the grid
  vector<int> hash_dims;
  ///Vector to store the dimensions of input data
  vector<int> img_dims;
  
  ///number of pixels per side of the gridboxes
  unsigned int ppb;
  //@}
  ///stores the plane number of the shelf
  int plane_number;
  
  /**
     The mean displacement of the plane.  The
     definition of velocity is
     v_i = x_{i +1} - x_i
     hence the 'forward' velocity
   */
  Touple mean_forward_disp;
  
};

template <class particle>
hash_shelf::hash_shelf(master_box_t<particle> & mb, int imsz1, 
		       int imsz2, unsigned int PPB): ppb(PPB){
  img_dims.push_back(imsz1);
  img_dims.push_back(imsz2);
  cout<<img_dims[0]<<endl
      <<img_dims[1]<<endl;
  init2();
  for(unsigned int j = 0; j<mb.size();j++)
    {
      particle* p = mb.get_particle(j);
      push(p);
    }
  
  

}

inline unsigned int hash_shelf::hash_function(particle_base* p){
    return (unsigned int)
      (((unsigned int)p->get_value(wrapper::d_ypos)/ppb)*hash_dims[0] +
       p->get_value(wrapper::d_xpos)/ppb);
}
}
#endif
