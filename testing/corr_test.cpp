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
//see website for additional permissions under GNU GPL version 3 section 7



#include <iostream>

#include "hash_shelf.h"
#include "corr.h"
using std::cout;
using std::endl;
using utilities::Tuple;

using std::string;
using std::cerr;

using tracking::Hash_shelf;
using tracking::Dummy_corr;


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
 
    Dummy_corr dc(10);
    
    test_shelf.compute_corr(dc);
    
 
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 

  

  return 0;

}
