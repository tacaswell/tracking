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
#include "params_matlab.h"
#include "wrapper_i_matlab.h"

using namespace utilities;
using std::cout;
using std::complex;
using std::endl;
using std::set;
using std::map;

/**
   Implementation for wrapper_i of files.
 */


int Wrapper_i_matlab::get_num_entries()const{
  return rows;
}



float Wrapper_i_matlab::get_value(int ind,  utilities::D_TYPE type,int junk)const{
  //note that this is sideways to deal with the fact that matlab is
  //blasted coloum major


//   std::map<utilities::D_TYPE,int>::const_iterator it = data_types_.find(type);
//   if(it == data_types_.end())
//     {
//       throw "type not in wrapper";
//     }
  
  int data_posistion = data_map_(type);
  if(data_posistion >=0)
    return *(first + ind  + rows * data_posistion);

  
  //deal with error
  cout<<"wrapper does not store this type of data, size of set is: "
      <<data_types_.size()
      <<   endl;
  //    return;
  return -123456789;


}

float Wrapper_i_matlab::get_value(float &out,int ind, utilities::D_TYPE type,int frame)const
{
  out = get_value(ind,type,frame);
}

complex<float> Wrapper_i_matlab::get_value(std::complex<float> &out,int ind, utilities::D_TYPE type,int frame)const
{
  throw " this wrapper type does not deal with complex yet";
}


void Wrapper_i_matlab::print()const{
  cout<<"rows: "<<rows<<" cols: "<<cols<<endl;
}


Wrapper_i_matlab::~Wrapper_i_matlab(){
  //don't need to do anything here because everything is taken care of
  //by matlab, or so we hope.
  first = NULL;
  
}


std::set<utilities::D_TYPE> Wrapper_i_matlab::get_data_types() const{
  set<utilities::D_TYPE> tmp;
  for(map<utilities::D_TYPE,int>::const_iterator it = data_types_.begin();
      it!= data_types_.end(); it++)
    tmp.insert((*it).first);
  return set<utilities::D_TYPE>(tmp);
}


void Wrapper_i_matlab::get_data_types(std::set<utilities::D_TYPE>& out) const{
  out = get_data_types();
  
}


Wrapper_i_matlab::Wrapper_i_matlab(params_matlab* param):
  rows(mxGetM(*(param->data_in))),
  cols(mxGetN(*(param->data_in))), first(mxGetPr(*(param->data_in))),
  data_map_(param->contains)
{
  
  //   //nonsense to get the map set up
  //   p_vals tmp[] = {d_index, D_XPOS, D_YPOS, d_I, d_r2, D_FRAME};
  //   int tmp2[] = {0, 1, 2 ,3,4,5};
    
  //   vector<utilities::D_TYPE > tmp3(tmp, tmp+6);
  //   vector<p_vals>::iterator it1 = tmp3.begin();
    
  //   vector<int> tmp4(tmp2, tmp2+6);
  //   vector<int>::iterator it2 = tmp4.begin();

  //   map<p_vals, int>::iterator it3 = data_types.begin();

  //   for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
  //    data_types.insert(it3,pair<p_vals, int>(*it1, *it2));
  //end nonense
  //there has to be a better way to do this

  /*  cout<<data_types[d_index]<<"\t"
      <<data_types[D_XPOS	]<<"\t"
      <<data_types[D_YPOS	]<<"\t"
      <<data_types[d_I	]<<"\t"
      <<data_types[d_r2	]<<"\t"
      <<data_types[d_e    ]<<"\t"
      <<endl;
  */


  
}
