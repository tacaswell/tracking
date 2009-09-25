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

#ifndef MASTER_BOX_T
#define MASTER_BOX_T

#include <vector>
#include <iostream>
#include <set>

#include "particle_base.h"
#include "params.h"
#include "enum_utils.h"
#include "wrapper_i.h"
#include "filter.h"

namespace tracking{
/**
   Templated class to hold the master list of all particles to be processed by
   the code.  
 
*/

template <class particle>
class master_box_t{
  
public:
  
  
  ///prints out a representation of this in some useful way
  void print();
    
  ///add next particle
  void push(particle* next){
    particle_vec.push_back(next);
  };

  ///return a pointer to particle in the location
  particle * get_particle(int n){
    return particle_vec.at(n);
  }

  /**
     adds a new type to the data_types
   */
  void append_to_data_types(utilities::D_TYPE type){
    data_types.insert(type);
  }

  
  ///Constructor for a master_box_t based on
  ///data read in from a txt file
  master_box_t(utilities::params* prams_in);


  master_box_t();
    
  ///Returns the total number of particles contained in the
  ///master_box_t.
  unsigned int size(){ return particle_vec.size();}

  ///Cleans up hanging lists from the tracking procedure
  void clean_pos_link();

  /**
     public facing initilization attemting to deal with matlab
   */
  void init(utilities::params* prams_in);


  /**
     initialization to take in a wrapper
   */
  void init(const utilities::Wrapper_in & w_in,utilities::Filter & filt);
  

  ~master_box_t();

protected:
  /**
     Vector that contains pointers to all of the particles that are
     going to be considered in the code.  This class is responcible
     for making all of the objects to represent the particles.  This
     is all done with pointers, not with objects to take anvatage of
     polymorphism.  This class is responcible to creating and destroying
     all of the particle objects.
  */
  std::vector<particle*> particle_vec;
  
  /**
     Pointer to wrapper to take care of particle location
     input data.  This class is responcible for creating
     and destroying this wrapper.  This pointer will be handed
     to every particle that is part of this master_box_t.  The
     exact type of wrapper that is made will be determined by the
     parameter object.
  */
  const utilities::Wrapper_in * in_wrapper;
  
  //imlement this
  unsigned int  imagesz1;
  unsigned int  imagesz2;

  std::set<utilities::D_TYPE> data_types;

  /**
     if the master box owns, and hence needs to delete the wrapper
   */
  bool own_wrapper_;

  /**
     private initialization function
   */
  
  void priv_init();

  /**
     Filter object
   */
  utilities::Filter* filt_;
  
};



template <class particle>
master_box_t<particle>::master_box_t(utilities::params* params_in )
  :in_wrapper(NULL),own_wrapper_(false){
  
  init(params_in);

}

template <class particle>
master_box_t<particle>::master_box_t()
  :in_wrapper(NULL),own_wrapper_(false){
  

}

template <class particle>
void master_box_t<particle>::init(utilities::params* params_in){
  if(in_wrapper!=NULL){
    std::cout<<"can't re-initialize"<<std::endl;
    return;
  }
  own_wrapper_ = true;
  
  in_wrapper = params_in->make_wrapper_in();
  
  priv_init();
}

template <class particle>
void master_box_t<particle>::init(const utilities::Wrapper_in & w_in, utilities::Filter & filt )
{
  if(in_wrapper!=NULL){
    std::cout<<"can't re-initialize"<<std::endl;
    return;
  }
  own_wrapper_ = false;
  
  in_wrapper = &w_in;
  filt_ = &filt;
  filt_->set_wrapper(in_wrapper);
  
  priv_init();
  
}

template <class particle>
void master_box_t<particle>::priv_init()
{
  
  
  data_types = in_wrapper->get_data_types();
  data_types.insert(utilities::D_UNQID);

  
  particle_base::intialize_wrapper_in(in_wrapper);
  particle_base::intialize_data_types(&data_types);

  int total_entries= in_wrapper->get_num_entries(-1);
  particle_vec.reserve(total_entries);
  

  int num_frames = in_wrapper->get_num_frames();
  
  
  for(int k = 0;k<num_frames;++k)
  {
    int num_entries= in_wrapper->get_num_entries(k);
    std::cout<<"adding: "<<num_entries<<std::endl;
    
    for(int j = 0; j<num_entries; ++j){
      if((*filt_)(j,k))
      {
	particle_vec.push_back( new particle(j,k));
      }
      
    }
  }
}


template <class particle>
void master_box_t<particle>::print(){
  for(unsigned int j = 0; j<particle_vec.size();++j)
    particle_vec.at(j)->print();
}

template <class particle>
master_box_t<particle>::~master_box_t(){
  //deletes the particles it made
  for(unsigned int j = 0; j<particle_vec.size();++j)
    {
      delete particle_vec.at(j);
    }
  //deletes the wrapper objects
  if(own_wrapper_)
  {
    delete in_wrapper;
  }
  
  
//   std::cout<<"mb dead"<<std::endl;
}


template <class particle>
void master_box_t<particle>::clean_pos_link(){
  for(unsigned int j = 0; j<particle_vec.size();j++){
    delete particle_vec.at(j)->pos_link;
    particle_vec.at(j)->pos_link = NULL;
  }
}
}


#endif
