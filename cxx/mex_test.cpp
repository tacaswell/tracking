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
//standard includes
#include<iostream>
#include<fstream>
#include <mcheck.h>

//my includes
#include "wrapper.h"
#include "wrapper_i.h"
#include "wrapper_o.h"

#include "particle.h"
#include "params.h"


#include "hash_box.h"
#include "hash_shelf.h"
#include "track_box.h"
#include "master_box_t.h"
#include "hash_case.h"

#include "wrapper_i_matlab.h"
#include "params_matlab.h"
#include "hash_case3d.h"

#include "wrapper_i_ning.h"
#include "params_ning.h"


#include "mex.h"

#include "matlab_utilities.h"

using namespace tracking;
using utilities::array_to_mat;
using utilities::vector_to_mat;


extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=2 && nrhs!=1){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }

  //nonsense to get the map set up
  map<wrapper::p_vals, int> contents;
//   wrapper::p_vals tmp[] = {wrapper::d_index,wrapper::d_xpos,
// 			   wrapper::d_ypos, wrapper::d_zpos, 
// 			   wrapper::d_frame};
//   int tmp2[] = {0, 1, 2 ,3,4};
  wrapper::p_vals tmp[] = {wrapper::d_xpos,
			   wrapper::d_ypos, 
			   wrapper::d_frame};
  int tmp2[] = {0, 1, 2 };

  vector<wrapper::p_vals > tmp3(tmp, tmp+3);
  vector<wrapper::p_vals>::iterator it1 = tmp3.begin();

  vector<int> tmp4(tmp2, tmp2+3);
  vector<int>::iterator it2 = tmp4.begin();

  map<wrapper::p_vals, int>::iterator it3 = contents.begin();

  for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
    contents.insert(it3,pair<wrapper::p_vals, int>(*it1, *it2));
  //end nonsense
  //there has to be a better way to do this

  


  params_matlab p_in = params_matlab(prhs,contents);
  // params_ning p_in = params_ning(3,100*1000,contents);

  //  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_unqid,4));
  //  contents[wrapper::d_index] = 5;
  //  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
  //  params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),contents.size());
  params_file p_out = params_file(mxGetM(*prhs),contents);
  //  master_box b = master_box(&p,&p,6);

  master_box_t<particle_track>bt(&p_in,&p_out);
  
  vector<int> dims;	
  //  for(int t = 0; t<3;t++)	
  //    dims.push_back(80);    

  dims.push_back(512);
  dims.push_back(1385);

  //  dims.push_back(50);

  cout<<"total number of particles is: "<<bt.size()<<endl;;

  hash_case s(bt,dims,5,1462);

  //  s.print();
  track_shelf tracks;

  //  s.link(5,tracks);


  cout<<endl;
  bt.initialize_out();
  for (int j = 0;j<bt.size();j++){
    //    (bt.get_particle(j))->print();
    (bt.get_particle(j))->set_particle();
  }
  bt.finalize_out();
  
 //  vector<double> bin_c, bin_r; 
//   s.gofr(9, 20, bin_c, bin_r);
//   vector_to_mat(plhs+1,bin_c);

  /*vector<double> b;
    for(int j = 0; j<15;j++)
    b.push_back(j);
  
    vector_to_mat(plhs+1,b);
  */

  // hash_box tmp;
  //
  //
  //


    
  //   //s.print();

  //   //  hash_shelf* f = s.return_shelf(0);


  //   //  f->get_region(5,5,&tmp,1);
  //   //  tmp.print();
  
  //

  //   
  //   tracks.print();


  
  //vec_print2(bin_c);



  //   for(int j = 0; j<15; j++){
  //     (b.get_particle(j))->set_particle();
  //   }

  //   b.finalize_out();
  
  //   track_box tb = track_box(dynamic_cast<particle_track*>(b.get_particle(0)));
  //   tb.print();
  
  //   for(int j = 1; j<15; j++){
  //     tb.push_back(dynamic_cast<particle_track*>(b.get_particle(j)));
  //   }
  
  //  tb.print();

  //  muntrace();
  return;
}
