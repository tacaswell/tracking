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

#include<iostream>

//my includes




#include "mex.h"

#include "params_matlab.h"
#include "master_box_t.h"
#include "particle.h"
#include "hash_shelf.h"

using namespace tracking;


extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=2 && nrhs!=1){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }

  // set up the input wrapper 
  //nonsense to get the map set up
  map<wrapper::p_vals, int> contents;
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


  // set up dimensions
  vector<int> dims;	
  for(int j =1;j<3;++j){
    if (!mxIsDouble(prhs[j]) ||  
	mxGetN(prhs[j])*mxGetM(prhs[j]) != 1  || mxIsComplex(prhs[j])) {
      mexErrMsgTxt("Input argument must be a real scalar double");
    }
    dims.push_back((int) mxGetScalar(prhs[j]));
  }

  if (!mxIsDouble(prhs[3]) ||  
	mxGetN(prhs[3])*mxGetM(prhs[3]) != 1  || mxIsComplex(prhs[3])) {
    mexErrMsgTxt("Input argument must be a real scalar double");
  }
  
  
  double max_range =mxGetScalar(prhs[3]);

  // make master 
  master_box_t<particle_base>bt(&p_in,&p_in);

  hash_shelf table(dims, 1+((int) max_range), 0);

  for(int j = 0; j< (int)bt.size () ; ++j)
    {
      table.push(bt.get_particle(j));
    }
  
  
	
  

}
