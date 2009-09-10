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
#include <set>
#include <map>

#ifndef WRAPPER_FLAT
#define WRAPPER_FLAT
namespace utilities{
                                                                               
   

/**
   Abstract base class from which all of the particle wrappers are derived.
   The family derived from wrapper_generic should be used for non-particle
   output (ie, vectors, representation of sets/maps, etc)
   This exists mostly to be a common place to define the enumeration of
   data types
*/

class Wrapper_flat{
public:
  
  
  
  Wrapper_flat(std::map<p_vals,int>map_in);
  Wrapper_flat(){};
  virtual ~Wrapper_flat(){};
  
  
protected:
  ///Boolean value to determine if this is a cloned wrapper or not.
  ///If the data structure of this is pointing at the same chunk of memory
  ///as another wrapper, then it only needs to be deleted once.
  ///Currently none of this is implemented
  bool clone;
  /**
     A map between the data types and a posistion in the data structure.
     This orginally had been burried down in the derived classes, but 
     I couldn't think of a data structure for storing particle data where
     this sort of thing wasn't uesful.  Thus, it has been dragged up to
     the top level.
   */
  std::map<p_vals, int> data_types_;
  int data_layout_[16];
};
}
#endif

 
