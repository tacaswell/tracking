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

#include "data_map.h"
using namespace utilities;
using std::map;

Data_map:: Data_map(const std::map<utilities::D_TYPE,int>& in)
{
  int count = 0;
  
  for(std::map<utilities::D_TYPE,int>::const_iterator it = in.begin();
      it!= in.end(); ++it)
  {
    if(!((*it).first<D_TYPE_COUNT))
    {
      throw "constants wrong, type out of array bounds";
    }
    data_layout_[(*it).first] = (*it).second;
    ++count;
    if(!(count<D_TYPE_COUNT))
    {
      throw "out of array spaces";
    }
  }
}




  
