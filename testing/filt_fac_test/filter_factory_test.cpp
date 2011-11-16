//Copyright 2011 Thomas A Caswell
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


#include "read_config.h"
#include "filter.h"
#include <vector>
#include <string>
#include <iostream>
using std::string;
using std::vector;
using utilities::Read_config;
using std::cout;
using std::endl;

using utilities::filter_factory;
using utilities::Filter;

int main ()
{ 
  Filter * filt;
  
  Read_config rc1("test.xml","test1");
  
  filt  = filter_factory(*(rc1.get_store()));
  cout<<filt->get_type()<<endl;
  delete filt;
    
  Read_config rc2("test.xml","test2");
  filt  = filter_factory(*(rc2.get_store()));
  cout<<filt->get_type()<<endl;
  delete filt;
  return 0;

}
