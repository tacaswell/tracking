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


#include "wrapper_i_ning_hd.h"
#include "wrapper_i_ning.h"
#include "params_ning_hd.h"


using namespace tracking;
using std::cout;
using std::ifstream;
using std::endl;
using std::ios;

wrapper_i_ning_hd::wrapper_i_ning_hd(params_file* param) :wrapper_i_ning(param){
  fill_data(param->fname,param->rows,5);
};

void wrapper_i_ning_hd::fill_data(std::string file_name, int row, int col){
  //magic number, fix this
  rows = row;
  cols = 5;
  first = (double*)malloc(rows*cols*sizeof(double));
  
  double tmp=0;
  
  
  ifstream indatafile;
  //add something to deal with the case where the file
  //doesn't exist, but everything seems happy
  indatafile.open(file_name.c_str(),ios::in);
  cout<<file_name<<endl;
  //this is all magic code, needs to be generalized
  

  for(int j = 0 ; j<rows; j++){
    indatafile >> tmp;
    *(first + j*cols) = tmp;

    for(int n = 0; n<3; n++){
      indatafile >> tmp;
      //move the origin to the corner, not the center (to deal with
      //the fact that all the code is written with the assumption
      //that all of the posistions are postive and scale up to make
      //the box look bigger (purely for my brain to be happier)
      *(first  + j*cols+n+1) = (tmp+.57)*50;
      assert((tmp+.7)>0);

      
    }
    indatafile >> tmp;
    *(first  + j*cols+4) = tmp;
    
  }

 
  cout<<rows<<endl;
  indatafile.close();
};
