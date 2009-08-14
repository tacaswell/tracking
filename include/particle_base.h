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
//standard incldues
#include <vector>
#include <iostream>
#include <set>
#include <list>

#include <complex>

//local includes
#include "wrapper.h"
#include "tuple.h"


#ifndef PARTICLE_BASE
#define PARTICLE_BASE
namespace tracking{
class wrapper_i_base;
class wrapper_o_base;

/**
   Base class for particles. Defines the basic functions
   a particle must have.  The reason that the wrapper_i class has been
   split off from this class has been to seperate the implemntation of
   copeing with data structures coming in from the derived classes of
   the particles (to carry extra information about the particles for
   tracking pruposes).  Need to roll my own dll class

 */
class particle_base{
  
public:
    
  ///Default constructor
  //clean this up/use inline intilization
  //  particle_base(wrapper_i_base * i_data, wrapper_o_base* o_out, 
  //	int i_ind);

  /**
     move to this being the primary constructor, get rid of the other
     one
   */
  particle_base(int i_ind);

  /**
     Constructor that takes the position of the particle. 
   */
  particle_base(int ind, utilities::Tuple pos,int frame=0);
  
  

  particle_base(const particle_base &p);

  
  
  ///default Destructor, made virtual
  virtual ~particle_base(){}


  ///A program generated global unique id, useful for tracking, comparing
  ///particles and that sort of thing that does not depends on
  ///properly parsing the data or the data being handed in behaving in
  ///any sort of nice way, but this does mean that the particle
  ///objects just got 32bits bigger
  int unq_id_;

  ///Pring out a rasonable representation of the partile
  virtual void print() const;

  ///Store this particle in the output wrapper structure
  virtual void set_particle() const;
  

  ///Returns the value of 'type' for the particle as specified
  ///by the in_wrapper
  virtual double get_value(wrapper::p_vals type) const;
  /**
     returns a tuple of the particle's position
   */
  const utilities::Tuple & get_position() const{
    return position_;}
  /**
     Retruns the distance from this particle to part_in
   */
  virtual double distancesq(const particle_base* part_in) const;
  /**
     Returns the distance of the particle from the specified origin
     @param origin the cordinates of the new origin
   */
  double get_r(const utilities::Tuple & origin) const;
  /**
     Returns the angle in the plane from the Y-axis
     @param origin the cordinates of the new origin
   */
  double get_theta(const utilities::Tuple & origin) const;


  /**
     Sorts the neighborhood vector in ascending order 
   */
  void sort_neighborhood();
  
  /**
     checks to make sure the neighborhood does not include any repeats, returns true
     if the neighborhood is repeat free.  Mostly for debugging
   */
  bool no_neighborhood_repeats()const;
  
  /**
     adds a particle to the neighborhood if it is close enough
     returns true if the particle is added, false otherwise
   */
  bool add_to_neighborhood(const particle_base* in);
  /**
     returns the maximum neighborhood range
   */
  static float get_max_range()
  {
    return max_neighborhood_range_;
    
  }


  /**
     Fills the order parameter with \f$\Phi_6\f$.
     See compute_phi_6()
   */
  void fill_phi_6();
  /**
     computes and returns the order parameter \f$\Phi_6\f$.
     Defined by
     \f[
     \Phi_{6}(r_{m}) = \frac{1}{N_{b}}\sum_{n=1}^{N_{b}}e^{6 i\theta_{mn} }
     \f]    
   */
  std::complex<float> compute_phi_6()const;
  


  

  /**
     Intialize the static input wrapper for all particles
   */
  static void intialize_wrapper_in(const wrapper_i_base* in);
  /**
     Intialize the static output wrapper for all particles
   */
  static void intialize_wrapper_out(wrapper_o_base* out);
  /**
     Intialize the static data types for all particles
   */
  static void intialize_data_types(std::set<wrapper::p_vals>*  data_types);

  


protected:
  ///A running total of all particles created 
  static int running_total_;
  ///object that takes care of all the underling data structures.
  const static wrapper_i_base* wrapper_in_;
  ///object that takes care of 
  static wrapper_o_base* wrapper_out_;
  ///Vector of the types of data that
  ///remove this

  static std::set<wrapper::p_vals>* data_types_;
  ///Identifier that comes from the wrapper
  int ind_;
  
  /**
     New attempt at storing the position data, this has the 
     problem of taking more memory, but it might help the time.
     This will also make keeping track of the displacements.
   */
  utilities::Tuple position_;
  
  /**
     The frame of the particle
  */
  int frame_;


  /**
     Vector of the particles with in max_range_ in the hash_shelf_ of
     the particle.
   */
  std::vector<const particle_base*> neighborhood_;
  
  /**
     Maximum distance to be part of the neighborhood
   */
  static float max_neighborhood_range_;

  /**
     Scaler order parameter, ex phi_6 in 2D
   */
  std::complex<float> s_order_parameter_;
  

  
private:
  void fill_position();
  void priv_init(int i_ind);

  /**
     function for sorting the vectors in ascending order, returns true
     if the first argument goes before the second argument in the
     specific strict weak ordering it defines, and false otherwise.

   */
  bool lthan(const particle_base* a,const particle_base* b)const;
  
  
};
}
#endif


/*
#ifndef PARTICLE_NEI
#define PARTICLE_NEI



   Class for particles that carry around with them their neighbor
   information.  Implements this list as a vector, this is just here
   so I don't forget I planned this and try to re implement it a
   different way.
 
class particle_neighbor : public particle_track{
protected:
  vector <particle_neighbor*> neighbors;
public:
  //getter, setter, etc functions
};


#endif
*/
