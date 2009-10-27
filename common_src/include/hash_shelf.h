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




#include "part_def.h"
#include "pair.h"
#include "triple.h"


#ifndef HASH_SHELF
#define HASH_SHELF
namespace utilities{
class Coarse_grain_array;
class Counted_vector;
class Array;
class Histogram2D;
class Wrapper_out;



}

namespace tracking{
class hash_box;
class master_box_t;

class Corr;
/**
   Class to implemnt the hash table
 */

class hash_shelf{
public:
  /**
     add the particle_base to the hash_shelf
     @param p pointer to the particle to add
  */
  void push(particle * p);

//   /*
//      add the particle_track to the hash_shelf
//      @param p pointer to the particle to add
//   */
//   void push(particle_track * p);


  ///constructor for eating a master_box_t
  hash_shelf(master_box_t & mb, int imsz1, 
	      int imsz2, unsigned int ppb);

  ///Constructor that makes an empty hash_shelf
  hash_shelf(unsigned int imsz1, unsigned int imsz2, 
	     unsigned int ppb,int i_frame);
  ///Constructor that makes an empty hash_shelf
  hash_shelf(utilities::Tuple imgsz, unsigned int ippb, int i_frame);


  ///Changed the pixels per box and re-hashes the shelf
  void rehash(unsigned int PPB);
  
  ///Generates the hash value based on a pointer to a particle object
  virtual unsigned int hash_function(const particle* p) const;

  ///Destructor
  virtual ~hash_shelf();

  ///Print function
  virtual void print() const;

  ///@name Getter Functions
  ///@{

  ///returns the box at (n,m)
  hash_box* get_box(int x, int y) const{
    return (hash_.at(((int)hash_dims_[0])*y + x));
  }
  
  ///Retruns the hash for the particle p

  
  /**
     appends the region of (2*range +1) on a side from the hash table
     centered on the box (n,m) deals with boxes with in range of the
     edges by truncating the volume returned.  Functions that use this
     need to keep track of this them selves
  */
  void get_region( int n,  int m, 
		   hash_box* box, int range=1) const;
  void get_region_px( int n, 
		      std::vector<const particle*> & out ,
		      int range_in_px) const;
  
  void get_region( int n,
		   hash_box* box, int range=1) const;
  virtual void get_region( particle* n,
			   hash_box* box, int range=1) const;

  
  void get_region( int n,
		   std::list<particle*>& in_list, int range=1) const;

  void get_region( int n, int m,
		   std::vector<const particle*> & out,int range) const;
  void get_region( int n,
		   std::vector<const particle*> & out,int range) const;
  
  
  int get_plane_num() const
  {
    return plane_number_;
  }
  
  ///@}

  ///@name g(r)
  ///@{

  ///returns the fully normalized g(r)
  void gofr_norm(float max_d, int nbins,
	    std::vector<float>& bin_count,std::vector<float>& bin_r) const;
  /**
     returns the total number of particles.  The bin_c is
     normalized by area and density, but not averaged
   */
  int gofr(float max_d, int nbins,
	    std::vector<float>& bin_count,std::vector<float>& bin_r) const;

  /**
     returns the total number of particles.  The bin_c is
     normalized by area and density, but not averaged.  Does
     not reinitialize the data arrays
   */

  float gofr(float max_d, int nbins, std::vector<float>& bin_count,int & count) const;
  ///@}

  


  /**
     G(r) filling in a more sensible object
  */
  void gofr(utilities::Coarse_grain_array G,int& particle_count) const;
  /**
     Generates and returns a pointer to all the particles in
     this shelf in a list form.  This makes new objects on the
     heap, be aware of this for memory leaks.

     \todo 
     remove, but this version can not be removed until the tracking code is overhauled to 

     see the other version
   */
  std::list<particle_track*> * shelf_to_list() const;

  /**
     Converts the whole shelf to a list of const pointers
   */
  void shelf_to_list(std::list<const particle*> &tmp) const;
  
  /**
     Converts the whole shelf to a list
   */
  void shelf_to_list(std::list<particle*> &tmp) const;
  
  /**
     Computes the mean displacement of a frame.  This is done to deal
     with possible flows in the sample.  Particles with out forward
     tracking data are ignored.
  */
  void compute_mean_forward_disp(utilities::Tuple & cum_disp);
  /**
     Getter function for mean forward displacement from the
     previous shelf in a case.
   */
  const utilities::Tuple& get_mean_forward_disp() const{
    return mean_forward_disp_;
  }
  /**
     Getter function for mean forward displacement from the
     first shelf in a case.
  */
  const utilities::Tuple&  get_cum_forward_disp() const{
    return cumulative_disp_;
  }

  
  /**
     Computes D_rr, theu two point microrhelogy correlation.
     See 
     Title: Two-Point Microrheology of Inhomogeneous Soft  Materials
     Authors: Crocker, John C. and Valentine, M. T. and Weeks, Eric
     R. and Gisler, T.  and Kaplan, P. D. and Yodh, A. G. and Weitz,
     D. A.
     prl 85,8,888
   */
  void D_rr(utilities::Coarse_grain_array & D)const;
  
  /**
     Computes a collection of correlation functions
  */
  void D_lots(utilities::Coarse_grain_array & Drr,
	      utilities::Coarse_grain_array & Dtt,
	      utilities::Coarse_grain_array & Ddrdr,
// 	      utilities::Coarse_grain_array & Dxx,
// 	      utilities::Coarse_grain_array & Dyy,
	      utilities::Coarse_grain_array & Duu,
	      utilities::Coarse_grain_array & Ddudu,
	      utilities::Counted_vector const & msd 
	      )const;
  
  /**
     Sets the pointer to the next shelf in a case
   */  
  void set_next_shelf(hash_shelf* next);
  


  
  /**
    Returns true if there is a shelf n steps forward and
    passes a pointer to it out as a reference, if there is
    not a shelf, returns false.
   */
  bool step_forwards(int n,const hash_shelf* & next)const;
  /**
     Returns an array of the vectors that point to the nearest
     neighbors.
   */
  void nearest_neighbor_array(utilities::Array & pos_array,
			      utilities::Array & nn_array, float range)const;
  
  /**
     next nn
   */
  void next_nearest_neighbor_array(utilities::Array & pos_array,
				   utilities::Array & nn_array,
				   utilities::Array & nnn_array)const;

  /**
     2D gofr
   */
  void gofr2D(float max_d, utilities::Histogram2D & gofr2 ) const;

  /**
     Fills in the neighbor vectors for the particles in the shelf
   */
  void fill_in_neighborhood();

  /**
     Passes functions down the pyramid, this one for void, argument-less
     functions non-const
   */
  void pass_fun_to_part(void(particle::*fun)());
  

  /**
     Passes functions down the pyramid, this one for void, argument-less
     functions, const
   */
  void pass_fun_to_part(void(particle::*fun)()const)const;
  
  /**
     for outputting to a wrapper
   */
  void output_to_wrapper(utilities::Wrapper_out & wrapper) const;
  
  
  /**
     passes a Corr object down the pyramid
   */
  void compute_corr(tracking::Corr &) const ;

  /**
     returns the average density of a region a distance buffer away from the edges
   */
  unsigned int get_density()const;
  
  
protected:
  //change all of this to be pointers to hash_boxes, to keep
  //consistent with everythign else


  ///Main data structure.  This is an vector of
  ///hash boxes.  For simplicity the strcuture is stored as
  ///a 1-D array and the class takes care of the 1D<->2D conversion
  std::vector<hash_box*> hash_;
  
  ///@name grid properties.
  /// These should not be allowed to change once
  /// the hash table is instantiated 
  //@{ 
  
  ///Vector to store the dimensions of the grid
  //vector<int> hash_dims;
  utilities::Tuple hash_dims_;
  ///Vector to store the dimensions of input data
  //  vector<int> img_dims;
  utilities::Tuple img_dims_;

  
  ///number of pixels per side of the gridboxes
  unsigned int ppb_;
  //@}
  ///stores the plane number of the shelf
  int plane_number_;
  
  /**
     The mean displacement of the plane.  The definition of velocity
     is $v_i = x_{i +1} - x_i$ hence the 'forward' velocity
   */
  utilities::Tuple mean_forward_disp_;

  /**
     Cumlative displacement vector
  */
  utilities::Tuple cumulative_disp_;
  
  /**
     computes area of image
   */
  int img_area()const;

  /**
     Pointer to the next shelf in the case
   */
  hash_shelf * next_;

  /**
     number of particles in shelf.
     Minor memory hit, but useful to know
   */
  unsigned int particle_count_;

  /**
     if the containing hash_case, and thus this shelf and it's boxes own the
     particles and need to clean them up at the end
   */
  bool own_particles_;
  
private:
  ///Initialization function
  //  void init(unsigned int X, unsigned int Y, unsigned int PPB);
  void init2();


};


}
#endif
