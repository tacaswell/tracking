//Copyright 2012 Thomas A Caswell
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



#include <iostream>
#include <set>

#include "wrapper_i_jb_binary.h"



using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::string;


using utilities::Wrapper_i_jb_binary;
using utilities::D_TYPE;
using utilities::Tuplef;



int main(int argc, const char * argv[])
{

  cout<<"hello world"<<endl;
  string fname = "/home/tcaswell/work/justin_data/large_packings/3000000_particles_phi_eq_0.7/xyz_positions.dat";
  
  Wrapper_i_jb_binary wrap = Wrapper_i_jb_binary();
  
  wrap.set_file_name(fname);
  
  wrap.proc_file(3);
  
}

