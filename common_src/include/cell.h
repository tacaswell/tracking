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

#ifndef CELL
#define CELL

namespace utilities{
class Array;
/**
   A base class for collecting arrays in.  Largely inspired by the matlab cell objects. Only used
   with matlab code.  This should be removed and the matlab cell functionality be wrapped under
   Generic_wrapper.
*/

class Cell {
public:
  /**
     Add an Array to the cell array
   */
  virtual void add_array(const Array & in )=0 ;

  /**
     Get the length of the cell array
   */
  int get_length() const{
    return length_;
  }
  /**
     Constructor
     @param l the length, that only seems to be set an a private value
   */
  Cell(int l);
  /**
     Destructor
   */
  virtual ~Cell(){};
  
		  
protected:
  /**
     Length of the cell array
   */
  int length_;

  
};

}
#endif 


