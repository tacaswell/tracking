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

#ifndef WRAPPER_
#define WRAPPER_
namespace tracking{
/**
   Abstract base class from which all of the particle wrappers are derived.
   The family derived from wrapper_generic should be used for non-particle
   output (ie, vectors, representation of sets/maps, etc)
   This exists mostly to be a common place to define the enumeration of
   data types
 */

class wrapper{
public:
  ///enumeration for data types
  enum p_vals {
    D_INDEX=0, 
    D_XPOS, 
    D_YPOS, 
    D_FRAME, 
    D_I, 
    D_R2, 
    D_E,
    D_NEXT,
    D_PREV, 
    D_DX, 
    D_DY, 
    D_ORGX, 
    D_ORGY, 
    D_ZPOS,
    D_UNQID,
    D_TRACKID,
    D_S_ORDER_PARAMETER,
  };

  ///Prints out the currents contents of the wrapper in some
  ///sort of sensible way.
  virtual void print() const= 0;
  
  ///returns the current number of particle entires that the wrapper
  ///has under it's control.  
  virtual int num_entries() const =0;

  /**
     Returns a set of the types of data that the wrapper knows how
     to deal with.  
   */
  virtual  std::set<p_vals> get_data_types()const;

  /**
     Returns a pointer to the map
  */
  std::map<p_vals, int>* get_map_ptr(){
    return &data_types_;
  }

  wrapper(std::map<p_vals,int>map_in);
  wrapper(){};
  virtual ~wrapper(){};

  
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

 
