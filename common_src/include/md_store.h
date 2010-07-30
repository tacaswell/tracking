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
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

#ifndef MD_STORE
#define MD_STORE

#include <string>
#include <vector>
#include "enum_utils.h"
namespace utilities
{

/**
   A class to hold Md_elements and functions to get information out
   in a useful way. 
*/
class Md_store
{
public:
    
  /**
     looks up the value of the given attribute.  The value is
     returned by reference.  The function returns the value.

     @param attr_name attribute name
     
  */
  template <class T>
  T get_value(const std::string & attr_name,T & val)const;
  

  /**
     Returns value at index j.  The value is
     returned by reference.  The function returns the value.

     
  */
  float get_value(int j,float & val)const;
  /**
     Gets value at index j.   The value is returned
     by reference.  The function either returns the value.

     
  */
  int get_value(int j,int & val)const;
  /**
     Gets value at index j.   The value is
     returned by reference.  The function returns as well the value.

     
  */
  std::string get_value(int j,std::string & val)const;
  
  
  
  

  /**
     Returns true if the stanza contains a pram with the given key
  */
  bool contains_key(const std::string& key)const;
  /**
     Returns the index of the given key in the internal structure
  */
  int get_key_index(const std::string& key)const;
  
  /**
     Returns the number of parsed prams
  */
  unsigned int size() const
  {
    return entries_.size();
  }
  
  /**
     Returns the key at j in the internal structure
  */
  std::string get_key(int j) const
  {
    return entries_.at(j).key;
  }
  /**
     Returns the type at j in the internal structure
  */
  utilities::V_TYPE get_type(int j)const
  {
    return str2VT_s(entries_.at(j).type);
  }
  /**
     Returns the value as a string
  */
  std::string get_val(int j)const
  {
    return std::string(entries_.at(j).value);
  }


  /**
     Add entry
  */
  void add_element(std::string & key,std::string & type, std::string & value);
  void add_element(const char * key,const char * type, const char * value);
  void add_element(const char * key,float val);
  void add_element(const char * key,int val);
  

  /**
     Add all elements in md_in to this Md_store object
   */
  void add_elements(const Md_store * md_in);
  

  /**
     Print out the contents
   */
  void print()const;
  
  ~Md_store();
  
private:
  /**
     A struct to store meta data information, (key,type,val) as strings to
     make passing this stuff around easy.
  */
  struct Md_element
  {
  
    Md_element(std::string key_i,std::string type_i,std::string value_i)
      :key(key_i),type(type_i),value(value_i){}
    Md_element(const char * key_i,const char * type_i,const char * value_i)
      :key(key_i),type(type_i),value(value_i){}
    Md_element();
    ~Md_element(){};
    std::string key;
    std::string type;
    std::string value;
  

  };

  std::vector<Md_element> entries_;
};

  

}

#endif
