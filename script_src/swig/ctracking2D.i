//Copyright 2012 Thomas A Caswell
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

%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
%}

  

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"

%include "exception.i"


%module ctracking2
%{
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#define DIM_COUNT 2
#define PTYPE 1


#include "generic_wrapper.h"
#include "generic_wrapper_hdf.h"
#include "md_store.h"
#include "enum_utils.h"
#include "wrapper_i.h"
#include "wrapper_i_hdf.h"
#include "wrapper_i_generic.h"
#include "filter.h"
#include "hash_case.h"
#include "corr.h"
#include "corr_gofr.h"
#include "part_def.h"

#include "track_shelf.h"
#include "track_accum.h"
#include "ta_vanHove.h"
%}

namespace std
{
  %template(IntVector) vector<int>;
  %template(UIntVector) vector<unsigned int>;
  %template(DoubleVector) vector<double>;
  %template(FloatVector) vector<float>;
  %template(Dset) set<utilities::D_TYPE>;
  
}

%exception{
  try  {
    $action
  }
  catch(const std::exception&  e)
  {
    SWIG_exception(SWIG_RuntimeError,e.what());
  }

}

%include "utilities.i"
 
namespace tracking
{

class Corr
{
 public:
  virtual float get_max_range() const = 0;
  Corr();
  virtual ~Corr();
};
 
class Corr_gofr :public Corr
{
 public:
  float get_max_range() const;
  float normalize(std::vector<float> & gofr) const;
  void get_bin_edges(std::vector<float> & out) const;
  Corr_gofr(int bins,float max);
  
};

 
class Trk_accumulator
{
public:
  virtual void add_disp(utilities::Tuplef displacement,unsigned steps) = 0;
  virtual unsigned max_step()const = 0;
  
};

 
class TA_vanHove: public Trk_accumulator
{
public:
  void add_disp(utilities::Tuplef displacement,unsigned steps);
  unsigned max_step()const ;
  TA_vanHove(unsigned max_steps,
	     unsigned nbins,
	     float max_range);
  ~TA_vanHove();
  void output_to_wrapper(utilities::Generic_wrapper & out,
			 const utilities::Md_store & md_store)const;


};


class Track_shelf{ 
 public:
  void remove_short_tracks(int min_length);
  void renumber();
  unsigned int get_track_count() const;
  void compute_corrected_TA(Trk_accumulator & ta)const;
  
  Track_shelf();
  ~Track_shelf();
  
};
 


class hash_case
{
public:
  void init(utilities::Wrapper_in & w_in , utilities::Filter & filt, float ppb)  ;
  void compute_corr(tracking::Corr &) const ;
  void link(float max_range,Track_shelf & tracks);
  void compute_mean_disp();

  int get_num_frames() const;
  void print() const; 
  hash_case();
  ~hash_case();
  
};
 

}

