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
#include<exception>

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
#include "counted_vector.h"
#include "coarse_grain_array.h"

#include "generic_wrapper_matlab.h"
#include "generic_parameters_matlab.h"

#include "cell_matlab.h"

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
using utilities::Counted_vector;
using utilities::Coarse_grain_array;

extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=9 || nrhs!=5){
    std::cerr<<"Error, wrong number of arguments:"<<nlhs<<" "<<nrhs<<endl;
    return;
  }
  try{

    utilities::Pair dims;	
    dims[0] = (int)mxGetScalar(prhs[1]);
    dims[1] = (int)mxGetScalar(prhs[2]);
    int frames = (int)mxGetScalar(prhs[3]);

    double max_r = mxGetScalar(prhs[4]);




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

    int msd_steps = 250;
    int corr_steps = 5;

    // set up the wrappers
    vector<Generic_wrapper_base *> wrapper_vec(8);
    
    Generic_parameters_matlab arr_parm(frames,2,plhs);
    wrapper_vec[0] = arr_parm.make_wrapper();
    Generic_parameters_matlab arr_parm2(1,msd_steps,plhs+1);
    for(int j = 0;j<6; ++j)
    {
      arr_parm2.change_mxArray(plhs+j+1);
      wrapper_vec[1+j] = arr_parm2.make_wrapper();
    }

//     Generic_parameters_matlab arr_parm3(2500,corr_steps,plhs+3);
//     for(int j = 0;j<10; ++j)
//     {
//       arr_parm3.change_mxArray(plhs+j+7);
//       wrapper_vec[7+j] = arr_parm3.make_wrapper();
//     }

//     Generic_parameters_matlab arr_parm4(502,2,plhs+17);
//     wrapper_vec[17] = arr_parm4.make_wrapper();



    params_matlab p_in = params_matlab(prhs,contents);
    contents.insert(pair<utilities::D_TYPE, int>(wrapper::D_TRACKID,3));
    params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),
					 contents.size());



    // start actual computation
    master_box_t<particle>bt(&p_in,&p_out);

    track_shelf tracks;
    cout<<"total number of particles is: "<<bt.size()<<endl;;
    //build hash case
    hash_case s(bt,dims,(int)max_r,frames);
    cout<<"case built"<<endl;

    //link tracks
    s.link(max_r,tracks);
    cout<<"linked"<<endl;
    
    


    //compute the mean displacements from frame to frame
        s.compute_mean_disp();
    

    
    Array mean_frame_disp(frames);
    s.get_mean_disp(mean_frame_disp);

    mean_frame_disp.set_array(wrapper_vec[0]);
    
    
    cout<<"mean disp"<<endl;
    
    //trim out short tracks
    tracks.remove_short_tracks(15);
    cout<<"trimmed"<<endl;
  
    
    // Compute msd

    //  
    //  
  
    time_t start,end;
    double dif;
    
    
    

    time (&start); 
    Counted_vector md(msd_steps);
    Counted_vector msd(msd_steps);
    Counted_vector msd_sq(msd_steps);
    tracks.msd_corrected(md,msd,msd_sq);
    md.output_to_wrapper(wrapper_vec[1],wrapper_vec[2]);
    msd.output_to_wrapper(wrapper_vec[3],wrapper_vec[4]);
    msd_sq.output_to_wrapper(wrapper_vec[5],wrapper_vec[6]);

    time (&end); 
    dif = difftime (end,start);
    cout<<"c msd: "<<dif<<endl;
    
    
    Generic_parameters_matlab arr_parm3(tracks.get_track_count(),3,plhs+7);
    wrapper_vec[7] = arr_parm3.make_wrapper();



    // output tracks
    Cell_matlab test_cell2(tracks.get_track_count(),plhs+8);
    // tracks.set_corrected_disp_to_cell(test_cell2);
//     tracks.initial_corrected_pos_to_wrapper(wrapper_vec[7]);
    tracks.corrected_tracks_out(test_cell2,wrapper_vec[7]);
    
    cout<<"c tracks"<<endl;


    for(int j = 0;j<wrapper_vec.size(); ++j)
    {
      delete wrapper_vec[j];
      wrapper_vec[j] = NULL;
    }

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


