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

#ifndef NEW_WRAPPER_
#define NEW_WRAPPER_

#include <complex>
#include <set>
namespace utilities
{
class params;

/**
   Enumeration for wrapper types
 */
enum W_TYPE{                                                                
  W_MATLAB = 0,                                                           
  W_TEXT,                                                                 
  W_NING,                                                                 
  W_HDF,                                                                  
};                                                                        

/**
   Enumeration for data types   
*/
enum D_TYPE {
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
const int D_TYPE_COUNT = 17;

// /**
//    Helper functions to convert D_TYPEs in to short name strings
//  */
// std::string DT2str_s(D_TYPE in)
// {
//   std::string out;
  
//   switch(in)
//   {
//   case D_INDEX: 
//     out = std::string("index");
//     return out;
//     break;
//   case D_XPOS: 
//     out = std::string("x") ; 
//     return out; 
//   case D_YPOS: 
//     out = std::string("y") ; 
//     return out; 
//   case D_FRAME: 
//     out = std::string("frame") ; 
//     return out; 
//   case D_I: 
//     out = std::string("intensity") ; 
//     return out; 
//   case D_R2: 
//     out = std::string("R2") ; 
//     return out; 
//   case D_E: 
//     out = std::string("eccentricty") ; 
//     return out;
//   case D_NEXT: 
//     out = std::string("next_part") ; 
//     return out;
//   case D_PREV: 
//     out = std::string("prev_part") ; 
//     return out; 
//   case D_DX: 
//     throw "x_shift";
//     break; 
//   case D_DY: 
//     throw "y_shift";
//     break; 
//   case D_ORGX: 
//     throw "not implemented";
//     break; 
//   case D_ORGY: 
//     throw "not implemented";
//     break; 
//   case D_ZPOS: 
//     out = std::string("z") ; 
//     return out;
//   case D_UNQID: 
//     out = std::string("unique_id") ; 
//     return out;
//   case D_TRACKID: 
//     out = std::string("track_id") ; 
//     return out;
//   case D_S_ORDER_PARAMETER: 
//     out = std::string("scaler_order_parameter") ; 
//     return out;
//   default:
//     throw("unknown type");
//     break;
    
//   }
  

   
// }



// /**
//    Helper functions to convert short name strings in to D_TYPEs 
//  */
// D_TYPE DT2str_s(std::string in)
// {
//   throw "function not written yet";
//   return D_TRACKID;
// }

// class params;



/**
   ABC for input wrappers
*/
class Wrapper_in
{
public:
  /**
     Returns the value of the type specified from the particle selected.  use the other version
   */
  virtual float get_value(int ind,D_TYPE type, int frame = -1) const = 0;
  /**
     Returns the value of the type specified from the particle selected
   */
  virtual void get_value(std::complex<float>& out,int ind,D_TYPE type, int frame) const = 0;
  /**
     Returns the value of the type specified from the particle selected
   */
  virtual void get_value(float& out,int ind,D_TYPE type, int frame) const = 0;

  /**
     Returns a set of the data types.  Use the other version
   */
  virtual std::set<D_TYPE> get_data_types() const = 0;

  /**
     Returns a set of the data types.  Use the other version
   */
  virtual void  get_data_types(std::set<D_TYPE> & out) const = 0;
  
  /**
     Returns the number of particles in the wrapper
   */
  virtual int get_num_entries() const = 0;
  
  
};

/**
   ABC for output wrappers
*/
class Wrapper_out
{
public:
  /**
     @name Output interface
     New API for output wrappers, which is a bit more carefully thought about.

     All derived classes must implement these and only these public functions (because
     of the way that pointers to this base class are used in the code, only these
     functions are available in the bulk of the code)

     

     Typical life cycle:
     
     -initialize wrapper
     -loop over planes (if wrapper knows about planes)
     --loop over particles
     ---open particle
     ---set values
     ---close particle
     --close particles
     -close planes
     -finalize wrapper
  */
  //\@{

  
  /**
     preforms any initialization that needs to be done
     the logic for this is to not needlessly open and close files, or creat
     matlab arrays if they arn't going to be used.  It also allows the wrapper
     object to be reused (ie, do all the processing, output one subset of data to
     one place, and another subset to another place
   */
  virtual void initialize_wrapper()=0;


  /**
     Opens a plane to put particles into
   */
  virtual void open_plane(int p) = 0;
  

  /**
     sets up the wrapper to add new particle to the underlying data
     structure.  Behavior of this function if a particle is already 'open'
     is currently undefined(either close it and move on, or nuke the old
     data, or just keep appending to it)

     This function can either write straight in to the underlyign data structure
     or work through a temporary buffer
  */
  virtual void new_particle()=0;
  
  /**
     Opens an already existing particle entry in the wrapper
     
     Throws an error of some sort if the particle doesn't exist.

     Still must be closed with an end particle
   */
  virtual void edit_particle(int id) = 0;
  
  /**
     sets the data for type to val for the currently activated
     particle in the data structure.  If the wrapper doesn't know
     about the D_TYPE type, then [[throw an exception|ignore]] )(have
     not decided yet)
    
     @param type
     the parameter to be set
     @param val
     the value to set the parameter to
  */
  virtual void set_value(D_TYPE type, float val)=0;
  /**
     sets data for complex numbers, see set_value

     @param type
     the parameter to be set
     @param val
     the value to set the parameter to
   */
  virtual void set_value(D_TYPE type, std::complex<float> val)=0;
  
  /**
     'closes' a particle. 
     Throws an error if there is no particle open
   */
  virtual void end_particle()=0;
  /**
     Closes the currently open plane.
     Throws an error if there is no open plane.
  */
  virtual void close_plane()=0;
  
  /**
     Performs any clean up.
     that needs to be done such as closing a file
  */
  virtual   void finalize_wrapper()=0;
  
  
  /**
     changes where the wrapper is pointing (ie, write out to a
     different array or file or somethin
   */
  virtual void reset_wrapper(params * param)=0;

  //\@}
  
  /**
     virtual destructor
   */
  virtual ~Wrapper_out(){};
  /**
     empty constructor 
   */
  Wrapper_out(){};
  
  
  

};


}
#endif
