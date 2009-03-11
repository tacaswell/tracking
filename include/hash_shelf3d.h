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
#include "hash_shelf.h"

#ifndef HASH_SHELF_3D
#define HASH_SHELF_3D
namespace tracking{
  /**
     Derived class from hash_shelf to deal with three dimensions
   */
class hash_shelf3d :public hash_shelf{
public:
  unsigned int hash_function(particle_base* p){
    return 
      (((int)p->get_value(wrapper::d_zpos))/ppb)*hash_dims[0]*hash_dims[1]+
      (((int)p->get_value(wrapper::d_ypos))/ppb)*hash_dims[1]+
      (((int)p->get_value(wrapper::d_xpos))/ppb);
  }

  hash_shelf3d(vector<int> imgsz, unsigned int ippb, int i_frame):
    hash_shelf(imgsz, ippb, i_frame){};
  void print();
  void get_region( particle_base* p,
		   hash_box* box, int range=1);
  void get_region( int n,  int m, int s,
		  hash_box* box, int range=1);

  ~hash_shelf3d(){}
protected:


private:
};
}
  


#endif
