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

  if(nlhs!=19 || nrhs!=5){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }
  try{

    utilities::Tuple dims;	
    dims[0] = (int)mxGetScalar(prhs[1]);
    dims[1] = (int)mxGetScalar(prhs[2]);
    int frames = (int)mxGetScalar(prhs[3]);

    double max_r = mxGetScalar(prhs[4]);




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
    contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
    params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),
					 contents.size());


    master_box_t<particle_track>bt(&p_in,&p_out);

    track_shelf tracks;
    cout<<"total number of particles is: "<<bt.size()<<endl;;
    //build hash case
    hash_case s(bt,dims,(int)max_r,frames);
    cout<<"case built"<<endl;

    //link tracks
    s.link(max_r,tracks);
    cout<<"linked"<<endl;
    
    
    
//     Histogram hist1(frames,0,frames);
//     tracks.track_length_histogram(hist1);



    //compute the mean displacements from frame to frame
    s.compute_mean_disp();
    

    
    Array mean_frame_disp(frames);
    s.get_mean_disp(mean_frame_disp);
    Generic_parameters_matlab arr_parm(frames,2,plhs);
    Generic_wrapper_base * wrapper = arr_parm.make_wrapper();
    mean_frame_disp.set_array(wrapper);
    delete wrapper;
    wrapper = NULL;
    
    cout<<"mean disp"<<endl;
    
    //trim out short tracks
    tracks.remove_short_tracks(15);
    cout<<"trimed"<<endl;
  
    // Compute msd

    //  
    //  
  
    time_t start,end;
    double dif;
    
    int steps = 25;
    

    time (&start); 
    Svector<double> msd_vec;
    Svector<int> msd_count_vec;
    
    time (&start); 
    Counted_vector msd(steps);
    Counted_vector msd_sq(steps);
    tracks.msd_corrected(msd,msd_sq);
    Generic_parameters_matlab arr_parm2(1,steps,plhs+1);
    Generic_wrapper_base * wrapper1 = arr_parm2.make_wrapper();
    arr_parm2.change_mxArray(plhs+2);
    Generic_wrapper_base * wrapper2 = arr_parm2.make_wrapper();
    msd.output_to_wrapper(wrapper1,wrapper2);
    
    arr_parm2.change_mxArray(plhs+17);
    Generic_wrapper_base * wrapper17 = arr_parm2.make_wrapper();
    arr_parm2.change_mxArray(plhs+18);
    Generic_wrapper_base * wrapper18 = arr_parm2.make_wrapper();
    msd_sq.output_to_wrapper(wrapper1,wrapper2);
    time (&end); 
    dif = difftime (end,start);
    cout<<"c msd: "<<dif<<endl;

    delete wrapper1;
    delete wrapper2;
    wrapper1= NULL;
    wrapper2 = NULL;
    delete wrapper17;
    delete wrapper18;
    wrapper17= NULL;
    wrapper18= NULL;



    Generic_parameters_matlab arr_parm3(2500,10,plhs+3);
    Generic_wrapper_base * wrapper3 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+4);
    Generic_wrapper_base * wrapper4 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+5);

    Generic_wrapper_base * wrapper5 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+6);
    Generic_wrapper_base * wrapper6 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+7);

    Generic_wrapper_base * wrapper7 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+8);
    Generic_wrapper_base * wrapper8 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+9);

    Generic_wrapper_base * wrapper9 = arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+10);
    Generic_wrapper_base * wrapper10 =arr_parm3.make_wrapper();

    arr_parm3.change_mxArray(plhs+11);
    Generic_wrapper_base * wrapper11 =arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+12);
    Generic_wrapper_base * wrapper12 =arr_parm3.make_wrapper();

    
    arr_parm3.change_mxArray(plhs+13);
    Generic_wrapper_base * wrapper13 =arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+14);
    Generic_wrapper_base * wrapper14 =arr_parm3.make_wrapper();

    arr_parm3.change_mxArray(plhs+15);
    Generic_wrapper_base * wrapper15 =arr_parm3.make_wrapper();
    arr_parm3.change_mxArray(plhs+16);
    Generic_wrapper_base * wrapper16 =arr_parm3.make_wrapper();

    

    Coarse_grain_array Drr  (5,100,2500,10);
    Coarse_grain_array Dtt  (5,100,2500,10);
    Coarse_grain_array Ddrdr(5,100,2500,10);
    Coarse_grain_array Dyy  (5,100,2500,10);
    Coarse_grain_array Dxx  (5,100,2500,10);
    Coarse_grain_array Duu  (5,100,2500,10);
    Coarse_grain_array Ddudu(5,100,2500,10);
    cout<<"rehash"<<endl;
    s.rehash(100);
    cout<<"rehashed"<<endl;
    
    msd.average_data();
    

    cout<<"trying 2 point "<<endl;
    s.D_lots(Drr,Dtt,Ddrdr,Dxx,Dyy,Duu,Ddudu,msd);
    cout<<"2 point computed"<<endl;
    
    Drr.output_to_wrapper(wrapper3,wrapper4);
    Dtt.output_to_wrapper(wrapper5,wrapper6);
    Ddrdr.output_to_wrapper(wrapper7,wrapper8);
    Dxx.output_to_wrapper(wrapper9,wrapper10);
    Dyy.output_to_wrapper(wrapper11,wrapper12);
    Duu.output_to_wrapper(wrapper13,wrapper14);
    Ddudu.output_to_wrapper(wrapper15,wrapper16);


    delete wrapper3;
    delete wrapper4;
    delete wrapper5;
    delete wrapper6;
    delete wrapper7;
    delete wrapper8;
    delete wrapper9;
    delete wrapper10;
    delete wrapper11;
    delete wrapper12;
    delete wrapper13;
    delete wrapper14;
    delete wrapper15;
    delete wrapper16;
    
    
    



//     // output tracks
//     Cell_matlab test_cell2(tracks.get_track_count(),plhs+3);
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


