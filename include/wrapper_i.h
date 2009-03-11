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
#include <iostream>
#include <map>

#include "wrapper.h"




#ifndef WRAPPER_BASE
#define WRAPPER_BASE

namespace tracking{
//forward declarations
class params_file;
using std::map;
using std::string;
/**
   Abstract base class for input wrappers.  Defines the basic
   functionality that a input wrapper needs to have.  This moatly
   exists to make the polymorphism in/out wrappers type safe.
 */
class wrapper_i_base: public wrapper{

private:

protected:
  

public:
  virtual double get_value(int ind, wrapper::p_vals type)=0;
  virtual void print(int ind);
  //  virtual int num_entries()=0;
  wrapper_i_base(std::map<p_vals,int>map_in):wrapper(map_in){};
  
  virtual ~wrapper_i_base(){};
  

  
};

}

#endif

