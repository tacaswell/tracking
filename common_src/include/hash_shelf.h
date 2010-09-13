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

#ifndef HASH_SHELF
#define HASH_SHELF


#include <vector>
#include <list>
#include <vector>




#include "part_def.h"

namespace utilities{

class Counted_vector;
class Array;
class Histogram2D;
class Wrapper_out;
class Md_store;



}

namespace tracking{
class hash_box;
class Master_box;

class Corr;
class Accumulator;

/**
   Class to implement the hash table.  This class contains hash_box objects and in contained by 
   Hash_case objects.
*/

class Hash_shelf{
public:

  /**
     Returns the image dimensions of the shelf
   */
  utilities::Tuplef get_img_dims()const
  {
    return utilities::Tuplef(img_dims_);
  }
  

  /**
     add the particle_base to the hash_shelf
     @param p pointer to the particle to add
  */
  void push(particle * p);
  
  ///Generates the hash value based on a pointer to a particle object
  ///@todo figure out why this is public and fix it
  virtual unsigned int hash_function(const particle* p) const;


  /**
     Constructor 
   */
  Hash_shelf(utilities::Tuplef imgsz, float upb,int i_frame,bool own_part, float z_offset = 0);
  


  ///Destructor
  virtual ~Hash_shelf();

  ///Print function
  virtual void print() const;



  

  /**
     @name Get Region
     appends the region of (2*range +1) on a side from the hash table
     centered on the box (n,m) deals with boxes with in range of the
     edges by truncating the volume returned.  Functions that use this
     need to keep track of this them selves
  */
  ///@{
  /**
     gets a region where range is set by a distance in real space
     /pram n the hash_box to center on
     /pram out a vector that the particles in the region will be added to
     /pram range_in_units the distance around the central box in data units to extract
  */
  void get_region_px( int n, std::vector<const particle*> & out ,
		      float range_in_units) const;

  /**
     gets a region around box n and puts the results into a hash_box.  This
     is here because it gets used in some places deep in the code.  These should
     be gotten rid of
     /pram n the hash_box to center on
     /pram box the hash_box to add particles to
     /pram range how big of a region to extract
  */
  void get_region( int n,hash_box* box, int range) const ;
  /**
     gets a region around box containing particle n and puts the
     results into a hash_box.  This is here because it gets used in
     some places deep in the code.  These should be gotten rid of

     /pram n particle in hash_box to center on 
     /pram box the hash_box to add     particles to 
     /pram range how big of a region to extract
  */
  void get_region( particle* n,hash_box* box, int range) const ;

  /**
     gets a region around box n and puts the particles in to the list handed in

     /pram n hash_box to center on
     /pram out_list list the particles are added to
     /pram range how big of a region to extract
  */
  void get_region( int n,std::list<particle*>& out_list, int range) const ;

  /**
     gets a region around box n and puts the particles in to the vector handed in.
     There is something funny going on with const correctness with this

     /pram n hash_box to center on
     /pram out_vector list the particles are added to
     /pram range how big of a region to extract
  */
  void get_region( int n,std::vector<const particle*> & out_vector,int range) const ;
  ///@}
  /**
     returns the plane number of this shelf
   */  
  int get_plane_num() const
  {
    return plane_number_;
  }
  
  
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
  void compute_mean_forward_disp(utilities::Tuplef & cum_disp);
  /**
     Getter function for mean forward displacement from the
     previous shelf in a case.
   */
  const utilities::Tuplef& get_mean_forward_disp() const{
    return mean_forward_disp_;
  }
  /**
     Getter function for mean forward displacement from the
     first shelf in a case.
  */
  const utilities::Tuplef&  get_cum_forward_disp() const{
    return cumulative_disp_;
  }

  
  /**
     Sets the pointer to the next shelf in a case
   */  
  void set_next_shelf(Hash_shelf* next);
  


  
  /**
    Returns true if there is a shelf n steps forward and
    passes a pointer to it out as a reference, if there is
    not a shelf, returns false.
   */
  bool step_forwards(int n,const Hash_shelf* & next)const;
  
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
  virtual void output_to_wrapper(utilities::Wrapper_out & wrapper) const;
  
  
  /**
     passes a Corr object down the pyramid
   */
  void compute_corr(tracking::Corr &) const ;
  
  /**
     Passes an Accumulator object down the pyramid
   */
  void compute_accum(Accumulator & in)const;
  

  /**
     Testing function to verify that the hash function 
   */
  void test() ;
  

  /**
     This needs to remain beaause of the way that track_list and the
     linking code is written
   */
  std::list<particle_track*> * shelf_to_list() const;

  /**
     Pointer to a Md_store object.  The input wrapper owns this, if it is deleted before
     the hash shelf is done, we are in trouble.
  */
  const utilities::Md_store * md_store_;


private:
  ///Main data structure.  This is an vector of
  ///hash boxes.  For simplicity the strcuture is stored as
  ///a 1-D array and the class takes care of the 1D<->2D conversion
  std::vector<hash_box*> hash_;
  
  ///stores the plane number of the shelf
  int plane_number_;
  
  /**
     The mean displacement of the plane.  The definition of velocity
     is $v_i = x_{i +1} - x_i$ hence the 'forward' velocity
   */
  utilities::Tuplef mean_forward_disp_;

  /**
     Cumlative displacement vector
  */
  utilities::Tuplef cumulative_disp_;

  /**
     Pointer to the next shelf in the case
   */
  Hash_shelf * next_;

  /**
     number of particles in shelf.
     Minor memory hit, but useful to know
   */
  unsigned int particle_count_;

  /**
     Number of particles in the input wrapper.  needed to use filters
     and to write back out to the same file
   */
  int max_part_indx_;
  
  /**
     Dimensions of the hash table
   */
  utilities::Tuplei hash_dims_;
  /**
     The cumulative dimensions for index to cord conversions
   */
  utilities::Tuplei hash_cum_dim_;
  
  /**
     Dimensions of the image
   */
  utilities::Tuplef img_dims_;

  
  
  /**
     size of the side of each hash box
   */
  float upb_;
  
  /**
     The z-off set of the shelf
   */
  float z_offset_;
  
  
  /**
     bool if the hash_box objects in this hash shelf are to own their particles.

     this needs to be kept track of because boxes are created on the fly.
   */
  bool own_part_;
  

  /**
     converts a coordinate tuple in the hash shelf to an index
   */
  int tuple_to_indx(const utilities::Tuplei& cord)const;
  
  /**
     converts an index in the hash shelf to a coordinate tuple 
   */
  utilities::Tuplei indx_to_tuple(int indx) const;
  
  /**
     converts an arbitrary 
   */
  utilities::Tuplei range_indx_to_tuple(int indx,const utilities::Tuplei& side) const;
  
  /**
     private initialization function
   */
  void priv_init();

  /**
     does the computation to get a region of size 2*range +1 around the center
   */
  void make_region_corners(const utilities::Tuplei & center, 
			   utilities::Tuplei & bottom_corner,
			   utilities::Tuplei & top_corner,
			   utilities::Tuplei & region_sides,
			   const int range)const;
  
  
  
};


}
#endif
