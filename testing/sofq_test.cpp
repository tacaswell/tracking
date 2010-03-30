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


#include "accum_sofq.h"



using tracking::Accum_sofq;
using utilities::Tuplef;
using utilities::Tuplei;
using tracking::particle;


int main()
{
  
  Tuplei bins(10);
  Tuplef range(.5);
  
  Accum_sofq test(range,bins);
  particle * dummy = NULL;
  test.add_particle(dummy);
  
  test.display();
  
  

  return -1;

}
