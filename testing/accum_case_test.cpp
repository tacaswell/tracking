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



#include <iostream>
#include <string>

#include "accum_case.h"
#include "accum_chi4_self.h"

using std::cout;
using std::endl;
using std::string;
using std::cerr;


using utilities::Accum_case;
using tracking::Accum_chi4_self;

float w(float x, float l)
{
  if(x<l)
  
    
    return 1;
  else
    return 0;
    
}


int main(int argc, const char * argv[])
{
  Accum_chi4_self chi = Accum_chi4_self(5,0,5,5,*w);
  Accum_case ac = Accum_case(chi,15);
  
  cout<<"hello world"<<endl;
}
