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



%include <std_common.i>
%include std_except.i 
%module cstatic2
%{
#define DIM_COUNT 2
#define PTYPE 0
#include "part_def.h"
#include "particle_base.h"
%}
namespace tracking
{
  
  class particle
  {
  public:
    void print();
    particle(int i,int f);
    ~particle();
  
  };
}
 
 
  
