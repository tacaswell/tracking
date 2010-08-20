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
#include <iostream>
#include <stdexcept>
using std::logic_error;

using namespace utilities;
using std::map;
using std::cout;
using std::endl;
Data_map:: Data_map(const std::map<utilities::D_TYPE,int>& in)
{
  for(int j = 0;j<D_SENTRY;++j)
    data_layout_[j] = -1;
  
  int count = 0;
  
  for(std::map<utilities::D_TYPE,int>::const_iterator it = in.begin();
      it!= in.end(); ++it)
  {
    if(!((*it).first<D_SENTRY))
    {
      throw logic_error("constants wrong, type out of array bounds");
    }
    data_layout_[(*it).first] = (*it).second;
    ++count;
    if(!(count<D_SENTRY))
    {
      throw logic_error("out of array spaces");
    }
  }
}



Data_map:: Data_map()
{
  for(int j = 0;j<D_SENTRY;++j)
    data_layout_[j] = -1;
}


void Data_map::set_lookup(D_TYPE dt ,int loc)
{
  //  cout<<dt<<'\t'<<loc<<endl;
  data_layout_[dt] = loc;
  //  cout<<dt<<':'<<data_layout_[dt]<<endl;
  
}



  
