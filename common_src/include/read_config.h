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
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.

#ifndef READ_XML
#define READ_XML
#include <vector>
#include <string>
#include "enum_utils.h"
#include "md_store.h"
namespace utilities{




/**
   class for reading configuration from xml files.  This is very simple.
   Each instance can only read from one element in the xml file and the
   element must be a child of the root. 
 */
class Read_config{
public:
  /**
     constructor.  The vector of strings is the list of attributes to try to parse.
     
     @param fname the xml file to parse
     @param elm_name the element to look for the parameters in.  
  */
  Read_config(std::string fname, std::string elm_name);
  /**
     destructor
  */
  ~Read_config();
  
  /**
     looks up the value of the given attribute.  The value is
     returned by reference.  The function returns the value.

     @param attr_name attribute name
     
  */
  template <class T>
  T get_value(const std::string & attr_name,T & val)const;
  
  

  /**
    Gets value at index j.   The value is
    returned by reference.  The function returns as well the value.
  */
  template <class T>
  T get_value(int j,T& val)const;
  

  /**
     Returns true if the stanza contains a pram with the given key
   */
  bool contains_key(const std::string& key)const;
  /**
     Returns the index of the given key in the internal structure
   */
  int get_key_index(const std::string& key)const;
  
  
  
  /**
     prints all information stored.
   */
  void print()const;

  /**
     Returns the number of parsed prams
   */
  int size() const
  {
    return md_store_.size();
  }
  
  /**
     Returns the key at j in the internal structure
   */
  std::string get_key(int j) const
  {
    return md_store_.get_key(j);
    
  }
  /**
     Returns the type at j in the internal structure
   */
  utilities::V_TYPE get_type(int j)const
  {
    return md_store_.get_type(j);
  }
  

private:
  Md_store md_store_;
  
  
};



}



#endif


