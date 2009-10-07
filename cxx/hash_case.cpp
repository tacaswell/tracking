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
#include "hash_case.h"
#include <stdexcept> // out_of_range exception
#include <iostream>
#include <vector>
#include "tuple.h"
#include "array.h"
#include "cell.h"

#include "wrapper_o.h"

#include "corr.h"

using namespace tracking;


using utilities::Tuple;

using utilities::Wrapper_out;

using std::list;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

void hash_case::print() const{
  for(unsigned int j = 0; j<h_case_.size();j++){
    h_case_.at(j)->print();
    cout<<endl;
  }
}


hash_case::hash_case()
  :inited(false){

}


hash_case::~hash_case(){
  for(unsigned int j = 0; j<h_case_.size(); j++){
    delete h_case_.at(j);
  }
  std::cout<<"hc dead"<<std::endl;
}

void hash_case::rehash(unsigned int ppb){
  for(vector<hash_shelf*>::iterator it = h_case_.begin();
      it<h_case_.end(); ++it)
    (*it)->rehash(ppb);
}


bool lt_pair_tac(const pair<particle_track*, float> &  lh, const pair<particle_track*, float> & rh){
    return lh.second<rh.second;
  }

void hash_case::fill_pos_link_next(list<particle_track*>* tlist, 
			vector<hash_shelf*>::iterator in_it, float max_disp)
{
  hash_box tmp_box;
  float distsq;
  list<particle_track*>* tmp_list = NULL;
  particle_track* tmp_particle1 = NULL;
  particle_track* tmp_particle2 = NULL;
  
  //square the maximum dispalcement to save having to take square roots later
  max_disp = max_disp*max_disp;

  
  //loop over partciles in handed in list
  for(list<particle_track*>::iterator it = tlist->begin();
            it != tlist->end(); it++)
    {
    
      tmp_particle1 = (*it);
      tmp_box.clear();
      
      (*in_it)->get_region(*it,&tmp_box, 1);

      //      cout<<"box size: "<<tmp_box.get_size()<<endl;
      
      //allocates a list
      tmp_list = tmp_box.box_to_list();
      

      //loop over the list to be added to the current particle to add
      //the the current particle as a previous to each of them and
      //remove the ones that are too far away
      for(list<particle_track*>::iterator it2 = tmp_list->begin();
	  it2!=tmp_list->end();it2++){

	tmp_particle2 = (*it2);
	distsq = (tmp_particle1->distancesq(tmp_particle2));
	//if the particles are with in the maximum dispalacement of eachother
	if(distsq<max_disp){
	  
	  //make sure the lists exist
	  if(tmp_particle2->p_pos_link==NULL){
	    tmp_particle2->p_pos_link = new list<pair<particle_track*,float> >;
	  }
	  if(tmp_particle1->n_pos_link==NULL){
	    tmp_particle1->n_pos_link = new list<pair<particle_track*,float> >;
	  }
	 
	  //add pairing to list
	  (tmp_particle2->p_pos_link)->
	    push_back(pair<particle_track*, float>(tmp_particle1,distsq));
	  (tmp_particle1->n_pos_link)->
	    push_back(pair<particle_track*, float>(tmp_particle2,distsq));
	  
	}
      }

      
      if(tmp_particle1->n_pos_link!=NULL)
	(tmp_particle1->n_pos_link)->sort(lt_pair_tac);
      //deletes the list
      delete tmp_list;
      tmp_list = NULL;

    }
  //  cout<<"finished loops"<<endl;
}

void hash_case:: compute_mean_disp(){
  Tuple cum_disp;
  for(vector<hash_shelf*>::iterator it = h_case_.begin();
      it<h_case_.end(); ++it){
    (*it)->compute_mean_forward_disp(cum_disp);  
  }
}

void hash_case::get_mean_disp(utilities::Array & mean_disp_array, int start){
  int tmp = start + mean_disp_array.get_row_num();
  tmp = (tmp > h_case_.size())? h_case_.size():tmp;
  mean_disp_array.clear();
  for(int j = start; j < tmp;++j){
    mean_disp_array.push(h_case_[j]->get_mean_forward_disp());
  }
}


void hash_case::get_cum_disp(utilities::Array & cum_disp_array, int start){
  int tmp = start + cum_disp_array.get_row_num();
  tmp = (tmp > h_case_.size())? h_case_.size():tmp;
  cum_disp_array.clear();
  for(int j = start; j < tmp;++j){
    cum_disp_array.push(h_case_[j]->get_cum_forward_disp());
  }
}


void hash_case::nearest_neighbor_array(utilities::Cell & pos_cell,
				       utilities::Cell & nn_cell, float range)const
{
  for(vector<hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
//     cout<<"here!! "<<h_case_.size()<<endl;
    
    utilities::Array nn_array(1);
    utilities::Array pos_array(1);

    (*shelf_it)->nearest_neighbor_array(pos_array,nn_array,range);
    nn_cell.add_array(nn_array);
    pos_cell.add_array(pos_array);
    
  }
  

}



void hash_case::next_nearest_neighbor_array(utilities::Cell & pos_cell,
				       utilities::Cell & nn_cell ,
				       utilities::Cell & nnn_cell )const
{
  int j = 0;
  
  for(vector<hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
    cout<<"here!! "<<j++<<endl;
    
    utilities::Array nn_array(1);
    utilities::Array nnn_array(1);
    utilities::Array pos_array(1);

    (*shelf_it)->next_nearest_neighbor_array(pos_array,nn_array,nnn_array);
    nn_cell.add_array(nn_array);
    nnn_cell.add_array(nnn_array);
    pos_cell.add_array(pos_array);
    
  }
  

}

void hash_case::gofr2D(float max_d, utilities::Histogram2D& gofr2 ) const
{
  
  for(vector<hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
    (*shelf_it)->gofr2D(max_d,gofr2);
  }
}


