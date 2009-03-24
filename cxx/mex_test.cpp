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
#include <exception>

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

#include "matlab_utils.h"


#include "histogram.h"
#include "svector.h"

#include "array.h"

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
extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=2 && nrhs!=1){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }
  try{
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
  
    params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),
					contents.size());
  

    // params_ning p_in = params_ning(3,100*1000,contents);

    //  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_unqid,4));
    //  contents[wrapper::d_index] = 5;
    //  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
    //  params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),contents.size());
    //  params_file p_out = params_file(5,contents);
    //  master_box b = master_box(&p,&p,6);

    master_box_t<particle_track>bt(&p_in,&p_out);
  
    vector<int> dims;	
    //  for(int t = 0; t<3;t++)	
    //    dims.push_back(80);    
    dims.push_back(520);
    dims.push_back(1400);

    //  dims.push_back(50);
    track_shelf tracks;
    
    cout<<"total number of particles is: "<<bt.size()<<endl;;
  
    int frames = 1462;
    //    hash_case s(bt,dims,5,1462);
    hash_case s(bt,dims,5,frames);
    cout<<"case built"<<endl;
    s.link(5,tracks);
  
    cout<<"linked"<<endl;
    s.compute_mean_disp();
  
    Array test_a(frames);
    s.get_cum_disp(test_a);

    Generic_parameters_matlab arr_parm(frames,2,plhs+1);

    Generic_wrapper_base * wrapper = arr_parm.make_wrapper();
  
    test_a.set_array(wrapper);

  
    //  s.print();
  
    bt.initialize_out();
    tracks.set_shelf();
    bt.finalize_out();
    
    Cell_matlab test_cell(tracks.get_track_count(),plhs+2);
    tracks.set_raw_disp_to_cell(test_cell);
    

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

  //   //  s.print();


  
  
  //   Histogram hist1(25,0,100);
  //   Histogram hist2(25,0,100);
  
  //   tracks.track_length_histogram(hist1);
  //   tracks.remove_short_tracks(10);
  //   tracks.track_length_histogram(hist2);
  //   hist1.print();
  //   hist2.print();
  
  

  //   Svector<double> msd_vec;
  //   Svector<int> msd_count_vec;
  
  // //   msd_vec.data.resize(100);
  // //   msd_count_vec.data.resize(100);



  // //   tracks.msd(msd_vec, msd_count_vec);

  
  // //  vector_to_mat(plhs+1, msd_count_vec.data);
  
  //   Histogram hist3(100,0,1000);
  //   tracks.msd_hist(25,hist3);
  // //    vector<int> tmp465 = hist3.get_bin_values();
  // //    vector_to_mat(plhs, tmp465);
  //   cout<<hist3.get_over_count()<<hist3.get_under_count()<<endl;

  return;
}
