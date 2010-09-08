//Copyright 2009,2010 Thomas A Caswell
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

//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

#ifndef CORR_
#define CORR_

#include <vector>

#include "part_def.h"

namespace utilities
{
class Generic_wrapper;
}





namespace tracking
{



/**
   A mostly ABC of objects that can be handed into hash_case and calculates
   correlation functions.  This class deals with temperature meta-data
 */
class Corr
{
public:
  /**
     takes in a particle_base object.  For correlations that need the
     tracking information this should throw an error or something
     clever like that
   */
  virtual void compute(const particle *,const std::vector<const particle*> & ) = 0;

  /**
     outputs the result of the computation to the wrapper
   */
  virtual void out_to_wrapper(utilities::Generic_wrapper &,const std::string & ) const =0;
  /**
     returns the maximum range that the Corr object looks out to.  This is needed so
     that the Hash_shelf can properly limit the particles that it hands in
   */
  virtual float get_max_range() const = 0;

  /**
     Adds the temperature for a plane. 
   */
  void add_plane_temperature(float temp);
  /**
     Destructor
   */
  virtual ~Corr(){};
protected:
  Corr();
  /**
     The sum of the temperatures added
   */
  float temperature_sum_;
  /**
     Count of the number of temperatures added
   */
  unsigned int plane_count_;
  
};

/**
   A subclass of Corr that does nothing.  This is useful for testing
 */
class Dummy_corr:public Corr
{
public:
  void compute(const particle *,const std::vector<const particle*> & ) {};

  void out_to_wrapper(utilities::Generic_wrapper &,const std::string & ) const{}
  
  
  virtual float get_max_range() const{return size_;};

  ~Dummy_corr(){};
  Dummy_corr(float in):size_(in){};
  
  
private:
  float size_;
  
};

  

}


#endif
