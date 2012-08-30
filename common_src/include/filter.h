//Copyright 2009-2011 Thomas A Caswell
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
#include <vector>

namespace utilities
{
class Wrapper_in;
class Md_store;

/**
   Enumeration for keeping track of which filter type a filter is,
useful for checking to make sure that the factory behaved properly.
 */
typedef enum FILT_TYPE{
  FILT_TRIV = 0,		// trivial, always returns true
  FILT_ERS, 			// e_cut(top),rg_cut(top),shift_cut(top)
  FILT_ERSImin,			// e_cut(top),rg_cut(top),shift_cut(top), I_cut(bottom)
} FILT_TYPE;


/**
   ABC for filter objects.  
   
   Filter objects act as a (partial) layer on top of the wrapper
   objects to allow only particles that meet criteria to even be
   created.  There are probably better ways to factor this code.
   
   They take in a the index and frame of particle in a returns a bool,
   true if the particle is good, false if it is bad.
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
     Returns filter type
   */
  virtual  FILT_TYPE get_type()const = 0;
  
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
  void set_wrapper(const Wrapper_in * ){};
  FILT_TYPE get_type()const{return FILT_TRIV;};
  Filter_trivial(){};
  ~Filter_trivial(){};
};



/**
   A basic filter, does rg (top), shift (top) and eccentricity (top) cuts
 */
class Filter_ers:public Filter
{
public:
  bool operator()(int ind,int frame) const;
  /**
     Constructor
   */
  Filter_ers();
  /**
     Initialization from an Md_store
   */
  void init(const Md_store & md_store);
  void set_wrapper(const Wrapper_in * w_i )
  {
    wrap_ = w_i;
  }
  FILT_TYPE get_type()const{return FILT_ERS;};
  /**
     Returns the filter parameters in a md_store object
   */
  Md_store get_parameters()const;
  
  /**
     Destructor
   */
  ~Filter_ers()
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

  

/**
   A basic filter, does rg (top), shift (top), eccentricity (top), and
   I (bottom) cuts
 */
class Filter_ersI:public Filter
{
public:
  bool operator()(int ind,int frame) const;
  /**
     Constructor
   */
  Filter_ersI();
  /**
     Initialization from an Md_store
   */
  void init(const Md_store & md_store);
  void set_wrapper(const Wrapper_in * w_i )
  {
    wrap_ = w_i;
  }
  FILT_TYPE get_type()const{return FILT_ERSImin;};
  /**
     Returns the filter parameters in a md_store object
   */
  Md_store get_parameters()const;
  
  /**
     Destructor
   */
  ~Filter_ersI()
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
     Minimum total intensity
   */
  float I_min_cut_;
  
  /**
     Wrapper to read particle data from
   */
  const Wrapper_in * wrap_;
};

  



/**
   Guesses which filter to generate from the parameters it is given in
   the Md_store object.

   The returned object needs to be cleaned up.
 */
Filter * filter_factory(const Md_store & filter_prams);

/**
   Extracts filter parameters from a data file.  Extracts the
   parameters asked for.  This functionality should be folded into the
   wrapper class.
 */
Md_store extract_prams(const std::string & fname,int comp_num, const std::vector<std::string> & pram_list);


}
#endif
