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



#ifndef HASH_BOX
#define HASH_BOX
#include <vector>
#include <list>

#include "part_def.h"
#include "enum_utils.h"
#include "part_def.h"

namespace utilities
{
class Wrapper_out;


}


namespace tracking{

class Corr;
class hash_shelf;

/**
   Class for storing particles in the hash table.  This class will
   hold a list of any  generation of particles.  The primary purpose
   of this class is to sit in a shelf that corresponds to a hash table
   of the data and to be passed out of shelf functions as a list of
   the particles selected.
*/
class hash_box{
protected:
  ///Contents of the box
  std::vector<particle*> contents_;
  
  /**
     pointer to the hash_shelf that contains this box
   */
  const hash_shelf  * shelf_;
  
  /**
     position in hash shelf 
   */
  int hash_indx_;
  
public:
  
  ///@name over all control
  ///Constructors, destructor, and clear all data
  //@{ 

  ///constructor with hash_shelf information
  hash_box(const hash_shelf* in,int hash_indx):contents_(),shelf_(in),hash_indx_(hash_indx){

  }
  /**
     empty constructor
   */
  hash_box():contents_(),shelf_(NULL),hash_indx_(-1){
    }

  ///Destructor.  Clears all the pointers to try and avoid stale pointers
  ~hash_box(){
    contents_.clear();
  }

  ///clears all the pointers, but doesn't destroy the box.
  void clear(){contents_.clear();}


  
  

  ///add next particle
  void push(particle* next){
    contents_.push_back(next);
  };


  ///appends the box 'next' to this box.
  void append(hash_box* next);
  
  
  ///prints out a representation of this in some useful way
  void print();

  //@}

  ///@name Getter Functions
  ///@{

  ///Returns the size of the box, ie number of particles
  int get_size(){return contents_.size(); }
  
  /**
     Returns a vector of the data of p_vals type for the box.
   */
  void get_val_vec(std::vector<float> & vec, utilities::D_TYPE type);

  /**
     Returns an iterator to the front of the contents vector,
     a-la the vector.begin() function.

     figure out who uses this function is used and make private or kill
   */
  std::vector<particle*>::iterator begin(){
    return contents_.begin();
  }
  /**
     a very const version
   */
  std::vector<particle*>::const_iterator begin()const{
    return contents_.begin();
  }
  /**
     Returns an iterator to past the end of the contents vector,
     a-la the vector.end() function.
     
     figure out who uses this function is used and make private or kill
   */
  std::vector<particle*>::iterator end(){
    return contents_.end();
  }

  /*
     Returns a pointer to a new list of the pa rticle_track* pointers
     for dealing with tracking.  This always returns a valid pointer
     to a list.  If the box is empty then a pointer to an empty list
     is returned.  This function allocates heap that needs to be cleaned
     up!
   */
  //  std::list<par ticle_track*>* box_to_list() const;

  /*
     A safer version of converting a hash_box to a list
   */
  //void  box_to_list(std::list<part icle_track*>& p_list) const;
  /**
     A safer version of converting a hash_box to a list
   */
  void  box_to_list(std::list<particle*>& p_list) const;
  
  //lots more of these along different components, likely reason to extend
  //these critters, however I haven't decided the most effective method of
  //returning this data, and it may very well depend on exactly what I am going
  //to do with it because I haven't addressed how to handle the IPP
  //junk

  ///@}
  
  
  ///@name g(r) functions
  ///@{
  /**Returns the number of particles that were used as centers and
   * returns the results by reference.  g(r) is computed for the
   * particles contained in *this against the particles contained in
   * *points.  The results are returned in two vectors, bin_count is
   * the number of particles in each bin and bin_r is the radius of
   * the inner edge of each bin. It takes two arguments, max_d is the
   * maximum distance to consider, nbins is the number of bins.  This
   * function uses linearly distributed boxes.  This function only
   * uses free code.
   * 
   * This code assumes that bin_count and bin_r contain junk and need
   * to be initialized.  Any data in them will be cleared

   * the underlying data structures should be clever enough work out
   * how many dimensions they are in.
  */
  int gofr(float max_d, int nbins, hash_box* points,
	   std::vector<float>& bin_count,std::vector<float>& bin_r);


  ///Same as gofr, but assumes that bin_count is properly initialized
  int gofr(float max_d, int nbins, hash_box* points,
	   std::vector<float>& bin_count);

  ///Same as gofr, with the bin count vector is normalized with
  ///respect to area
  int gofr_norm_a(float max_d, unsigned int nbins, hash_box* points,
	   std::vector<float>& bin_count,std::vector<float>& bin_r);
  
  ///Same as gofr_norm_a, with the bin count vector is normalized with
  ///respect to area and the local average density
  int gofr_norm_an(float max_d, int nbins, 
		   std::vector<float> & bin_count,std::vector<float>& bin_r);


  ///Returns g(r).  The information is returned in two vectors,
  ///bin_count is the number of particles in each bin and bin_r is the
  ///radius of the center of each bin. It takes two arguments, max_d
  ///is the maximum distance to consider, nbins is the number of bins.
  ///This function uses linearly distributed boxes.  This function
  ///uses intel code
  void gofr_ipp(unsigned int max_d, unsigned int nbins, 
       std::vector<int> bin_count,std::vector<float> bin_r);
  
  /**
     computes 2D histogram 
   */
  int gofr2D(float max_d, int nbins, hash_box* points,
		       std::vector<float>& bin_count);
    
  ///@}

  /**
     Passes functions down the pyramid, this one for void, argument-less
     functions noncosnst
   */
  void pass_fun_to_part(void(particle::*fun)());
  /**

     Passes functions down the pyramid, this one for void, argument-less
     functions const
   */
  void pass_fun_to_part(void(particle::*fun)()const)const;

  /**
     loops over particles to 
   */
  void output_to_wrapper(utilities::Wrapper_out & wrapper) const;

    
  /**
     passes a Corr object down the pyramid
   */
  void compute_corr(tracking::Corr &) const ;
  
};
}
#endif

  //random ideas
  /*
    Implement a g(r) struct that will hold the the count and r for
    each bin
   */