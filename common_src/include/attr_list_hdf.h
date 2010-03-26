//Copyright 2010 Thomas A Caswell
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

#ifndef HDF_ATTTR_LIST
#define HDF_ATTTR_LIST

#include <list>
#include <string>
#include "tuple.h"
// forward declare classes
namespace H5
{
class H5Object;
}

namespace utilities
{

/**
   A class that takes in a hdf object, makes a list of the attributes.
   The object can then be queried as to what it contains and handles getting
   meta-data in and out of the hdf_object.  This should be broken out the
   way that that wrappers are for handling multiple subclasses under the same interface,
   but that isn't needed yet at this point as the only type of output under active work
   is hdf.
 */
class Attr_list_hdf
{
public:
  /**
     Constructor, gets 
   */
  Attr_list_hdf(H5::H5Object * obj);
  /**
     Destructor			
   */
  ~Attr_list_hdf();

  /**
     If the attribute list contains the key.  
   */
  bool contains_attr(const std::string & key) const;
  
  /**
     Returns the value associated with the key cast as an integer.  Still
     need to sort out what to do if this is an inappropriate cast.  This assumes
     that you have checked to make sure that the attribute exists, or it
     will throw the hdf errors out
   */
  int get_value(const std::string & key, int & value_out) const ;
  /**
     Sets the value associated with the key cast as an integer.  Still
     need to sort out what to do if this is an inappropriate cast.
  */
  void set_value(const std::string & key,  const int &   value_in,bool over_write = false) ;

  float get_value(const std::string & key, float & value_out) const ;
  void set_value(const std::string & key,  const float & value_in,bool over_write = false) ;
  
  utilities::Tuple<float,2> get_value(const std::string & key, utilities::Tuple<float,2> & value_out) const ;
  void set_value(const std::string & key,  const utilities::Tuple<float,2> & value_in,bool over_write = false) ;

  utilities::Tuple<float,3> get_value(const std::string & key, utilities::Tuple<float,3> & value_out) const ;
  void set_value(const std::string & key,  const utilities::Tuple<float,3> & value_in,bool over_write = false) ;
  
  std::string get_value(const std::string & key,std::string & value_out)const;
  void set_value(const std::string & key,const std::string & value_out,bool over_write = false);

    
  unsigned int get_value(const std::string & key,unsigned int& value_out)const;



  /**
     Removes the key from the H5Object.
   */
  void remove_attr(const std::string & key);


  
  void print()const;
  

  
private:
  std::list<std::string> keys_;
  H5::H5Object * obj_;
  
  

};
}

  

#endif
