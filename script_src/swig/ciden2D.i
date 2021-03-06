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



%module ciden2
%{
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#define DIM_COUNT 2
#define PTYPE 0


#include "generic_wrapper.h"
#include "generic_wrapper_hdf.h"
#include "md_store.h"
#include "enum_utils.h"
#include "wrapper_i.h"
#include "wrapper_i_hdf.h"
#include "wrapper_i_generic.h"
#include "filter.h"
#include "hash_case.h"
#include "part_def.h"
#include "iden.h"
#include "image_base.h"
#include "image_stack.h"
#include "image1.h"
#include "image_series.h"
#include "md_parser.h"
#include "mm_md_parser.h"
#include "wrapper_i_plu.h"

using std::string;
using std::vector;
using std::set;


 
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

namespace iden
{

 
 

class  MD_parser
{
public:
  MD_parser(){};
  virtual  ~MD_parser(){};
  virtual utilities::Md_store * parse_md(const fipImage & img_in) const=0 ;
    
  
};
class Mm_md_parser : public MD_parser
{
public:
  Mm_md_parser();
  ~Mm_md_parser();
  utilities::Md_store * parse_md(const fipImage & img_in) const ;
 };
 
 
class Image_base
{
public:
  virtual ~Image_base();
  virtual int get_frame_count() const = 0;  
};

class Image_stack:public Image_base
{
public:
  Image_stack(std::string fname);
  int get_frame_count() const;  
  ~Image_stack();
 
  void initialize();
  void deinitialize();

};
 
class Image_series:public Image_base
{
public:
  Image_series();
  ~Image_series();
  int get_frame_count() const;  
  bool init(const std::string & base_name);
  
 } ;
 

 
class Wrapper_i_plu: public Wrapper_in
{
public:
  int get_num_entries(int frame);
  int get_num_entries();
  int get_num_frames();

  
  Wrapper_i_plu(int);    
  Wrapper_i_plu(int,utilities::Tuplef);    
  ~Wrapper_i_plu();
  void set_dims(const utilities::Tuplef&);


  int get_value(int& out,int ind,utilities::D_TYPE type, int frame) const;
  float get_value(float &out,int ind, utilities::D_TYPE type,int frame )const;
  %extend
   {
     float get_value_f(int ind, utilities::D_TYPE type,int frame )const
     {
       float tmp;
       tmp = $self->get_value(tmp,ind,type,frame);
       return tmp;
     }
   }
  
};

class Image2D
{
public:

  Image2D(const int image_length, const int image_width);
  /* Image2D(const Image2D & other); */
  /* Image2D(const float * IN_ARRAY2,int DIM1,int DIM2); */
  ~Image2D();
  
  /* void set_data(const Image_base & image); */
  /* void add_data(const Image_base & image); */
  %apply (float * IN_ARRAY2,int DIM1,int DIM2){(const float* data, int n, int m)}  ;
  void set_data(const float * data,int n,int m);
  //  void add_data(const float * data,int n,int m);
  %clear (const float* data, int n, int m);
  
  /* Ipp32f *get_image2D() const; */
  /* int get_stepsize() const; */
  /* int get_width() const; */
  /* int get_numberofpixels() const; */
  /* int get_height() const; */
  
};


 class Iden
{
public:
  Iden(const utilities::Md_store& in);
  ~Iden();
  
  void set_image_src(Image_base * image);
  void set_md_parser(MD_parser * parser);
  void set_params(const utilities::Md_store& param_in);
  
  Wrapper_i_plu * fill_wrapper(unsigned int frames=0,unsigned int start=0);
  Wrapper_i_plu * fill_wrapper_avg(unsigned int frames=0,unsigned int start=0);


  void process_frame(const Image2D & img_in,
                     unsigned int frame_number,
                     const utilities::Md_store * md_store_in,
                     Wrapper_i_plu & wrapper_out) const;
  
};
 
 
}
