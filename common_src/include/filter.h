//Copyright 2009 Thomas A Caswell
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

#ifndef FILTER
#define FILTER
#include <string>

namespace utilities
{
class Wrapper_in;
class Md_store;

/**
   ABC for filter objects.  They take in a the index and frame of particle
   in a returns a bool, true if the particle is good, false if it is bad.
 */
class Filter
{
public:
  /**
     Returns true if the particle at index in frame passes what every
     criteria that the subclasses implement.
   */
  virtual bool operator() (int index,int frame)const = 0;
  /**
     Sets pointer to Wrapper_in to read data from
   */
  virtual void set_wrapper(const Wrapper_in * )=0;
  /**
     Destructor
   */
  virtual ~Filter(){};
  /**
     Empty constructor 
   */
  Filter(){};
};


/**
   a trivial filter class that does nothing
 */
class Filter_trivial:public Filter
{
public:
  bool operator()(int,int)const
  {
    return true;
  }
  virtual void set_wrapper(const Wrapper_in * ){};
  Filter_trivial(){};
  ~Filter_trivial(){};
};

  



/**
   A basic filter, does rg, shift and eccentricity cuts
 */
class Filter_basic:public Filter
{
public:
  bool operator()(int ind,int frame) const;
  /**
     Constructor to read threshold values from a HDF file.
   */
  Filter_basic(const std::string&,int);
  /**
     Constructor that just sets the threshold values
   */
  Filter_basic(float ecut,float rg_cut,float shift_cut);
  void set_wrapper(const Wrapper_in * w_i )
  {
    wrap_ = w_i;
  }
  
  /**
     Returns the filter parameters in a md_store object
   */
  Md_store get_parameters()const;
  
  /**
     Destructor
   */
  ~Filter_basic()
  {
  }
private:
  /**
     Eccentricity threshold
   */
  float e_cut_;
  /**
     Radius of gyration threshold
   */
  float rg_cut_;
  /**
     Centroid shift threshold
   */
  float shift_cut_;
  /**
     Wrapper to read particle data from
   */
  const Wrapper_in * wrap_;
};



}
#endif
