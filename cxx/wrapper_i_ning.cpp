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
#include<iostream>
#include<fstream>
#include <stdlib.h>
#include <string>
#include <assert.h>

#include "wrapper_i_ning.h"
#include "params_file.h"


using namespace tracking;
using std::cout;
using std::ifstream;
using std::endl;
using std::ios;

/*
   Implementation for wrapper_i of files.
 */

void wrapper_i_ning::init(){
//   p_vals tmp[] = {d_index, d_xpos, d_ypos, d_zpos, d_frame};
//   int tmp2[] = {0, 1, 2 ,3,4};

//   vector<wrapper::p_vals > tmp3(tmp, tmp+5);
//   vector<p_vals>::iterator it1 = tmp3.begin();

//   vector<int> tmp4(tmp2, tmp2+5);
//   vector<int>::iterator it2 = tmp4.begin();

//   map<p_vals, int>::iterator it3 = contents.begin();

//   for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
//     contents.insert(it3,pair<p_vals, int>(*it1, *it2));
  
  /*  cout<<contents[d_index]<<"\t"
      <<contents[d_xpos	]<<"\t"
      <<contents[d_ypos	]<<"\t"
      <<contents[d_I	]<<"\t"
      <<contents[d_r2	]<<"\t"
      <<contents[d_e    ]<<"\t"
      <<endl;*/

}

wrapper_i_ning::wrapper_i_ning(std::map<wrapper::p_vals,int>map_in)
  :wrapper_i_base(map_in)
{

}


wrapper_i_ning::wrapper_i_ning(params_file* param):
  wrapper_i_base(param->contains){
  //  init();
  fill_data(param->fname,param->rows,5);
  
}


void wrapper_i_ning::fill_data(string file_name, int row, int col){
  rows = 100*1000;
  cols = 5;
  first = (double*)malloc(rows*cols*sizeof(double));
  

  
  
  ifstream indatafile;
  //add something to deal with the case where the file
  //doesn't exist, but everything seems happy
  indatafile.open(file_name.c_str(),ios::in);
  cout<<file_name<<endl;
  double tmp=0;
  double dtime=0;
  int time = 0;
  //this is all magic code, needs to be generalized
  for(int k = 0; k<100; k++){
    indatafile >> dtime;
    time = (int)dtime - 1001;

    for(int j = 0 ; j<1000; j++){
      *(first + k*1000*cols + j*cols) = j;
      for(int n = 0; n<3; n++){
	indatafile >> tmp;
	//move the origin to the corner, not the center (to deal with
	//the fact that all the code is written with the assumption
	//that all of the posistions are postive and scale up to make
	//the box look bigger (purely for my brain to be happier)
	*(first + k*1000*cols + j*cols+n+1) = (tmp+.7)*50;
	assert((tmp+.7)>0);
      }
      *(first + k*1000*cols + j*cols+4) = time;
    }
  }
 
  cout<<rows<<endl;
  indatafile.close();
}



int wrapper_i_ning::num_entries(){
  return rows;
}


 double wrapper_i_ning::get_value(int ind,  wrapper::p_vals type){


 //   map<wrapper::p_vals,int>::iterator it = data_types.find(type);

 //   if(it !=data_types.end()){
 //     return *(first + ind * cols + (*it).second);
 //   }

   int data_posistion = data_layout[type];
   if(data_posistion >=0)
     return *(first + ind * cols + data_posistion);

   //deal with error
   cout<<"wraper does not store this type of data, size of set it: "
       <<data_types.size()
       <<   endl;
   //    return;
   return -123456789;




}

void wrapper_i_ning::print(){
  cout<<"rows: "<<rows<<" cols: "<<cols<<endl;
}


wrapper_i_ning::~wrapper_i_ning(){
  free(first);
}

