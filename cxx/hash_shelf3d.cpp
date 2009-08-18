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
#include "hash_shelf3d.h"
using namespace tracking;

void hash_shelf3d::print(){
  for(int k = 0;k<hash_dims[2];k++){
    cout<<"Plane "<<k<<endl; 
    for(int j=0; j<hash_dims[0];j++){
      for(int i=0; i<hash_dims[1];i++){
	cout<<(hash.at( k*hash_dims[1]*hash_dims[1]+
			j*hash_dims[0] + i)).get_size()
	    <<"\t";
      }
      cout<<endl;
    }
  }
}


void hash_shelf3d::get_region( particle_base* p,
		   hash_box* box, int range){
  get_region((int)(p->get_value(wrapper::D_XPOS)/ppb),
	     (int)(p->get_value(wrapper::D_YPOS)/ppb),
	     (int)(p->get_value(wrapper::d_zpos)/ppb),
	     box,range);

}

void hash_shelf3d::get_region( int n,  int m, int s,
			       hash_box* box, int range){
  

  if(n<0||m<0||range<0||box==NULL)
    return;
  
  int i_bot = (((n-range)>=0)?(n-range):0);
  int i_top = ((n+range)<(int)hash_dims[0]?(n+range+1):hash_dims[0]);
  int j_bot = ((m-range)>0?(m-range):0);
  int j_top = ((m+range)<(int)hash_dims[1]?(m+range+1):hash_dims[1]);
  int k_bot = ((s-range)>0?(s-range):0);
  int k_top = ((s+range)<(int)hash_dims[2]?(s+range+1):hash_dims[2]);
  

  
  for( int i = i_bot; i<i_top;i++){ //x
    for( int j = j_bot; j<j_top;j++){//y
      for( int k = k_bot; k<k_top;k++){//z
	box->append(&(hash.at(i + hash_dims[0]*j + hash_dims[1]*hash_dims[2]*k)));
      }
    }
  }
}
