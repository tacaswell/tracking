//Copyright 2009 Thomas A Caswell
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
#include <exception>

//my includes
#include "wrapper.h"
#include "wrapper_i.h"
#include "wrapper_o.h"

#include "particle_base.h"
#include "params.h"


#include "hash_box.h"
#include "hash_shelf.h"
#include "track_box.h"
#include "master_box_t.h"
#include "hash_case.h"

#include "wrapper_i_matlab.h"
#include "params_matlab.h"


#include "wrapper_i_ning.h"
#include "params_ning.h"
#include "params_file.h"


#include "mex.h"

#include "matlab_utils.h"


#include "histogram.h"
#include "svector.h"

#include "array.h"

#include "generic_wrapper_matlab.h"
#include "generic_parameters_matlab.h"

#include "cell_matlab.h"

#include "coarse_grain_array.h"
#include "counted_vector.h"

using namespace tracking;
using std::exception;
using std::cerr;

using utilities::array_to_mat;
using utilities::vector_to_mat;
using utilities::Svector;
using utilities::Histogram;
using utilities::Array;
using utilities::Cell_matlab;
using utilities::Generic_wrapper_base;
using utilities::Generic_parameters_matlab;
using utilities::Coarse_grain_array;
using utilities::Counted_vector;



extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=2|| nrhs!=5){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }
  try{

    
    utilities::Pair dims;	
    dims[0] = (int)mxGetScalar(prhs[1]);
    dims[1] = (int)mxGetScalar(prhs[2]);
    int frames = (int)mxGetScalar(prhs[3]);
    int max_r =  (int)mxGetScalar(prhs[4]);;
    
    
    //nonsense to get the map set up
    map<utilities::D_TYPE, int> contents;
    utilities::D_TYPE tmp[] = {wrapper::D_XPOS,
			     wrapper::D_YPOS, 
			     wrapper::D_FRAME};
    int tmp2[] = {0, 1, 2 };
  

    vector<utilities::D_TYPE > tmp3(tmp, tmp+3);
    vector<utilities::D_TYPE>::iterator it1 = tmp3.begin();

    vector<int> tmp4(tmp2, tmp2+3);
    vector<int>::iterator it2 = tmp4.begin();

    map<utilities::D_TYPE, int>::iterator it3 = contents.begin();

    for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
      contents.insert(it3,pair<utilities::D_TYPE, int>(*it1, *it2));
    //end nonsense
    //there has to be a better way to do this
    

    params_matlab p_in = params_matlab(prhs,contents);

    //this is meaningless and not used
    params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),
 					contents.size());

    Master_box<particle>bt(&p_in,&p_out);
    cout<<"total number of particles is: "<<bt.size()<<endl;;
  
//     particle *p;
//     int max_sz = bt.size();
//     int cur_frame = -1;
    
//     for(unsigned int j = 0; j<max_sz; ++j)
//     {
//       p = bt.get_particle(j);

//       int tmp_frame = (int)p->get_value(wrapper::D_FRAME);
//       if(tmp_frame != cur_frame)
//       {
// 	cout<<tmp_frame<<"\t"<<j<<endl;
// 	cur_frame = tmp_frame;
//       }
//     }
    
    
    
    hash_case h_case(bt,dims,max_r,frames);
    cout<<"case built"<<endl;

//     h_case.print();
    
    
    

    // Output NN vectors
    Cell_matlab nn_cell(frames,plhs+1);
    Cell_matlab pos_cell(frames,plhs);
    h_case.nearest_neighbor_array(pos_cell,nn_cell,max_r);
    cout<<"out put"<<endl;
    

    

  }
  catch(const char * err){
    cerr<<err<<endl;
  } 
  catch(exception & e)
  {
    cerr<<e.what()<<endl;
  }
  catch(...){
    cerr<<"uncaught error"<<endl;
  }



  return;
}
