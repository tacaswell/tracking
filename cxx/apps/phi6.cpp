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


#include "hash_case.h"


#include "wrapper_i_matlab.h"
#include "params_matlab.h"



#include "mex.h"

#include "matlab_utils.h"



using namespace tracking;
using std::exception;
using std::cerr;



extern void _main();
void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray* prhs[] ){

  if(nlhs!=0|| nrhs!=5){
    cout<<"Error, wrong number of arguments"<<endl;
    return;
  }
  try{

      
    utilities::Tuple dims;	
    dims[0] = (int)mxGetScalar(prhs[1]);
    dims[1] = (int)mxGetScalar(prhs[2]);
    int frames = (int)mxGetScalar(prhs[3]);
    float neighbor_range =  (float)mxGetScalar(prhs[4]);;

 
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

    //this is meaningless and not used
    params_matlab p_out = params_matlab(plhs,contents,mxGetM(*prhs),
 					contents.size());
    

    
    master_box_t<particle_base>master_box(&p_in,&p_out);
    cout<<"total number of particles is: "<<master_box.size()<<endl;;
    
    
    hash_case h_case(master_box,dims,(int)ceil(neighbor_range),frames);
    cout<<"case built"<<endl;
    
    h_case.print();
    
    particle_base::set_neighborhood_range(neighbor_range);
    
    h_case.pass_fun_to_shelf(&hash_shelf::fill_in_neighborhood);
    h_case.pass_fun_to_part(&particle_base::fill_phi_6);
    h_case.pass_fun_to_part(&particle_base::print);

    
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

