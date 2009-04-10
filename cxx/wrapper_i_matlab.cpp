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

#include "mex.h"

#include "wrapper_i.h"
#include "params_matlab.h"
#include "wrapper_i_matlab.h"

using namespace tracking;
using std::cout;

/**
   Implementation for wrapper_i of files.
 */


int wrapper_i_matlab::num_entries(){
  return rows;
}



double wrapper_i_matlab::get_value(int ind,  wrapper::p_vals type){
  //note that this is sideways to deal with the fact that matlab is
  //blasted coloum major


  std::map<p_vals,int>::const_iterator it = data_types.find(type);
  if(it == data_types.end())
    {
      throw "type not in wrapper";
    }
  
  int data_posistion = data_layout[type];
  if(data_posistion >=0)
    return *(first + ind  + rows * data_posistion);

  
  //deal with error
  cout<<"wraper does not store this type of data, size of set it: "
      <<data_types.size()
      <<   endl;
  //    return;
  return -123456789;


}


void wrapper_i_matlab::print(){
  cout<<"rows: "<<rows<<" cols: "<<cols<<endl;
}


wrapper_i_matlab::~wrapper_i_matlab(){
  //don't need to do anything here because everything is taken care of
  //by matlab, or so we hope.
  first = NULL;
  
}

wrapper_i_matlab::wrapper_i_matlab(params_matlab* param):
  wrapper_i_base(param->contains),
   rows(mxGetM(*(param->data_in))),
  cols(mxGetN(*(param->data_in))), first(mxGetPr(*(param->data_in)))
{
  
  //   //nonsense to get the map set up
  //   p_vals tmp[] = {d_index, d_xpos, d_ypos, d_I, d_r2, d_frame};
  //   int tmp2[] = {0, 1, 2 ,3,4,5};
    
  //   vector<wrapper::p_vals > tmp3(tmp, tmp+6);
  //   vector<p_vals>::iterator it1 = tmp3.begin();
    
  //   vector<int> tmp4(tmp2, tmp2+6);
  //   vector<int>::iterator it2 = tmp4.begin();

  //   map<p_vals, int>::iterator it3 = data_types.begin();

  //   for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
  //    data_types.insert(it3,pair<p_vals, int>(*it1, *it2));
  //end nonense
  //there has to be a better way to do this

  /*  cout<<data_types[d_index]<<"\t"
      <<data_types[d_xpos	]<<"\t"
      <<data_types[d_ypos	]<<"\t"
      <<data_types[d_I	]<<"\t"
      <<data_types[d_r2	]<<"\t"
      <<data_types[d_e    ]<<"\t"
      <<endl;
  */

  
}
