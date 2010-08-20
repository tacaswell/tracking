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
#include "track_list.h"
#include "particle_track.h"
#include "track_shelf.h"
#include "track_box.h"

#include <stdexcept>
using std::runtime_error;

using namespace tracking;
using std::vector;
using std::set;
using std::list;
using std::pair;

const static unsigned int MAX_NET_SIZE = 75;

void track_list::link_next(list<particle_track*>* new_next){
  //tracks->print();

  p_list = store_list;
  store_list = new list<particle_track*>(*new_next);
  n_list = new_next;


  trivial_bonds();
  
  clean_lists();

  while(n_list->size()>0 && p_list->size()>0){
    set_up_sub_ntwrk();
    
    
    //have this function delete pos_link for the particles that get
    //dropped
    link_sub_ntwrk();

    clean_lists();
  }
  //   cout<<endl;


  delete p_list;
  delete n_list;

}


void track_list::link_sub_ntwrk (){
  vector<pair<particle_track*, particle_track*> > best, cur;
  int p_size = p_sub_net.size();
  int n_size = n_sub_net.size();

  int vec_size = (p_size>n_size) ? p_size : n_size;

  best.reserve(vec_size);
  cur.reserve(vec_size);


  
  
  //call the recursive monster
  float min = max_disp_sq*vec_size;
  recur_fnc_pn(&best,&cur,0,min,
	       p_sub_net.begin(),p_sub_net.end());



  link_pairs(best);

  

	       
}




void track_list::link_pairs(vector<pair<particle_track*, particle_track*> >& in){
  //sets the correct forward and backward links for the pairs and deletes the
  //pos_link lists
  particle_track* tmp_prev_particle = NULL;
  particle_track* tmp_next_particle = NULL;

  //deal with particles that have been linked
  for( vector<pair<particle_track*, particle_track*> >::iterator it = in.begin();
       it<in.end(); it++)
    {
      tmp_prev_particle = (*it).first;
      tmp_next_particle = (*it).second;
    
      if(tmp_prev_particle!=NULL){
	if(tmp_next_particle !=NULL)
	{
	  //tracks->add_to_track(tmp_prev_particle->get_track_id(),tmp_next_particle );
	  (tmp_prev_particle->get_track())->push_back(tmp_next_particle);
	  
	}
	
	delete tmp_prev_particle->n_pos_link;
	tmp_prev_particle->n_pos_link=NULL;
	
	p_sub_net.erase(tmp_prev_particle);

      }
      if(tmp_next_particle!=NULL){
	if(tmp_prev_particle == NULL){
	  tracks->add_new_track(tmp_next_particle);
// 	  cout<<"Adding due to subnetwork failure"<<endl;
// 	  tmp_next_particle->print();
	}
	delete tmp_next_particle->p_pos_link;
	tmp_next_particle->p_pos_link=NULL;
	
	n_sub_net.erase(tmp_next_particle);

      }
      
    }
  

  //deals with any previous particles left in the sub network
  // //cout<<p_sub_net.size()<<endl;
  if(p_sub_net.size()!=0){
    for(set<particle_track*>::iterator it = p_sub_net.begin();
	it!=p_sub_net.end(); it++)
      {
	//just drop particle
	delete (*it)->n_pos_link;
	(*it)->n_pos_link=NULL;
      }
  }
  //clear the subnetwork
  p_sub_net.clear();

  //exact same as above, there has to be a cleaner way to write this
  if(n_sub_net.size()!=0){
    //    //cout<<n_sub_net.size()<<endl;
     for(set<particle_track*>::iterator it = n_sub_net.begin();
	it!=n_sub_net.end(); it++)
      {
	//start a new track
	tracks->add_new_track(*it);
	// cout<<"Adding due to particles left in subnetwork"<<endl;
	// (*it)->print();
	delete (*it)->p_pos_link;
	(*it)->p_pos_link=NULL;
	
      }
  }  
  n_sub_net.clear();

}

//takes an iterator over p_sub_net, and looks for links forwards
void track_list::recur_fnc_pn(vector<pair<particle_track*, particle_track*> >* min,
			      vector<pair<particle_track*, particle_track*> >* cur,
			      float disp,float &min_disp,
			      set<particle_track*>::iterator it,
			      const set<particle_track*>::iterator& itend){
  
    

  list<pair<particle_track*, float> >::iterator it2;
  //this is a 'prev' particle
  particle_track* tmp_prev_particle = *it;
  //this is a next particle
  particle_track* tmp_next_particle = NULL;

  bool repeated_particle = false;
  //base case logic
  //beware of tricksy iterators
  if((++it)==itend){
    //    cout<<"base case"<<endl;
    for(it2 = (tmp_prev_particle->n_pos_link)->begin();
	it2!=(tmp_prev_particle->n_pos_link)->end();
	it2++)
      {
	//if this is a new minimum
	if((disp+(*it2).second) < min_disp)
	  {
	    tmp_next_particle = (*it2).first;
	    //check to make sure the particle we want to link to isn't already
	    //linked
	    repeated_particle = false;
	    for(vector<pair<particle_track*, particle_track*> >::iterator it3 = cur->begin();
		it3 < cur->end(); it3++)
	      {
		if((*it3).second == tmp_next_particle){
		  repeated_particle = true;
		  //if we find it once, then we do't need to check the rest
		  //of the vector, so break out of this loop
		  break;
		}
	      }
	    
	    //if this particle is alread mapped to in the current arangement, bail
	    if(repeated_particle)
	      continue;
	  
	    cur->push_back(pair<particle_track*,particle_track*>
			   (tmp_prev_particle,tmp_next_particle));
	    *min = *cur;
	    min_disp = disp + ((*it2).second);
	    cur->pop_back();
	  }
      }
	
    //deal with null connection
    if((disp+ max_disp_sq) <min_disp)
      {
	//	cout<<"base case null link"<<endl;
	cur->push_back(pair<particle_track*,particle_track*>
		       (tmp_prev_particle,NULL));
	*min = *cur;
	min_disp = disp + max_disp_sq;
	cur->pop_back();
      }
    
    
    

      
    return;
  }

  //normal case logic
  //may want to invert some of this logic to make it clearer
  //  cout<<"normal case logic"<<endl;
  for(it2 = (tmp_prev_particle->n_pos_link)->begin();
      it2!=(tmp_prev_particle->n_pos_link)->end();
      it2++)
    {
      
      //if we arn't done, but the running total is higher than the minimum, bail
      if((disp+(*it2).second) > min_disp)
	continue;

      tmp_next_particle = (*it2).first;
      repeated_particle = false;

      for(vector<pair<particle_track*, particle_track*> >::iterator it3 = cur->begin();
	  it3<cur->end(); it3++)
	{
	  if((*it3).second == tmp_next_particle){
	    repeated_particle = true;
	    break;
	  }
	}
      //if this particle is alread mapped to in the current arangement, bail
      if(repeated_particle)
	continue;

      cur->push_back(pair<particle_track*,particle_track*>
		     (tmp_prev_particle,tmp_next_particle));
      
      //note that it got incrimented in that test at the top
      recur_fnc_pn(min,cur,disp+(*it2).second, min_disp, it, itend);

      //remove this try before continueing the loop
      cur->pop_back();
    }
    //deal with null connection
  if((disp+ max_disp_sq) <min_disp)
    {
      //      cout<<"normal case null link"<<endl;
      cur->push_back(pair<particle_track*,particle_track*>
		     (tmp_prev_particle,NULL));
      recur_fnc_pn(min,cur,disp+max_disp_sq, min_disp, it, itend);

      cur->pop_back();
    }
  
  //deal with null connection


  return;

}


//takes an iterator over n_sub_net, and looks for links backwards
void track_list::recur_fnc_np(  vector<pair<particle_track*, particle_track*> >* min,
			     vector<pair<particle_track*, particle_track*> >* cur,
			     float disp,float & min_disp,
			     set<particle_track*>::iterator it,
			     const set<particle_track*>::iterator & itend){

  list<pair<particle_track*, float> >::iterator it2;
  particle_track* tmp_next_particle = *it;
  particle_track* tmp_prev_particle = NULL;
  bool repeated_particle = false;
  //base case logic
  //beware of tricksy iterators
 
  if((it++)==itend){
    for(it2 = (tmp_next_particle->p_pos_link)->begin();
	it2!=(tmp_next_particle->p_pos_link)->end();
	it2++)
      {
	if(disp+(*it2).second < min_disp)
	  {
	    tmp_prev_particle = (*it2).first;
	    repeated_particle = false;
	    for(vector<pair<particle_track*, particle_track*> >::iterator it3 = cur->begin();
		it3<cur->end(); it3++)
	      {

		
		if((*it3).first == tmp_prev_particle){
		  repeated_particle = true;
		  break;
		}
	      }
	    
	    //if this particle is alread mapped to in the current arangement, bail
	    if(repeated_particle)
	      continue;
	  
	    cur->push_back(pair<particle_track*,particle_track*>
			   (tmp_prev_particle,tmp_next_particle));

	    
	    *min = *cur;
	    min_disp = disp + ((*it2).second);
	    cur->pop_back();
	  }
	//deal with null connection
	if(disp+ max_disp_sq <min_disp)
	  {
	    cur->push_back(pair<particle_track*,particle_track*>
			   (NULL,tmp_next_particle));
	    *min = *cur;
	    min_disp = disp + max_disp_sq;
	    cur->pop_back();
	  }
	  
	
      }
    return;
  }

  //normal case logic
  //may want to invert some of this logic to make it clearer
  for(it2 = (tmp_next_particle->p_pos_link)->begin();
      it2!=(tmp_next_particle->p_pos_link)->end();
      it2++)
    {
      
      //if we arn't done, but the running total is higher than the minimum, bail
      if(disp+(*it2).second > min_disp)
	continue;
      tmp_prev_particle = (*it2).first;
      repeated_particle = false;
      for(vector<pair<particle_track*, particle_track*> >::iterator it3 = cur->begin();
	  it3<cur->end(); it3++)
	{
	  if((*it3).first == tmp_prev_particle){
	    repeated_particle = true;
	    break;
	  }
	}
      //if this particle is alread mapped to in the current arangement, bail
      if(repeated_particle)
	continue;
      
      //check this logic too!!
      cur->push_back(pair<particle_track*,particle_track*>
		     (tmp_prev_particle,tmp_next_particle));
      
      recur_fnc_np(min,cur,disp+(*it2).second, min_disp, it, itend);

      cur->pop_back();
    }
    //deal with null connection
  if(disp+ max_disp_sq <min_disp)
    {
      
      cur->push_back(pair<particle_track*,particle_track*>
		     (NULL,tmp_next_particle));
      recur_fnc_np(min,cur,disp+(*it2).second, min_disp, it, itend);

      cur->pop_back();
    }
  
  //deal with null connection
  return;

}

			     

//there has to be a better way to write this
void track_list::clean_lists(){
  particle_track* tmp_particle = NULL;
  for(list<particle_track*>::iterator it = n_list->begin();
      it!=n_list->end();)
    {
      tmp_particle = *it;
      if((tmp_particle->get_prev())!=NULL)
	{
	  delete tmp_particle->p_pos_link;
	  tmp_particle->p_pos_link = NULL;
	  it = erase(n_list,it);
	  continue;
	}
      if( tmp_particle->p_pos_link == NULL)
	{

	  it = erase(n_list,it);

	  continue;
	}
      it++;
    }

  for(list<particle_track*>::iterator it = p_list->begin();
      it!=p_list->end();)
    {
      tmp_particle = *it;
      if((tmp_particle->get_next())!=NULL){
	delete tmp_particle->n_pos_link;
	tmp_particle->n_pos_link = NULL;
	it = erase(p_list,it);
	continue;
      }
      if( tmp_particle->n_pos_link == NULL)
	{
	  it = erase(p_list,it);
	  continue;
	}
      it++;
    }
 

}





void track_list::trivial_bonds(){
  particle_track* tmp_prev_particle= NULL;
  particle_track* tmp_next_particle= NULL;

    
  for(list<particle_track*>::iterator it = n_list->begin();
      it!=n_list->end();it++)
    {
      tmp_next_particle = *it;
      //if a particle.has no possible previous links than it must be a new track
      //as far as the algorithm is concerned
      if((tmp_next_particle->p_pos_link)==NULL){
	  tracks->add_new_track(tmp_next_particle);
//  	  cout<<"Adding due to trivial bond"<<endl;
//  	  tmp_next_particle->print();
      }
      else if( ((tmp_next_particle)->p_pos_link)->size()==1)
	{
	  tmp_prev_particle = ((tmp_next_particle->p_pos_link)->front()).first;
	  if((tmp_prev_particle->n_pos_link)!=NULL && (tmp_prev_particle->n_pos_link)->size()==1){
	    //add to tracks
	    //tracks->add_to_track(tmp_prev_particle->get_track_id(), tmp_next_particle);
	    (tmp_prev_particle->get_track())->push_back(tmp_next_particle);
	  }
	}
      

    }


//  
//  for(list<particle_track*>::iterator it = p_list->begin();
//      it!=p_list->end();it++)
//    {
//      tmp_prev_particle = *it;
//      if(((tmp_prev_particle)->n_pos_link)!=NULL && ((tmp_prev_particle)->n_pos_link)->size()==1)
//	{
//	  tmp_next_particle = ((tmp_prev_particle->n_pos_link)->front()).first;
//	  if((tmp_next_particle->p_pos_link)!=NULL && (tmp_next_particle->p_pos_link)->size()==1){
//	    //add to tracks
//	    tracks->add_to_track(tmp_prev_particle->get_track_id(), tmp_next_particle);
//	    
//	  }
//	}
//      
//
//    }
//  
}

void track_list::set_up_sub_ntwrk(){
  particle_track* tmp_particle= NULL;
  list<particle_track*> tmp_p;
  list<particle_track*> tmp_n;
  pair<set<particle_track*>::iterator, bool> tmp_return;
  list<pair<particle_track*,float> >::iterator it, it2,it_end;
  p_sub_net.clear();
  n_sub_net.clear();


  tmp_particle = p_list->front();
  p_sub_net.insert(tmp_particle);
  it_end = (tmp_particle->n_pos_link)->end();
  
  for(it =  (tmp_particle->n_pos_link)->begin();
      it!=it_end ; it++)
  {
    tmp_n.push_back((*it).first);
  }

  while((tmp_p.size()!=0) ||(tmp_n.size()!=0))
  {
    // sanity check
    if(n_sub_net.size() >MAX_NET_SIZE ||p_sub_net.size() >MAX_NET_SIZE)
    {
      throw runtime_error("subnetwork too big, barfing");
    }
    
    //loop over tmp_p
    while(tmp_p.size()!=0)
    {
      tmp_particle = tmp_p.front();
      tmp_return = p_sub_net.insert(tmp_particle);
      if((tmp_return.second))
      {
	it_end =(tmp_particle->n_pos_link)->end();
	for(it =  (tmp_particle->n_pos_link)->begin();
	    it!= it_end; it++)
	  tmp_n.push_back((*it).first);
      }
      tmp_p.pop_front();
    }

    while(tmp_n.size()!=0)
    {
      tmp_particle = tmp_n.front();
      tmp_return = n_sub_net.insert(tmp_particle);
      if((tmp_return.second))
      {
	it_end = (tmp_particle->p_pos_link)->end();
	for(it =  (tmp_particle->p_pos_link)->begin();
	    it!= it_end; it++)
	  tmp_p.push_back((*it).first);
      }
      tmp_n.pop_front();
    }
  }
//   if(n_sub_net.size() >50 ||p_sub_net.size() >50)
//   {
//     cout<<'=';
//     return;
//   }
 
//   if(n_sub_net.size() >25 ||p_sub_net.size() >25)
//   {
//     cout<<'-';
//     return;
//   }

//   if(n_sub_net.size() >10 ||p_sub_net.size() >10)
//   {
//     cout<<'+';
//     return;
    
//   }
  
}



track_list::track_list(list<particle_track*>* first_list, float i_max_disp, 
		       Track_shelf* in_tracks):
  p_list(NULL), n_list(NULL), store_list(first_list), 
  tracks(in_tracks),max_disp(i_max_disp), max_disp_sq(i_max_disp*i_max_disp)
{
//   bool(*fn_pt)(particle_track*,particle_track*) = part_lt;
//   n_sub_net = 
//     set<particle_track*,bool(*)(particle_track*,particle_track*)>(fn_pt);
//   p_sub_net = 
//     set<particle_track*,bool(*)(particle_track*,particle_track*)>(fn_pt);

// tac 2009-10-19
// took out beacuse with out the bool, this is just a boring default constructor

//   n_sub_net = set<particle_track*>;
//   p_sub_net = set<particle_track*>;

} 


list<particle_track*>::iterator 
track_list::erase(list<particle_track*>* in_list,
		  list<particle_track*>::iterator it)
{
//  
//  
//  
//  
  return in_list->erase(it);
}








track_list::~track_list(){
  //clean up the possible link lists on all elements
  particle_track* tmp_particle = NULL;
  for(list<particle_track*>::iterator it = store_list->begin();
      it!=store_list->end();)
    {
      tmp_particle = *it;
      delete (tmp_particle)->p_pos_link;
      delete (tmp_particle)->n_pos_link;
      (tmp_particle)->p_pos_link = NULL;
      (tmp_particle)->n_pos_link = NULL;
      it = erase(store_list, it);
    }
  
  //deletes the (now empty) list
  delete store_list;
}
  
/* 

void track_list::trivial_bonds(dir_type dir){
  //function pointer stuff, embed this in to a function
  //eventually
  bool(*not_null_part)(particle_track*);
  bool(*not_null_list)(particle_track*);
  set_up_funcptr(dir,not_null_part, not_null_list);
  particle_track* p_tmp;
  for(list<particle_track*>::iterator it = p_list.begin();it!=p_list.end();it++)
    {
      //if a particle 
      if(not_null_part(*it)){
	erase(it);
	continue;
      }
      //if a particle only has one possible partner
      if(((*it)->pos_link)->size()==1){
	//extract a pointer to it's partner to make the rest of this readable
	p_tmp = ((*it)->pos_link)->front();
	//if the partner also only has one possible partner, then this
	//is a trivial bond
	if(  (p_tmp->pos_link)->size()==1){
	  //nonense having to do with false hopes of generality, this
	  //might die in the near future when this gets refactored
	  if(dir==dt_prev){
	    (*it)-> set_next(p_tmp);
	    p_tmp-> set_prev(*it);
	  }
	  else if(){
	    (*it)-> set_prev(p_tmp);
	    p_tmp-> set_next(*it);
	  }
	  
	  //remove this particle from the list
	  erase(it);
	}
      }
      
    }


}






void track_list::set_up_funcptr(dir_type dir,   bool(*not_null_part)(particle_track*),
				bool(*not_null_list)(particle_track*)){
   switch( dir)
    {
    case dt_prev:
      not_null_part = &prev_not_null;
      not_null_list = &next_not_null;
      break;
    case dt_next:
      not_null_part = &next_not_null;
      not_null_list = &prev_not_null;
      break;
    default:
    }
}

void track_list::remove_non_null(dir_type dir){
  //function pointers are kind of creepy to me still
  bool(*not_null)(particle_track*)
  switch( dir)
    {
    case dt_prev:
      not_null = &prev_not_null;
      break;
    case dt_next:
      not_null = &next_not_null;
      break;
    default:
      
    }

  for(list<particle_track*>::iterator it = p_list.begin();it!=p_list.end();it++)
    if(not_null(*it))
      erase(it);
    
  
}

*/
