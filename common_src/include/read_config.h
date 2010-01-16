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
namespace utilities{

/**
   Class for reading configuration from xml files.  This is very simple.
   Each instance can only read from one element in the xml file and the
   element must be a child of the root. 
 */
class Read_config{
public:
  /**
     constructor.  The vector of strings is the list of attributes to try to parse.
     
     @param attr_names the attributes to look for
     @param elm_name the element to look for the parameters in.  
  */
  Read_config(std::string fname, std::vector<std::string> attr_names,std::string elm_name);
  /**
     destructor
  */
  ~Read_config();

  /**
     looks up the value of the given attribute.  Returns true if the
     attribute is found and false if not found.  The value is returned
     by reference in val.  The attribute must have been given in the constructor
     to be found.

     @param attr_name attribute name
  */
  bool get_val(std::string attr_name,float & val)const;
  bool get_val(std::string attr_name,int & val)const;
  bool get_val(std::string attr_name,std::string & val)const;
  
  /**
     prints all information stored.
   */
  void print()const;
  

private:
  /**
     vector of the attributes to be parsed
  */
  std::vector<std::string> attr_names_;
  /**
     vector of parsed values
  */
  std::vector<std::string> attr_values_;

  /**
     vector of if the attribute was found
   */
  
  std::vector<bool> attr_found_;
  
};



}



#endif


