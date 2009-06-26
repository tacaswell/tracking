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
  fraction and returns the mean g(r), the corrected msd, and the
  corrected tracks for the data set.
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

  
//     utilities::Tuple dims;	
//     dims[0] = (520);
//     dims[1] = (1390);
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

    

    params_matlab p_in = params_matlab(prhs,contents);
    //    params_ning_hd p_in = params_ning_hd(20464,contents);


    contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,5));
    params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),
					 contents.size());


    master_box_t<particle_track>bt(&p_in,&p_out);

    // dims[0] = (2000);
//     dims[1] = (3000);
    
    
    track_shelf tracks;
    cout<<"total number of particles is: "<<bt.size()<<endl;

//     for(int k = 0;k<15;++k)
//     {
//       (bt.get_particle(k))->print();
//       cout<<      (bt.get_particle(k))->get_value(wrapper::d_frame)<<endl;
//     }
    
     
  
    //build hash case
    hash_case s(bt,dims,(int)max_r,frames);
    cout<<"case built"<<endl;

    

    // Compute G(r)
    vector<double> gofr_bin_count;
    vector<double> gofr_bin_edges;
    s.gofr_norm(max_r,bins,gofr_bin_count,gofr_bin_edges);
    vector_to_mat(plhs,gofr_bin_count);
    vector_to_mat(plhs +1,gofr_bin_edges);
    cout<<"gofr computed"<<endl;

//     //link tracks
//      s.link(2,tracks);
//     cout<<"linked"<<endl;
    
//     //compute the mean displacements from frame to frame
//     s.compute_mean_disp();
    
    
//     Array mean_frame_disp(frames);
//     s.get_mean_disp(mean_frame_disp);
//     Generic_parameters_matlab arr_parm(frames,2,plhs+2);
//     Generic_wrapper_base * wrapper = arr_parm.make_wrapper();
//     mean_frame_disp.set_array(wrapper);
    // delete wrapper;

//     cout<<"mean disp"<<endl;
    
//     //trim out short tracks
//     tracks.remove_short_tracks(10);
//     cout<<"trimed"<<endl;
  
//     // Compute msd

//     Svector<double> msd_vec;
//     Svector<int> msd_count_vec;
    
//     msd_vec.data.clear();
//     msd_vec.data.resize(200);
//     msd_count_vec.data.clear();
//     msd_count_vec.data.resize(200);
//     tracks.msd_corrected(msd_vec, msd_count_vec);
//     vector_to_mat(plhs+3, msd_vec.data);
//     vector_to_mat(plhs+4, msd_count_vec.data);
//     cout<<"c msd"<<endl;


//     // output tracks
//     Cell_matlab test_cell2(tracks.get_track_count(),plhs+5);
//     tracks.set_corrected_disp_to_cell(test_cell2);
//     cout<<"c tracks"<<endl;

    
    
    
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


