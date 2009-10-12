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
  This file is a specific static application of the tracking tool
  chain.  This file is for dealing with data sets at a fixed volume
  fraction and returns the mean g(r)
 */


//standard includes
#include<iostream>
#include<fstream>
#include <exception>

//my includes


#include "track_shelf.h"
#include "master_box_t.h"
#include "hash_case.h"

#include "wrapper_i_matlab.h"
#include "params_matlab.h"

#include "mex.h"
#include "matlab_utils.h"
#include "array.h"

#include "histogram.h"
#include "svector.h"

#include "generic_wrapper_matlab.h"
#include "generic_parameters_matlab.h"

#include "params_ning_hd.h"

#include "cell_matlab.h"

using namespace tracking;
using std::exception;

using utilities::array_to_mat;
using utilities::vector_to_mat;
using utilities::Svector;
using utilities::Histogram;
using utilities::Array;
using utilities::Cell_matlab;
using utilities::Generic_wrapper_base;
using utilities::Generic_parameters_matlab;
extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=2 || nrhs!=6){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }
  try{

    /**
       \todo add checks on input types
    */
    utilities::Tuple dims;	
    dims[0] = (int)mxGetScalar(prhs[1]);
    dims[1] = (int)mxGetScalar(prhs[2]);
    int frames = (int)mxGetScalar(prhs[3]);

    double max_r = mxGetScalar(prhs[4]);
    int bins = (int)mxGetScalar(prhs[5]);



    //nonsense to get the map set up
    map<utilities::D_TYPE, int> contents;

    utilities::D_TYPE tmp[] = {wrapper::D_XPOS,
			     wrapper::D_YPOS, 
			     wrapper::D_FRAME};
    int tmp2[] = {0, 1, 2};
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
 


    contents.insert(pair<utilities::D_TYPE, int>(wrapper::D_TRACKID,5));


    master_box_t<particle_track>bt(&p_in);
    
    track_shelf tracks;
    cout<<"total number of particles is: "<<bt.size()<<endl;

//     for(int k = 0;k<15;++k)
//     {
//       (bt.get_particle(k))->print();
//       cout<<      (bt.get_particle(k))->get_value(wrapper::D_FRAME)<<endl;
//     }
    
     
  
    //build hash case
    hash_case hcase(bt,dims,(int)max_r,frames);
    cout<<"case built"<<endl;

//     hcase.print();
    

    // Compute G(r)
    vector<double> gofr_bin_count;
    vector<double> gofr_bin_edges;
    cout<<"starting gofr"<<endl;
    hcase.gofr_norm(max_r,bins,gofr_bin_count,gofr_bin_edges);
    vector_to_mat(plhs,gofr_bin_count);
    vector_to_mat(plhs +1,gofr_bin_edges);
    cout<<"gofr computed"<<endl;

    
    
    
  }
  catch(const char * err){
    cout<<err<<endl;
  } 
  catch(exception & e)
    {
      cout<<e.what()<<endl;
    }
  catch(...){
    cout<<"uncaught error"<<endl;
  }

  return;
}


