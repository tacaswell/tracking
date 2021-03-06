//Copyright 2008-2011 Thomas A Caswell
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

#ifndef MASTER_BOX_T
#define MASTER_BOX_T

#include <vector>
#include <iostream>
#include <set>


#include "enum_utils.h"
#include "part_def.h"

namespace utilities
{
class Wrapper_in;
class Md_store;

class Filter;


}


namespace tracking{
class Accumulator;
/**
   Class to hold the master list of all particles to be processed by
   the code.  
 
*/
class Master_box{
  
public:
  
  
  ///prints out a representation of this in some useful way
  void print();
    
  ///add next particle
  void push(particle* next){
    particle_vec_.push_back(next);
  };

  ///return a pointer to particle in the location
  particle * get_particle(int n){
    return particle_vec_.at(n);
  }

  /**
     adds a new type to the data_types
   */
  void append_to_data_types(utilities::D_TYPE type){
    data_types.insert(type);
  }

  /**
     Constructor
   */
  Master_box();
    
  ///Returns the total number of particles contained in the
  ///Master_box.
  unsigned int size(){ return particle_vec_.size();}

  ///Cleans up hanging lists from the tracking procedure
  void clean_pos_link();


  /**
     initialization to take in a wrapper
   */
  void init(const utilities::Wrapper_in & w_in,utilities::Filter & filt);
  
  /**
     Puts all of the particles in to the Accumulator
   */
  void compute_accum(tracking::Accumulator & in)const;
  
  /**
     Returns a pointer to the  Md_store for plane j in the input wrapper
   */
  const utilities::Md_store * get_Md_store(unsigned int j) const;
    
  /**
     Destructor
   */
  ~Master_box();

protected:
  /**
     Vector that contains pointers to all of the particles that are
     going to be considered in the code.  This class is responcible
     for making all of the objects to represent the particles.  This
     is all done with pointers, not with objects to take anvatage of
     polymorphism.  This class is responcible to creating and destroying
     all of the particle objects.
  */
  std::vector<particle*> particle_vec_;
  
  /**
     Pointer to wrapper to take care of particle location
     input data.  This class is responcible for creating
     and destroying this wrapper.  This pointer will be handed
     to every particle that is part of this Master_box.  The
     exact type of wrapper that is made will be determined by the
     parameter object.
  */
  const utilities::Wrapper_in * in_wrapper_;
  
  //imlement this
  ///@todo sort out this member
  unsigned int  imagesz1;
  ///@todo sort out this member
  unsigned int  imagesz2;

  /**
     A set of the data types known by particles in the Master_box
   */
  std::set<utilities::D_TYPE> data_types;

  /**
     if the master box owns, and hence needs to delete the wrapper
   */
  bool own_wrapper_;

  /**
     private initialization function
   */
  
  void priv_init(utilities::Filter const*  const filt);
  
};
}



#endif
