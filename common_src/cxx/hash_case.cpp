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
#include "hash_shelf.h"

#include <stdexcept> // out_of_range exception
#include <iostream>
#include <vector>

#include "master_box_t.h"

#include "particle_track.h"
#include "array.h"
#include "cell.h"

#include "wrapper_o.h"

#include "corr.h"

using namespace tracking;


using utilities::Tuplei;
using utilities::Tuplef;

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


// hash_case::hash_case()
//   :inited(false){

// }


hash_case::hash_case(Master_box & mb,const utilities::Tuplef & dims, 
		     float ppb, int frames):inited(false){
  init(mb,dims,ppb,frames);

}
  



void hash_case::init(Master_box & mb,const utilities::Tuplef & dims, 
		     float ppb, int frames){

  if(inited){
    std::cout<<"can't re init"<<std::endl;
    return;
  }
    

  
  // mb.append_to_data_types(utilities::D_NEXT);
  // mb.append_to_data_types(utilities::D_PREV);
  
  
  h_case_.resize(frames);
  h_case_.at(0) = new Hash_shelf(dims, ppb,0);
  for(unsigned int j = 1; j<h_case_.size(); ++j){
    h_case_[j] = new Hash_shelf(dims, ppb,j);
    h_case_[j-1]->set_next_shelf(h_case_[j]);
    h_case_[j]->md_store_ = mb.get_Md_store(j);
    
  }

  particle *p;
  int max_sz = mb.size();
  

  int current_frame =-1;
  int current_count = 0;
  


  for( int j = 0; j<max_sz; ++j){
    p = mb.get_particle(j);
    try{
      int cf;
      (p->get_value(utilities::D_FRAME,cf));
      if(cf != current_frame)
      {
	//	std::cout<<"frame "<<current_frame<<": "<<current_count<<std::endl;
	current_frame = cf;
	current_count = 1;
      }
      else
      {
	++current_count;
      }
      
      h_case_.at(cf)->push(p);
    }
    catch(const char * e)
    {
      std::cout<<e<<std::endl;
      
      int yar;
      (int)p->get_value(utilities::D_FRAME,yar);
      std::cout<<"trying to put in to shelf: "<<yar<<std::endl;
      return;
    }
    
    catch(...){
      
      int yar ;
      (int)p->get_value(utilities::D_FRAME,yar);
      std::cout<<"trying to put in to shelf: "<<yar<<std::endl;
      return;
    }
  
  }
  //  std::cout<<"frame "<<current_frame<<": "<<current_count<<std::endl;

  inited = true;
}



hash_case::~hash_case(){
  for(unsigned int j = 0; j<h_case_.size(); j++){
    delete h_case_.at(j);
  }
  std::cout<<"hc dead"<<std::endl;
}

void hash_case::rehash(float ppb){
  for(vector<Hash_shelf*>::iterator it = h_case_.begin();
      it<h_case_.end(); ++it)
    (*it)->rehash(ppb);
}


void hash_case:: compute_mean_disp(){
  Tuplef cum_disp;
  for(vector<Hash_shelf*>::iterator it = h_case_.begin();
      it<h_case_.end(); ++it){
    (*it)->compute_mean_forward_disp(cum_disp);  
  }
}

void hash_case::get_mean_disp(utilities::Array & mean_disp_array, int start){
  unsigned int tmp = start + mean_disp_array.get_row_num();
  tmp = (tmp > h_case_.size())? h_case_.size():tmp;
  mean_disp_array.clear();
  for(unsigned int j = start; j < tmp;++j){
    mean_disp_array.push(h_case_[j]->get_mean_forward_disp());
  }
}


void hash_case::get_cum_disp(utilities::Array & cum_disp_array, int start){
  unsigned int tmp = start + cum_disp_array.get_row_num();
  tmp = (tmp > h_case_.size())? h_case_.size():tmp;
  cum_disp_array.clear();
  for(unsigned int j = start; j < tmp;++j){
    cum_disp_array.push(h_case_[j]->get_cum_forward_disp());
  }
}


// void hash_case::nearest_neighbor_array(utilities::Cell & pos_cell,
// 				       utilities::Cell & nn_cell, float range)const
// {
//   for(vector<Hash_shelf*>::const_iterator shelf_it = h_case_.begin();
//       shelf_it!= h_case_.end();++shelf_it)
//   {
// //     cout<<"here!! "<<h_case_.size()<<endl;
    
//     utilities::Array nn_array(1);
//     utilities::Array pos_array(1);

//     (*shelf_it)->nearest_neighbor_array(pos_array,nn_array,range);
//     nn_cell.add_array(nn_array);
//     pos_cell.add_array(pos_array);
    
//   }
  

// }



// void hash_case::next_nearest_neighbor_array(utilities::Cell & pos_cell,
// 				       utilities::Cell & nn_cell ,
// 				       utilities::Cell & nnn_cell )const
// {
//   int j = 0;
  
//   for(vector<Hash_shelf*>::const_iterator shelf_it = h_case_.begin();
//       shelf_it!= h_case_.end();++shelf_it)
//   {
//     cout<<"here!! "<<j++<<endl;
    
//     utilities::Array nn_array(1);
//     utilities::Array nnn_array(1);
//     utilities::Array pos_array(1);

//     (*shelf_it)->next_nearest_neighbor_array(pos_array,nn_array,nnn_array);
//     nn_cell.add_array(nn_array);
//     nnn_cell.add_array(nnn_array);
//     pos_cell.add_array(pos_array);
    
//   }
  

// }

// void hash_case::gofr2D(float max_d, utilities::Histogram2D& gofr2 ) const
// {
  
//   for(vector<Hash_shelf*>::const_iterator shelf_it = h_case_.begin();
//       shelf_it!= h_case_.end();++shelf_it)
//   {
//     (*shelf_it)->gofr2D(max_d,gofr2);
//   }
// }


