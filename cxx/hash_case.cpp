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
using namespace tracking;
using std::list;

void hash_case::print(){
  for(unsigned int j = 0; j<h_case.size();j++){
    h_case.at(j)->print();
    cout<<endl;
  }
}




hash_case::~hash_case(){
  for(unsigned int j = 0; j<h_case.size(); j++){
    delete h_case.at(j);
  }
}

void hash_case::rehash(unsigned int ppb){
  for(vector<hash_shelf*>::iterator it = h_case.begin();
      it<h_case.end(); it++)
    (*it)->rehash(ppb);
}

void hash_case::link(double max_range, track_shelf& tracks){

  //doing this rehash takes a long time for large data sets, better to just
  //get it right begin with
  //this->rehash((int)(1.2*max_range));

  //  cout<<"rehashed"<<endl;

  vector<hash_shelf*>::iterator it = h_case.begin();

  //generate first list
  list<particle_track*>* t_list = (*it)->shelf_to_list();

  //  cout<<"generated list 1"<<endl;
  //fill first track pos_link_next
  fill_pos_link_next(t_list,++it,max_range);
  //  cout<<"get links "<<endl;

  for(list<particle_track*>::iterator it2 = t_list ->begin();
      it2!=t_list->end(); it2++)
    tracks.add_new_track(*it2);
  
  //  cout<<"populated initial tracks"<<endl;
  
  //  tracks.print();
  
  //make local track_list object
  track_list tracking(t_list, max_range,&tracks);
  
  int stupid_counter = 0;
  //cout<<"here"<<endl;
  //loop over shelves
  while( it<(h_case.end()-1))
  {
    ++stupid_counter;
    if(stupid_counter%50==0)
      cout<<stupid_counter<<endl;;
    //generate next list
    t_list = (*it)->shelf_to_list();
    //    cout<<": "<<t_list->size()<<endl;
    
    //fill next list's pos_link_next
    fill_pos_link_next(t_list,++it,max_range);
    


    //shove in to track_list
    tracking.link_next(t_list);
  
  }
  //make last list
  t_list = (*it)->shelf_to_list();
  tracking.link_next(t_list);
  cout<<"here"<<endl;
}

bool lt_pair_tac(const pair<particle_track*, double> &  lh, const pair<particle_track*, double> & rh){
    return lh.second<rh.second;
  }

void hash_case::fill_pos_link_next(list<particle_track*>* tlist, 
			vector<hash_shelf*>::iterator in_it, double max_disp)
{
  hash_box tmp_box;
  double distsq;
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
	    tmp_particle2->p_pos_link = new list<pair<particle_track*,double> >;
	  }
	  if(tmp_particle1->n_pos_link==NULL){
	    tmp_particle1->n_pos_link = new list<pair<particle_track*,double> >;
	  }
	 
	  //add pairing to list
	  (tmp_particle2->p_pos_link)->
	    push_back(pair<particle_track*, double>(tmp_particle1,distsq));
	  (tmp_particle1->n_pos_link)->
	    push_back(pair<particle_track*, double>(tmp_particle2,distsq));
	  
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





