//Copyright 2010,2012 Thomas A Caswell
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
class Attr_list_hdf;

/**
   A class to hold meta data.  
*/
class Md_store
{
public:
    
  /**
     looks up the value of the given attribute.  The value is
     returned by reference.  The function returns the value.

     @param[in] attr_name attribute name
     @param[out] val the value
     @return the value
  */
  template <class T>
  T get_value(const std::string & attr_name,T & val)const;
  
    
  /**
     Sets a given (key,val) pair.

     @param[in] attr_name attribute name
     @param[in] val the value
  */
  template <class T>
  void set_value(const std::string & attr_name,const T & val,bool over_write = true);
  
  
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
     Gets value at index j.   The value is returned
     by reference.  The function either returns the value.
  */
  unsigned int get_value(int j,unsigned int & val)const;
  
  /**
     Gets value at index j.   The value is
     returned by reference.  The function returns as well the value.
  */
  std::string get_value(int j,std::string & val)const;

  /**
     Gets value at index j.   The value is
     returned by reference.  The function returns as well the value.
  */
  bool get_value(int j,bool & val)const;

  
  
  /**
     removes a given key

     @return true if found and removed, false if not found
   */
  bool remove_key(const std::string & key);
  
  /**
     Returns true if the stanza contains a pram with the given key
  */
  bool contains_key(const std::string& key)const;

  
  /**
     Returns true if the stanza contains a pram with the given key.
     If it contains the key indx is the location.

     @param[in] key key to look for
     @param[out] indx location of the key, returns len+1 if key does not exist
     @return if the key exists
  */
  bool contains_key(const std::string& key,unsigned int & indx)const;

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
    return entries_.at(j).type;
  }

  /**
     Returns the value as a (void *)
  */
  void * get_val(int j)const
  {
    return entries_.at(j).value;
  }


  //@{
  /**
     Don't use these, will became private.

     Adds an element with the given (key,val)
  */
  void add_element(const std::string & key,const std::string & type,const  std::string & value);
  void add_element(const char * key,const char * type, const char * value);
  void add_element(const char * key,float val);
  void add_element(const char * key,int val);
  void add_element(const char * key,unsigned int val);
  void add_element(const char * key,bool val);
  void add_element(const char * key,const char *  val);
  void add_element(const char * key,const std::string &val);
  //@}
  /**
     Add all elements in md_in to this Md_store object
   */
  void add_elements(const Md_store * md_in);
  

  /**
     Print out the contents
   */
  void print()const;

  /**
     Destructor
   */
  ~Md_store();
  
  /**
     Default constructor
   */
  Md_store();
  
  /**
     Constructor from an attr_list
   */
  Md_store(const utilities::Attr_list_hdf & );
  
private:
  /**
     A struct to store meta data information, (key,type,val) as strings to
     make passing this stuff around easy.
  */
  struct Md_element
  {
    /**
       Constructor from strings
     */
    Md_element(std::string key_i,utilities::V_TYPE type_i,void * value_i)
      :key(key_i),type(type_i),value(value_i){}
    /**
       Constructor from const char *
     */
    Md_element(const char * key_i,utilities::V_TYPE type_i,void * value_i)
      :key(key_i),type(type_i),value(value_i){}

    /**
       Copy constructor
     */
    Md_element(const Md_element &);
    /**
       Assignment operator
    */
    Md_element & operator =(const Md_element &);
    /**
       Empty constructor
     */
    Md_element();
    /**
       Destructor, if more supported dyp
     */
    ~Md_element();
    
    /**
       The meta data key
     */
    std::string key;
    /**
       The type of the meta data
     */
    utilities::V_TYPE type;
    /**
       pointer to the value.  The struct owns the pointer and will clean it up
     */
    void * value;
  

    };
    
    

  /**
     The
   */
  std::vector<Md_element> entries_;
  };
  
    
}


  



#endif
