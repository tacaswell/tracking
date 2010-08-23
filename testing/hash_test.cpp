//Copyright 2008,2009 Thomas A Caswell
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


#include <iostream>

#include "hash_shelf.h"
using std::cout;
using std::endl;
using utilities::Tuple;

using std::string;
using std::cerr;

using tracking::Hash_shelf;


int main(int argc, const char * argv[])
{

  try
  {
#if DIM_COUNT == 2
    Hash_shelf test_shelf(Tuple<float,2>(100,100),5,0,false);
#elif DIM_COUNT == 3
    Hash_shelf test_shelf(Tuple<float,3>(100,100,100),10,0,false);
#endif
    test_shelf.test();
    
 
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 

  

  return 0;

}
