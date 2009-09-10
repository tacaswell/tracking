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

#include "wrapper_i_ning.h"
#include "wrapper_i_hdf5.h"
#include "params_ning.h"
// #include "hash_case3d.h"	

#include "histogram.h"
#include "svector.h"

using namespace tracking;
using std::cout;
using std::ifstream;
using std::endl;
using std::ios;
using std::vector;
using std::exception;


using utilities::Histogram;
using utilities::Svector;


using H5::DataSpaceIException;
using H5::DataSetIException ;
using H5::FileIException;





int main(){
  
  //nonsense to get the map set up
  map<utilities::D_TYPE, int> contents;
  //   utilities::D_TYPE tmp[] = {wrapper::D_INDEX,wrapper::D_XPOS,
  // 			   wrapper::D_YPOS, wrapper::d_zpos, 
  // 			   wrapper::D_FRAME};
  //   int tmp2[] = {0, 1, 2 ,3,4};
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
    


  try
  {
    
    Wrapper_i_hdf5 wrap(contents);
    master_box_t<particle_track> master_box;
    wrap.fill_master_box(master_box);
    
    cout<<"total number of particles is: "<<master_box.size()<<endl;;
    

  }
  
  // catch failure caused by the H5File operations
  catch( FileIException error )
  {
    error.printError();
    return -1;
  }

  // catch failure caused by the DataSet operations
  catch( DataSetIException error )
  {
    error.printError();
    return -1;
  }

  // catch failure caused by the DataSpace operations
  catch( DataSpaceIException error )
  {
    error.printError();
    return -1;
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


  


  return 0;
}

// tac 2009-07-17
// moved every thing to this comment block as I have no recollection of
// what I was doing here


//   //  mtrace();
//   //  params_file p = params_file(50*50,6);
//   //  params_ning_hd p = params_ning_hd(20464,6);
// //   map<utilities::D_TYPE, int> contents;
// //   utilities::D_TYPE tmp[] = {wrapper::D_INDEX,wrapper::D_XPOS,
// // 			   wrapper::D_YPOS, wrapper::d_zpos, 
// // 			   wrapper::D_FRAME};
// //   int tmp2[] = {0, 1, 2 ,3,4};
  
// //   vector<utilities::D_TYPE > tmp3(tmp, tmp+5);
// //   vector<utilities::D_TYPE>::iterator it1 = tmp3.begin();
  
// //   vector<int> tmp4(tmp2, tmp2+5);
// //   vector<int>::iterator it2 = tmp4.begin();
  
// //   map<utilities::D_TYPE, int>::iterator it3 = contents.begin();
  
// //   for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
// //     contents.insert(it3,pair<utilities::D_TYPE, int>(*it1, *it2));
//   //nonsense to get the map set up
//   map<utilities::D_TYPE, int> contents;
// //   utilities::D_TYPE tmp[] = {wrapper::D_INDEX,wrapper::D_XPOS,
// // 			   wrapper::D_YPOS, wrapper::d_zpos, 
// // 			   wrapper::D_FRAME};
// //   int tmp2[] = {0, 1, 2 ,3,4};
//   utilities::D_TYPE tmp[] = {wrapper::D_XPOS,
// 			   wrapper::D_YPOS, 
// 			   wrapper::D_FRAME};
//   int tmp2[] = {0, 1, 2 };

//   vector<utilities::D_TYPE > tmp3(tmp, tmp+3);
//   vector<utilities::D_TYPE>::iterator it1 = tmp3.begin();

//   vector<int> tmp4(tmp2, tmp2+3);
//   vector<int>::iterator it2 = tmp4.begin();

//   map<utilities::D_TYPE, int>::iterator it3 = contents.begin();

//   for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
//     contents.insert(it3,pair<utilities::D_TYPE, int>(*it1, *it2));
//   //end nonsense
//   //there has to be a better way to do this

//   int num_particles = 1000000;
  
//   params_file p_in = params_file(num_particles,"rawdata_from_matlab.txt",contents);
//   //  params_file p_in = params_file(num_particles,"new_dummy.txt",contents);

//   contents.insert(pair<utilities::D_TYPE, int>(wrapper::D_TRACKID,3));
  
//   params_file p_out = params_file(num_particles,"new_out.txt",contents);

//   Wrapper_in * in_wrapper = p_in.make_wrapper_in();
  

//   particle_base::intialize_wrapper_in(in_wrapper);
//   std::set<utilities::D_TYPE> data_types = in_wrapper->get_data_types();
//   data_types.insert(wrapper::D_UNQID);



//   particle_base::intialize_data_types(&data_types);
  
//   vector<particle_track*> working_vec(100,NULL);
  
//   for(vector<particle_track*>::iterator vec_it = working_vec.begin();
//       vec_it != working_vec.end(); ++vec_it)
//   {
//     (*vec_it) = new particle_track(0);
//   }
  
//   for(vector<particle_track*>::iterator vec_it = (working_vec.begin() +1);
//       vec_it != (working_vec.end()-1); ++vec_it)
//   {
//     (*vec_it)->set_next(*(vec_it+1));
//     (*vec_it)->set_prev(*(vec_it-1));
//   }
//   working_vec[0]->set_next(working_vec[1]);
//   working_vec[99]->set_prev(working_vec[98]);

//   // for(vector<particle_track*>::iterator vec_it = working_vec.begin();
// //       vec_it != working_vec.end(); ++vec_it)
// //   {
// //     const particle_track* tmp;
// //     const particle_track* working;
// //     working = *vec_it;
// //     cout<<working->get_value(wrapper::D_UNQID)<<"\t";
// //     tmp = working->get_prev();
// //     if(tmp!=NULL)
// //     {
// //       cout<<tmp->get_value(wrapper::D_UNQID)<<"\t";
// //     }
// //     else
// //     {
// //       cout<<"null\t";
// //     }
// //     tmp = working->get_next();
// //     if(tmp!=NULL)
// //     {
// //       cout<<tmp->get_value(wrapper::D_UNQID)<<"\t";
// //     }
// //     else
// //     {
// //       cout<<"null\t";
// //     }
// //     cout<<endl;
// //   }
  
//   bool more = true;
//   const particle_track* tmp_t;
//   tmp_t= working_vec[0];
  
//   while(more)
//   {
//     if(tmp_t!=NULL)
//     {
//       cout<<tmp_t->get_value(wrapper::D_UNQID)<<"\t";
//     }
//     else
//     {
//       cout<<"null\t";
//     }
//     cout<<endl;
//     more = tmp_t->step_forwards(6,tmp_t);
//   }
  
  

// //   master_box_t<particle_track>bt(&p_in,&p_out);
// //   cout<<"total number of particles is: "<<bt.size()<<endl;;
  
// //    int x_dim = 1385;
// //    int y_dim = 512;
// // //  int x_dim = 150;
// // //  int y_dim = 150;
  
// //   vector<int> dims;
// //   dims.push_back(y_dim);
// //   dims.push_back(x_dim);



// //   int frames;
// //   frames = (int)bt.get_particle(bt.size() -1)->get_value(wrapper::D_FRAME);

  
// //   hash_case s(bt,dims,5,frames+1);
  

  
// //   track_shelf tracks;
  


// //   s.link(5,tracks);



// //   Svector<double> msd_vec;
// //   Svector<int> msd_count_vec;
  
// //   msd_vec.data.resize(500);
// //   msd_count_vec.data.resize(500);


// //   tracks.remove_short_tracks(200);

// //   tracks.msd(msd_vec, msd_count_vec);
  
// //   msd_vec.print();
// //   msd_count_vec.print();
  

// //   Histogram hist1(25,0,frames);
// //   Histogram hist2(25,0,frames);

// //   tracks.track_length_histogram(hist1);
// //   tracks.remove_short_tracks(200);
// //   tracks.track_length_histogram(hist2);
// //   hist1.print();
// //   hist2.print();

// //   bt.initialize_out();
// //   tracks.set_shelf();
// //   bt.finalize_out();

