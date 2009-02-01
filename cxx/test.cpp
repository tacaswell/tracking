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

#include "wrapper_i_ning.h"
#include "params_ning.h"
#include "hash_case3d.h"

#include "histogram.h"

using namespace tracking;
using std::cout;
using std::ifstream;
using std::endl;
using std::ios;

using utilities::Histogram;
/*//defined constants*/
#define NUMCOLUMNS 6


void vec_print2(vector<double> in){
  for(unsigned int j = 0 ; j<in.size(); j++)
    cout<<in.at(j)<<"\t";
  cout<<endl;
}






int main(){
  //  mtrace();
  //  params_file p = params_file(50*50,6);
  //  params_ning_hd p = params_ning_hd(20464,6);
//   map<wrapper::p_vals, int> contents;
//   wrapper::p_vals tmp[] = {wrapper::d_index,wrapper::d_xpos,
// 			   wrapper::d_ypos, wrapper::d_zpos, 
// 			   wrapper::d_frame};
//   int tmp2[] = {0, 1, 2 ,3,4};
  
//   vector<wrapper::p_vals > tmp3(tmp, tmp+5);
//   vector<wrapper::p_vals>::iterator it1 = tmp3.begin();
  
//   vector<int> tmp4(tmp2, tmp2+5);
//   vector<int>::iterator it2 = tmp4.begin();
  
//   map<wrapper::p_vals, int>::iterator it3 = contents.begin();
  
//   for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
//     contents.insert(it3,pair<wrapper::p_vals, int>(*it1, *it2));
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

  int num_particles = 1000000;
  
  params_file p_in = params_file(num_particles,"rawdata_from_matlab.txt",contents);

  contents.insert(pair<wrapper::p_vals, int>(wrapper::d_trackid,3));
  
  params_file p_out = params_file(num_particles,"new_out.txt",contents);


  master_box_t<particle_track>bt(&p_in,&p_out);
  cout<<"total number of particles is: "<<bt.size()<<endl;;
  
  vector<int> dims;
  dims.push_back(512);
  dims.push_back(1385);


  int frames;
  frames = bt.get_particle(bt.size() -1)->get_value(wrapper::d_frame);

  hash_case s(bt,dims,5,frames+1);
  
  track_shelf tracks;
  
  s.link(5,tracks);

  Histogram hist1(25,0,frames);
  Histogram hist2(25,0,frames);

  tracks.track_length_histogram(hist1);
  tracks.remove_short_tracks(200);
  tracks.track_length_histogram(hist2);
  hist1.print();
  hist2.print();


//   bt.initialize_out();
//   tracks.set_shelf();
//   bt.finalize_out();


  return 0;
}
