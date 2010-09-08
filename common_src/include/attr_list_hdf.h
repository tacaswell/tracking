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
#include "enum_utils.h"
// forward declare classes
namespace H5
{
class H5Object;
}

namespace utilities
{


/**
   Wraps the attribute functionality of hdf objects.  Eventually this
   should be split into a base class and the hdf class so that other
   input/output formats can be handled under the same interface.  It
   possibly should be merged with md_store objects as well.

   If the underlying object is changed else where, the list of keys
   will not be updated.
 */
class Attr_list_hdf
{
public:
  /**
     Constructor
   */
  Attr_list_hdf(H5::H5Object * obj);
  /**
     Destructor			
   */
  ~Attr_list_hdf();

  /**
     Returns true If the attribute list contains the key.  
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
  /**
     Gets float type values
   */
  float get_value(const std::string & key, float & value_out) const ;
  /**
     Sets float type values
   */
  void set_value(const std::string & key,  const float & value_in,bool over_write = false) ;
  
  /**
     Gets tuple values, templated on tuple length 
   */
  template<int N>
  utilities::Tuple<float,N> get_value(const std::string & key, utilities::Tuple<float,N> & value_out) const ;
  /**
     Sets tuple values, templated on tuple length 
   */
  template<int N>
  void set_value(const std::string & key,  const utilities::Tuple<float,N> & value_in,bool over_write = false) ;
  /**
    Gets string values
   */
  std::string get_value(const std::string & key,std::string & value_out)const;
  /**
     Sets string values
   */
  void set_value(const std::string & key,const std::string & value_out,bool over_write = false);
  
  /**
     Gets bool value.
   */
  bool get_value(const std::string & key,bool & value_out)const;
  /**
     Sets bool value
   */
  void set_value(const std::string & key,const bool & value_out,bool over_write = false);

  
  /**
     Gets an unsigned int value
   */  
  unsigned int get_value(const std::string & key,unsigned int& value_out)const;
  /**
     Sets an unsigned int value
   */  
  void set_value(const std::string & key,const unsigned int & value_out,bool over_write = false);

  
  
  

  /**
     Removes the key from the H5Object.
   */
  void remove_attr(const std::string & key);

  
  /**
     Prints the contents to stdout
   */
  void print()const;
  
  /**
     Returns a list of the keys contained in this wrapper
   */
  std::list<std::string> contents()const;
  /**
     Returns the type of scalar attributes.  Only works
     for int,float, and string.  All others return V_ERROR.

     \todo make to work for all (or most) types
   */
  utilities::V_TYPE get_type(const std::string& key)const;
  
  
private:
  /**
     List of the keys of the attributes in the object
   */
  std::list<std::string> keys_;
  /**
     Pointer to the hdf object
   */
  H5::H5Object * obj_;
  
  

};
}

  

#endif
