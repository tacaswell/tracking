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
#include "hash_case.h"
#include "hash_shelf3d.h"

#ifndef HASH_CASE3D
#define HASH_CASE3D
namespace tracking{
  
/**
   derived class from hash_case to implement a third dimension
 */
class hash_case3d:public hash_case{
public:
  template<class particle>
  hash_case3d(master_box_t<particle> & mb,vector<int> img_dims, 
	      unsigned int ppb, int frames);
  
};


template<class particle>
hash_case3d::hash_case3d(master_box_t<particle> & mb,vector<int> img_dims, 
		     unsigned int ppb, int frames){
  h_case.resize(frames);
  for(unsigned int j = 0; j<h_case.size(); j++){
    h_case.at(j) = new hash_shelf3d(img_dims, ppb,j);
  }
  // cout<<
  particle *p;
  for(unsigned int j = 0; j<mb.size(); j++){
    p = mb.get_particle(j);
    (h_case.at((int)(p->get_value(wrapper::D_FRAME))))->push(p);
  }
}

}
#endif
