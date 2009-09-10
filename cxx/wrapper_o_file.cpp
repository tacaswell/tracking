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
#include "wrapper_o_file.h"
#include "params_file.h"
#include <iostream>
#include <stdexcept> // out_of_range exception




using namespace tracking;






///print out a representation of the data
void wrapper_o_file::print()const{
//   for(unsigned int j = 0; j<data.size();j++){
//     for(unsigned int k = 0; k<(data.at(j)).size();k++)
//       cout<<(data.at(j)).at(k)<<" ";
//     cout<<endl;
//   }
}
 
void wrapper_o_file::initialize(params_file* param){
  //This clearly needs to be fixed to do something with the 
  //parameter file
  p_vals tmp[] = {
    wrapper::D_INDEX, 
    wrapper::D_XPOS, 
    wrapper::D_YPOS, 
    wrapper::D_I, 
    wrapper::D_R2, 
    wrapper::D_E
  };
  int tmp2[] = {0, 1, 2 ,3,4,5};
  
  vector<utilities::D_TYPE > tmp3(tmp, tmp+6);
  vector<p_vals>::iterator it1 = tmp3.begin();

  vector<int> tmp4(tmp2, tmp2+6);
  vector<int>::iterator it2 = tmp4.begin();

  map<p_vals, int>::iterator it3 = contents.begin();

  for( ;it2<tmp4.end() && it1<tmp3.end() ; it1++, it2++, it3++)
    contents.insert(it3,pair<p_vals, int>(*it1, *it2));
  
  cols = 6;
  fname = string("out.txt");

}

// int wrapper_o_file::add_particle(){
//   data.push_back(vector<double> (cols,0));
//   return seq_count++;
// }

// void wrapper_o_file::set_value(int ind, utilities::D_TYPE type,double val){
//   (data.at(ind)).at(contents[type]) = val;
// }

wrapper_o_file::wrapper_o_file(params_file* param):wrapper_o_base(param->contains),
						   fname(param->fname),cols((param->contains).size()){
  
}

// void wrapper_o_file::finalize(){

// }

void wrapper_o_file::print(int ind){
//   for(unsigned int k = 0; k<(data.at(ind)).size(); k++)
//     cout<<(data.at(ind)).at(k)<<"\t";
//   cout<<endl;
}


void wrapper_o_file::initialize_wrapper(){
  f_out.open(fname.c_str());
  wrapper_open = true;
}

void wrapper_o_file::start_new_particle(){
  if(part_open){
    cout<<"particle already open w_o_f"<<endl;
    return;
  }


  data.resize(cols,-1);
  part_open = true;
  
  
}

void wrapper_o_file::set_new_value(utilities::D_TYPE type, double val){
  
  int data_posistion = data_layout_[type];
  if(data_posistion >=0){
    if(part_open){
      //change to [] eventually?
      data.at(data_posistion) = val;
      return;
    }
    //deal with fail condition
    cout<<"no particle is open"<<endl;
  }
  cout<<"wrapper doesn't have this data_type"<<endl;
}
void wrapper_o_file::end_new_particle(){

  for(vector<double>::iterator it = data.begin(); it!=data.end(); it++)
      f_out<<(*it)<<"\t";

  f_out<<endl;

  data.clear();
  part_open = false;

}
void wrapper_o_file::finalize_wrapper(){
  
  
  cout<<"writting out the file"<<endl;
  cout<<fname<<endl;

  f_out.close();
  
  wrapper_open = false;

}

void wrapper_o_file::reset_wrapper(params * param){
}


