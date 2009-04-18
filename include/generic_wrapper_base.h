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

#ifndef GWRAPPER_BASE
#define GWRAPPER_BASE

namespace utilities{
/**
   Base class for the generic wrapper family of functions.  These
   exist because the other wrapper functions are very specific for
   reading in and writing out particles.  These are intended to by used
   for getting arbitrary data out.

   The basic idea is to write to the under ling data structure in
   atomic rows, ie ask for a new row, fill it with what ever, store to
   the underling data structure.

   This is an abstract base class
*/

class Generic_wrapper_base{
public:


  /**
     @name row operations
   */
  //\@{

  /**
     Opens a new row to add data to
   */
  virtual void start_new_row() = 0;
  /**
     Inputs data to be added to the wrapper.

     This may write directly in to the underlying data
     structure, or it may buffer the data until a finalize call
   */
  virtual void append_to_row(double data_in) = 0;
  /**
     finishes the row and sets it to the underlying strucutre
   */
  virtual void finish_row() = 0;
  //\@}


  /**
     @name wrapper operations
   */
  //\@{
  
  /**
     Tells the wrapper to setup to accept rows
   */
  virtual void initialize_wrapper () = 0;
  /**
     cleans up the wrapper
   */
  virtual void finalize_wrapper () = 0;

  //\@}
  
  /**
     Constructor
   */
  Generic_wrapper_base(bool a,bool b):
    wrapper_open_(a),row_open_(b){}
  /**
     Destructor 
   */
  virtual ~Generic_wrapper_base(){};
protected:
  bool wrapper_open_;
  bool row_open_;
};
}
#endif
