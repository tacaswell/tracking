//Copyright 2011 Thomas A Caswell
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


%module cstatic2
%{
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#define DIM_COUNT 2
#define PTYPE 1


#include "enum_utils.h"
#include "wrapper_i.h"
#include "wrapper_i_hdf.h"
#include "wrapper_i_generic.h"
#include "filter.h"
#include "hash_case.h"
#include "corr.h"
#include "corr_gofr.h"
#include "part_def.h"
#include "tuple.h"

%}

namespace std
{
  %template(IntVector) vector<int>;
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

namespace utilities
{
  enum D_TYPE
  {
  D_INDEX               =0,	// index of particle in case,
				// (frame,indx) is unique and should
				// be persistent in wrappers (wrapper)
  D_XPOS  		=1,	// x-position (plu)
  D_YPOS  		=2,	// y-position (plu)
  D_FRAME  		=3,	// frame (wrapper)
  D_I  			=4,	// integrated intensity (plu)
  D_R2  		=5,	// radius of gyration (plu)
  D_E 			=6,	// eccentricity (plu)
  D_NEXT_INDX 		=7,	// index of the next particle in the
				// track (track)
  D_PREV_INDX  		=8,	// index of the previous particle in
				// the track (track)
  D_DX  		=9,	// x-shift (plu)
  D_DY  		=10,	// y-shift (plu)
  D_ORGX  		=11,	// original x-position (computed)
  D_ORGY  		=12,	// original y-position (computed)
  D_ZPOS 		=13,	// z-position (link or meta-data)
  D_TRACKID 		=14,	// track id (track)
  D_S_ORDER_PARAMETER 	=15,	// scalar order parameter (computed)
  D_MULT 		=16,	// multiplicity (plu)
  D_N_SIZE  		=17,	// neighborhood size (computed)
  D_SENTRY			// this entry must remain last, and I
				// am making assumptions about how the
				// numbering will work in the compiler
  
    };
%include "tuple.i"
typedef Tuple<float,DIM_COUNT> Tuplef;
%template (Tuplef) Tuple<float,DIM_COUNT>;
   
 


class Wrapper_in
{
public:
  virtual int get_num_entries(int frame) =0;
  virtual int get_num_entries() =0;
  virtual int get_num_frames()  =0;
  virtual ~Wrapper_in();
};
 
class Wrapper_i_hdf: public Wrapper_in
{
public:
  int get_num_entries(int frame);
  int get_num_entries();
  int get_num_frames();

  Wrapper_i_hdf();
  ~Wrapper_i_hdf();
  
  bool set_file_name(const std::string & fname);
  bool add_dtype(utilities::D_TYPE dtype,int comp_key);
  bool set_twoD(bool twod_data);
  bool initialize(int f_count);

};
  


class Wrapper_i_generic:public Wrapper_in{
 public:
  Wrapper_i_generic();  
  ~Wrapper_i_generic();

  int get_num_entries(int frame);
  int get_num_entries();
  int get_num_frames();


  bool setup(std::set<D_TYPE>,int n,const Tuplef & d);

  bool open_frame(unsigned int frame,int N,float z);
  bool set_data_type(D_TYPE dtype);
%apply (int* IN_ARRAY1, int DIM1) {(int* data, int N)}  
  bool add_int_data(int * data,int N);
%clear (int* vector, int length);
%apply (float* IN_ARRAY1, int DIM1) {(float* data, int N)}
  bool add_float_data(float * data,int N);
%clear (float* vector, int length);
  //  bool set_meta_data(const Md_store & md_store);
  bool close_frame();
  bool finalize_wrapper();
  

};
 




 


class Filter
{
public:
  virtual bool operator()(int index,int frame) const = 0;
  virtual void set_wrapper(const Wrapper_in * w_in)= 0;
  virtual ~Filter();
  
};


class Filter_trivial : public Filter
{
public:
  bool operator()(int index,int frame) const;
  void set_wrapper(const  Wrapper_in* w_in);
  Filter_trivial();
  ~Filter_trivial();

  
};


class Filter_basic : public Filter
{
public:
  bool operator()(int index,int frame) const;
  void set_wrapper(const Wrapper_in * w_i );
  Filter_basic();
  ~Filter_basic();

  void init(const std::string&,int);
  void init(float ecut,float rg_cut,float shift_cut);
  
    
  
 };
   
}
 
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
 

class hash_case
{
public:
  void init(utilities::Wrapper_in & w_in , utilities::Filter & filt, float ppb)  ;
  void compute_corr(tracking::Corr &) const ;
  
  int get_num_frames() const;
  void print() const; 
  hash_case();
  ~hash_case();
  
};
 
  
 

}