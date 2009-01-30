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
#include "hash_box.h"
using namespace tracking;
void hash_box::append(hash_box* next){
  
  vector<particle_base *>::iterator it;
  for (it = next->contents.begin(); it<next->contents.end(); it++)
    contents.push_back(*it);

}

void hash_box::print(){
  
  vector<particle_base *>::iterator it;
  for (it = contents.begin(); it<contents.end(); it++)
    (*it)->print();

}

void hash_box::get_val_vec(vector<double> & vec, wrapper::p_vals type){
  
  vec.reserve(contents.size());
  vector<particle_base *>::iterator it;
  
  for (it = contents.begin(); it<contents.end(); it++){
    vec.push_back((double)(*it)->get_value(type));
  }
  
}

list<particle_track*>* hash_box::box_to_list(){

  list<particle_track*>* tmp = new list<particle_track*>;
  
  for(vector<particle_base*>::iterator it = contents.begin();
      it<contents.end(); it++)
    {
      tmp->push_back(static_cast<particle_track*>(*it));
    }


  return tmp;

}
