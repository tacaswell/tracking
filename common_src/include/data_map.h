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

#ifndef DATA_MAP
#define DATA_MAP

#include <map>

#include "enum_utils.h"

namespace utilities
{
/**
   class to handle translation from D_TYPE to column
   position for flat data structures.  When the particle
   data is stored in a Nxm array, with N particles and m
   D_TYPES, this class provides a map between the column number
   and what data is in the column.  
 */
class Data_map
{
public:
  /**
     Constructor.
   */
  Data_map(const std::map<utilities::D_TYPE,int>& in);

  /**
     Default constructor
   */
  Data_map();
  /**
     Destructor
   */
  ~Data_map(){}
  /**
     looks up the position of the D_TYPE in
   */
  int operator()(utilities::D_TYPE in)const
  {
    return data_layout_[in];
  }
    
  /**
     Returns the value data_layout at index in
   */
  int operator()(int in)const
  {
    // if(in>=D_SENTRY)
    //   throw runtime_error("index out of range");
    /// @todo uncomment this 

    return data_layout_[in];
  }
  /**
     Set the look up column of the D_TYPE dt to loc
   */
  void set_lookup(D_TYPE dt,int loc);
  
  
private:
  /**
     An array that serves as a look up table.  This takes advantage of the fact that
     enums resolve to integers, each entry is the location of the D_TYPE that resolves
     to that elements index.  D_TYPES that aren't include are set to -1
   */
  int data_layout_[D_SENTRY];
};


  
/**
   prints out the data_map to cout
   lifted almost whole cloth from c++/4.3/complex
*/
template<typename _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os, const Data_map& in)
{

    
  std::basic_ostringstream<_CharT, _Traits> s;
  s.flags(__os.flags());
  s.imbue(__os.getloc());
  s.precision(__os.precision());

    
  s << '(' ;
  s<< in(0)  ;
  for(int j = 1;j<D_SENTRY;++j)
    s<< ','<<in(j)  ;
  s << ')';

  return __os << s.str();
}
}




#endif
