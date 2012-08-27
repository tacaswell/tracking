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

#include "ta_disp.h"

#include "accumulator.h"
#include "accum_chi4_self.h"
#include "accum_case.h"

using std::string;
using std::vector;
using std::set;

using tracking::w_step;
 
%}

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

class Accumulator
{
public:
  virtual void out_to_wrapper(utilities::Generic_wrapper &,
                              const utilities::Md_store & md_store ) const =0;
  virtual ~Accumulator(); 
  Accumulator() ;
  
  

};

 class Accum_chi4_self : public Accumulator
{
public:


  void out_to_wrapper(utilities::Generic_wrapper &,
                      const utilities::Md_store & md_store ) const ;

  // special stuff
  
  Accum_chi4_self(unsigned max_t,float min_l,float max_l,unsigned int l_steps,float (*w)(float,float));

  ~Accum_chi4_self();
  
  void  add_to_chi4(std::vector<float>& Q_accum,std::vector<float>& Q2_accum,const int time_steps)const;
} ;

 %constant float w_step(float,float);
}

namespace utilities
{
  
class Accum_case
{
public:


  Accum_case();
  
  
  template <class T> void  fill(const T & base_obj,const unsigned int frame_count)
  {
    accum_vec_.resize(frame_count);
    for(unsigned int j = 0; j<frame_count;j++)
      accum_vec_[j] = new T(base_obj);
    frame_count_ = accum_vec_.size();
    
  };
  
  %extend
   {
     void fill_chi4(const tracking::Accum_chi4_self &base_obj ,const unsigned int frame_count ){
         $self->fill(base_obj,frame_count);
       }
   }
  
       



  tracking::Accumulator* at(int j);
   

 

  unsigned int size()const;
  ~Accum_case();

} ;
}

namespace  tracking
{

class Trk_accumulator
{
public:
  virtual void add_disp(const utilities::Tuplef & displacement,unsigned steps) = 0;
  virtual unsigned max_step()const = 0;
  
};

 
class TA_vanHove: public Trk_accumulator
{
public:
  void add_disp(const utilities::Tuplef & displacement,unsigned steps);
  unsigned max_step()const ;
  TA_vanHove(unsigned max_steps,
	     unsigned nbins,
	     float max_range);
  ~TA_vanHove();
  void output_to_wrapper(utilities::Generic_wrapper & out,
			 const utilities::Md_store & md_store)const;


};

class TA_disp: public Trk_accumulator
{
public:
  void add_disp(const utilities::Tuplef & displacement,unsigned steps);
  unsigned max_step()const ;
  
  TA_disp(unsigned max_steps,
	     unsigned r_nbins,
             float max_r,
	     unsigned t_nbins);
  ~TA_disp();
  void output_to_wrapper(utilities::Generic_wrapper & out,
			 const utilities::Md_store & md_store)const;


};
 
class hash_case;
 

class Track_shelf{ 
 public:
  void remove_short_tracks(int min_length);
  void renumber();
  unsigned int get_track_count() const;
  void compute_corrected_TA(Trk_accumulator & ta)const;
  void init(hash_case & hc);
  Track_shelf();
  ~Track_shelf();
  
};
 


class hash_case
{
public:
  void init(utilities::Wrapper_in & w_in , utilities::Filter & filt, float ppb)  ;
  void init(float box_side_len, 
            const utilities::Wrapper_in & w_in ,
            Track_shelf & tracks,
            utilities::Filter & filt, 
            int min_trk_length)  ;
  void compute_corr(tracking::Corr &) const ;
  void link(float max_range,Track_shelf & tracks);
  void compute_mean_disp();
  void compute_accum(utilities::Accum_case & )const;
  int get_num_frames() const;
  void print() const; 
  hash_case();
  ~hash_case();
  
};
 

}

