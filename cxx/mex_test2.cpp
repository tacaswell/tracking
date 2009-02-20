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
#include "matrix.h"


#include "matlab_utils.h"


#include "histogram.h"
#include "svector.h"


using namespace tracking;
using utilities::array_to_mat;
using utilities::vector_to_mat;
using utilities::Svector;
using utilities::Histogram;

extern void _main();

//globals :(
master_box_t<particle_track> master_box;
hash_case hcase;
track_shelf tracks;


void my_mex_exit_func(void)
 {
// 	delete hcase;
// 	hcase = NULL;

// 	delete master_box;
// 	master_box = NULL;
	
	cout<<"ran at exit"<<endl;

 }

void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  mexAtExit(my_mex_exit_func);

  if(mexIsLocked()){
      int mode;

      if (!mxIsDouble(prhs[0]) ||  
	  mxGetN(prhs[0])*mxGetM(prhs[0]) != 1  || mxIsComplex(prhs[0])) {
	mexErrMsgTxt("Input argument must be a real scalar double");
      }
      
      mode = (int) mxGetScalar(prhs[0]);
      
	
      Histogram hist1(25,0,100);
      Histogram hist2(25,0,100);


      switch(mode){
	//-1 cleans up the library
      case -1:
	
// 	delete hcase;
// 	hcase = NULL;

// 	delete master_box;
// 	master_box = NULL;

	mexUnlock();	
	
	cout<<"unlocked"<<endl;
	
	break;
	
	//trim tracks
      case 0:
	if(nrhs != 2)
	  mexErrMsgTxt("need 2 input arguments");
  
	tracks.track_length_histogram(hist1);
	tracks.remove_short_tracks((int) mxGetScalar(prhs[1]));
	tracks.track_length_histogram(hist2);
	hist1.print();
	hist2.print();

	break;

	
      case 1:
	break;
	
      case 2:
	break;
	
      case 3:
	break;

      }
      
      
      
    
  }
  else{
    
    
    
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
    contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
  
    params_matlab p_out 
      = params_matlab(plhs,contents,mxGetM(*prhs),contents.size());
    

    // params_ning p_in = params_ning(3,100*1000,contents);

    //  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_unqid,4));
    //  contents[wrapper::d_index] = 5;
    //  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
    //  params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),contents.size());
    //  params_file p_out = params_file(5,contents);
    //  master_box b = master_box(&p,&p,6);

    master_box.init(&p_in,&p_out);
  
    vector<int> dims;	
    //  for(int t = 0; t<3;t++)	
    //    dims.push_back(80);    
    dims.push_back(520);
    dims.push_back(1400);

    //  dims.push_back(50);
    //    tracks = new track_shelf();
    
    cout<<"total number of particles is: "<<master_box.size()<<endl;
    
    //    hcase.init(master_box,dims,5,1462);
    //    hcase.link(5,tracks);
    mexLock();

  }
  
//   bt.initialize_out();
//   tracks.set_shelf();
//   bt.finalize_out();


 


  //  s.print();


  
  
  

  Svector<double> msd_vec;
  Svector<int> msd_count_vec;
  
//   msd_vec.data.resize(100);
//   msd_count_vec.data.resize(100);



//   tracks.msd(msd_vec, msd_count_vec);

  
//  vector_to_mat(plhs+1, msd_count_vec.data);
  
//   Histogram hist3(100,0,1000);
//   tracks.msd_hist(25,hist3);
//   vector<int> tmp465 = hist3.get_bin_values();
//   vector_to_mat(plhs, tmp465);
//   cout<<hist3.get_over_count()<<hist3.get_under_count()<<endl;

  return;
}
