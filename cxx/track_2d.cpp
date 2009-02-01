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
/*
  This function is meant to be a stand alone (almost) drop in replacement for track.m for 2-D samples
  The array can 

  incantation:
  data_out[x,y,t,track_id] = track_2D(data_in[x y t], max_disp, max_x, max_y, frames);
*/

//standard includes
#include<iostream>
#include<fstream>
#include<vector>
#include<map>

//my includes

#include "master_box_t.h"
#include "track_shelf.h"


#include "params_matlab.h"
#include "hash_case3d.h"


#include "mex.h"

#include "matlab_utilities.h"

using namespace tracking;
using utilities::array_to_mat;
using utilities::vector_to_mat;


extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=1 || nrhs!=5){
    cout<<"Error, wrong number of arguments"<<endl;
    cout<<"Provide one output array, one input 3xN array and "<<endl;
    //    return;
  }

  
  vector<int> dims;	
  dims.push_back((int)mxGetScalar(prhs[2]));    
  dims.push_back((int)mxGetScalar(prhs[3]));    
  
  track_shelf tracks;


  //nonsense to get the map set up
  map<wrapper::p_vals, int> contents;
  wrapper::p_vals tmp[] = {wrapper::d_xpos,
			   wrapper::d_ypos,
			   wrapper::d_frame};
  int tmp2[] = {0, 1, 2};

  vector<wrapper::p_vals > tmp3(tmp, tmp+3);
  vector<wrapper::p_vals>::iterator it1 = tmp3.begin();

  vector<int> tmp4(tmp2, tmp2+3);
  vector<int>::iterator it2 = tmp4.begin();

  map<wrapper::p_vals, int>::iterator it3 = contents.begin();

  for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
    contents.insert(it3,pair<wrapper::p_vals, int>(*it1, *it2));
  //end nonsense
  //there has to be a better way to do this

  double max_disp = mxGetScalar(prhs[1]);
  if(max_disp<0)
    max_disp = 1;

  

  //set up input and output wrapers
  params_matlab p_in = params_matlab(prhs,contents);

   contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
  
  params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),contents.size());
  
  //load data from matlab
  master_box_t<particle_track>bt(&p_in,&p_out);
  //make hash case
  hash_case s(bt,dims,25,(int)mxGetScalar(prhs[4]));
  //link tracks
  //  cout<<max_disp<<endl;
  s.link(max_disp,tracks);
  // s.print();

  //output to matlab
  bt.initialize_out();
  tracks.set_shelf();
  bt.finalize_out();
  
  return;
}
