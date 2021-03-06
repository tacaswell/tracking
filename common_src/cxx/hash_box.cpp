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
#include "particle_track.h"

using namespace tracking;
using std::list;
using std::vector;
void hash_box::append(hash_box* next){
  
  vector<particle *>::iterator it;
  for (it = next->contents_.begin(); it!=next->contents_.end(); it++)
    contents_.push_back(*it);

}

void hash_box::print(){
  
  vector<particle *>::iterator it;
  for (it = contents_.begin(); it!=contents_.end(); it++)
    (*it)->print();

}

// void hash_box::get_val_vec(vector<float> & vec, utilities::D_TYPE type){
  
//   vec.reserve(contents_.size());
//   vector<particle *>::iterator it;
  
//   float tmp;
  
//   for (it = contents_.begin(); it!=contents_.end(); ++it)
//   {
//     vec.push_back((float)(*it)->get_value(type,tmp));
//   }
// }


// list<particle_track*>* hash_box::box_to_list() const{
//   // this should be cleaned up
//   list<particle_track*>* tmp = new list<particle_track*>;
  
//   for(vector<particle*>::const_iterator it = contents_.begin();
//       it!=contents_.end(); ++it)
//   {
//     tmp->push_back(static_cast<particle_track*>(*it));
//   }
//   return tmp;
// }

void  hash_box::box_to_list(std::list<particle*>& p_list) const
{
  p_list.clear();
  
  for(vector<particle*>::const_iterator it = contents_.begin();
      it!=contents_.end(); ++it)
  {
    p_list.push_back(*it);
  }
}

hash_box::~hash_box()
{
  if(owns_particles_)
  {
    vector<particle*>::iterator it_end = contents_.end();
    for(vector<particle*>::iterator it = contents_.begin();
	it!=it_end; ++it)
      delete *it;
  }
  
  contents_.clear();
}
