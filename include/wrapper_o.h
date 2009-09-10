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
#include <string>
#include <vector>
#include <map>

#include "wrapper.h"

 

#ifndef WRAPPER_O_BASE
#define WRAPPER_O_BASE

namespace tracking{
class params;
using std::string;
using std::vector;
using std::map;
using std::cout;
using std::pair;
using std::endl;
using std::ofstream;
;

/**
   Abstract base class for data wrapper_o 's.  In terms of
   functionality derived classes can declare 
   
   @author Thomas Caswell
*/

class wrapper_o_base:  public wrapper{
protected:
  ///Count of the number of particles that have been added
  int part_count;
  
  ///to be removed, move this accounting to derived classes
  ///posistion in squence
  int seq_count;

  ///if a particle is 'opened'
  bool part_open;
  
  ///if the wrapper is 'open' to get additional particles
  bool wrapper_open;
  
  
  
//   wrapper_o_base(){
//     part_count = 0;
//     seq_count = 0;
//     part_open = false;
//     wrapper_open = false;
//   }
  
  wrapper_o_base(std::map<p_vals,int>map_in)
    :wrapper(map_in),part_count(0),
     seq_count(0), part_open(false),
     wrapper_open(false){};
  
public:
  ///Destructor
  virtual ~wrapper_o_base(){};
  

  /**
     @name old API Old wrapper_o API, will be depreciated.  
     (From the one class that uses it, leave me alone, im trying to leave a
     trail for my self to follow later).
  */
  //\@{
  
  //Sets the value of 'type' for particle 'ind' to be 'val'
  //this assume random acess, gives too much control away from this
  //class
  //virtual void set_value(int ind, utilities::D_TYPE type,double val)=0;
  //Prints the entry ind
  //this is for debugging only, will be removed (maybe)
  //virtual void print(int ind)=0;
  ///returns the number of particles stored
  virtual int num_entries() const=0;
  
  ///adds another particle to the data table and fills it with
  ///zero values.  Returns the index of the new particle
  //  virtual int add_particle()=0;
  
  //\@}

  /**
     @name NEW API
     New API for output wrappers, which is a bit more carefully thought about.

     All derived classes must implement these and only these public functions (because
     of the way that pointers to this base class are used in the code, only these
     functions are available in the bulk of the code)

     If the underlying data struture
     has a notion of 'order' then the particles are garunteed to be in that
     structure in the order they were added

     Typical life cycle:
     
     -initialize wrapper
     -loop over particles
     --open particle
     --set values
     --close particles
     -finalize wrapper
  */
  //\@{

  /**
     sets up the wrapper to add new particle to the underlying data
     structure.  Behavior of this function if a particle is alread 'open'
     is currently undefined(either close it and move on, or nuke the old
     data, or just keep appending to it)

     This function can either write straight in to the underlyign data structure
     or work through a temporary buffer
  */
  virtual void start_new_particle()=0;
  
  /**
     sets the data for type to val for the currently activated
     particle in the data structure.  If the wrapper doesn't know
     about the p_val type, then [[throw an exception|ignore]] )(have
     not decided yet)
    
     @param type
     the parameter to be set
     @param val
     the value to set the parameter to
  */
  virtual void set_new_value(utilities::D_TYPE type, double val)=0;
  
  /**
     'closes' a partcile anc garuntees that the data is written to the
     underlying data structure.  Does nothing if a particle isn't
     open.
   */
  virtual void end_new_particle()=0;

  /**
     Preforms any clean up (not of data structures, that is taken care of by destructor)
     that needs to be done such as closing a file
  */
  virtual   void finalize_wrapper()=0;

  /**
     preforms any initialization that needs to be done
     the logic for this is to not needlessly open and close files, or creat
     matlab arrays if they arn't going to be used.  It also allows the wrapper
     object to be reused (ie, do all the processing, output one subset of data to
     one place, and another subset to another place
   */
  virtual void initialize_wrapper()=0;

  /**
     changes where the wrapper is pointing (ie, write out to a
     different array or file or somethin
   */
  virtual void reset_wrapper(params * param)=0;

  
  
  //\@}
  

  
};
}
#endif
  

  
  
