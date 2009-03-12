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

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>

//my includes




#include "mex.h"

#include "params_file.h"
#include "master_box_t.h"
#include "particle.h"
#include "hash_shelf.h"
#include "wrapper.h"

using namespace tracking;
using std::vector;
using std::map;

int main(int argc, char* argv[]) { 
  
  if(argc!=7){
    cout<<"Error, wrong number of arguments"<<endl;
    return -1;
  }

  vector<int> dims;
  dims.push_back(atoi(argv[2]));
  dims.push_back(atoi(argv[3]));
  int num_particles = atoi(argv[4]);
  double max_range = atof(argv[5]);
  int nbins = atoi(argv[6]);
  
  // set up the input wrapper 
  //nonsense to get the map set up
  map<wrapper::p_vals, int> contents;
  wrapper::p_vals tmp[] = {wrapper::d_xpos,
			   wrapper::d_ypos};
  int tmp2[] = {0, 1};
  vector<wrapper::p_vals > tmp3(tmp, tmp+2);
  vector<wrapper::p_vals>::iterator it1 = tmp3.begin();
  vector<int> tmp4(tmp2, tmp2+2);
  vector<int>::iterator it2 = tmp4.begin();
  map<wrapper::p_vals, int>::iterator it3 = contents.begin();
  for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
    contents.insert(it3,pair<wrapper::p_vals, int>(*it1, *it2));
  //end nonsense
  //there has to be a better way to do this
  params_file p_in(num_particles,argv[1],contents);


  
  // make master box
  master_box_t<particle_base>bt(&p_in,&p_in);

  hash_shelf table(dims, ((int) max_range), 0);



  for(int j = 0; j< (int)bt.size () ; ++j)
    {
      table.push(bt.get_particle(j));
    }

   table.print();
  
  vector<double> bin_count;
  vector<double> bin_edges;
  
  bin_count.resize (nbins);
  bin_edges.resize (nbins);
  
  table.gofr_norm (max_range,nbins,bin_count,bin_edges);

  ofstream myfile;
  myfile.open ("output.txt");
  vector<double>::iterator nit1 = bin_count.begin();
  vector<double>::iterator nit2 = bin_edges.begin();
  for(;nit1<bin_count.end();++nit1,++nit2){
    myfile << *nit1<<"\t"<<*nit2<<endl;
  }
  myfile.close();
  
    return 0;
}
