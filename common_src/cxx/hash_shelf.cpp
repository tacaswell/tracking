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

#include <exception>
#include <stdexcept> // out_of_range exception
#include "exception.h"
#include <cmath>
#include<iostream>
#include <vector>

#include "hash_shelf.h"
#include "hash_box.h"
#include "track_box.h"
#include "coarse_grain_array.h"
#include "counted_vector.h"
#include "array.h"
#include "wrapper_o.h"

#include "master_box_t.h"
#include "particle_base.h"
#include "particle_track.h"
#include "pair.h"
#include "triple.h"

#include "corr.h"

using namespace tracking;


using utilities::Coarse_grain_array;
using utilities::Tuple;
using utilities::Null_field;
using utilities::Ll_range_error;
using utilities::Wrapper_out;

using tracking::Corr;


using std::list;
using std::cout;
using std::endl;
using std::vector;



unsigned int hash_shelf::hash_function(const particle* p) const
{
  utilities::Tuple cur_pos = p->get_position();
  
  return (unsigned int)
    (((unsigned int)cur_pos[1]/ppb_)*hash_dims_[0] + cur_pos[0]/ppb_);
  //   return (unsigned int)
  //     (((unsigned int)p->get_value(wrapper::D_YPOS)/ppb)*hash_dims[0] +
  //        p->get_value(wrapper::D_XPOS)/ppb_);
}

hash_shelf::hash_shelf(master_box_t& mb, int imsz1, 
		       int imsz2, unsigned int PPB): ppb_(PPB)
{
  img_dims_[0] = imsz1;
  img_dims_[1] = imsz2;
  std::cout<<img_dims_[0]<<std::endl
	   <<img_dims_[1]<<std::endl;
  init2();
  for(unsigned int j = 0; j<mb.size();j++)
  {
    particle* p = mb.get_particle(j);
    push(p);
  }
}


void hash_shelf::push(particle * p){
  try{
    (hash_.at(hash_function(p)))->push(p);
  }
  catch (std::exception& e)    {
    cout << e.what() << endl;
    float tmp;
    
    std::cout<<"hash value: "<<hash_function(p)<<"\t"<<"hash table size "<<hash_.size()<<std::endl;
    std::cout<<p->get_value(utilities::D_YPOS,tmp)<<"\t"<<p->get_value(utilities::D_XPOS,tmp)<<std::endl;
    p->print();
    throw;

  }
  catch(...)
  {
    std::cout<<"the problem is here"<<std::endl;

    return;
  }    
  
  ++particle_count_;
//   cout<<"particle: "<<endl;
//   p->print();
//   cout<<"hash_box: "<<hash_function(p)<<endl;
}






hash_shelf::hash_shelf(unsigned int imsz1, 
		       unsigned int imsz2, unsigned int PPB,
		       int i_frame):  ppb_(PPB), plane_number_(i_frame),
				      next_(NULL),particle_count_(0)
{
  img_dims_[0] = (imsz1);
  img_dims_[1] = (imsz2);
  init2();
}

hash_shelf::hash_shelf(utilities::Tuple imgsz, 
		       unsigned int ippb, 
		       int i_frame):img_dims_(imgsz),  ppb_(ippb),  
				    plane_number_(i_frame), 
				    next_(NULL),particle_count_(0)
{  
  init2();
}

void hash_shelf::init2(){

  int k = Tuple::length_;
  hash_dims_.clear();
  hash_.clear();

  for(int j = 0; j<k;++j)
    {
      hash_dims_[j] = (((int)(img_dims_[j]))%ppb_==0?((int)img_dims_[j])/ppb_:(((int)img_dims_[j])/ppb_)+1);
    }
  // tac 2009-03-11
  // hash_dims.push_back((*it)%ppb_==0?(*it)/ppb_:(1+(*it)/ppb_)+1);


  int tmp_prod = (int) hash_dims_.prod();


  hash_.reserve(tmp_prod);
  for(int j = 0; j<tmp_prod;j++)
    hash_.push_back(new hash_box(this,j));
}


void hash_shelf::print() const{
  // tac 2009-07-22
  // changed print to spew less

  int total = 0;
  
  for(int i=0; i<hash_dims_[0];i++){
    for(int j=0; j<hash_dims_[1];j++){
      cout<<(hash_.at(j*(int)hash_dims_[0] + i))->get_size() <<"\t";
      total += (hash_.at(j*(int)hash_dims_[0] + i))->get_size();
      
    }
    cout<<endl;
  }
  //mean_forward_disp_.print();

  cout<<total<<endl;
}

void hash_shelf::get_region( int n, int m,
			     hash_box* box,int range) const {
  if(n<0||m<0||range<0||box==NULL)
    return;
  int x_bot = (((n-range)>=0)?(n-range):0);
  int x_top = ((n+range)<((int)hash_dims_[0]-1)?(n+range):((int)hash_dims_[0]-1));
  int y_bot = ((m-range)>0?(m-range):0);
  int y_top = ((m+range)<((int)hash_dims_[1]-1)?(m+range):((int)hash_dims_[1]-1));
  //      cout<<"x: "<<n<<"\t"
  //          <<"y: "<<m<<"\t"
  //          <<x_bot<<"\t"
  //          <<x_top<<"\t"
  //          <<y_bot<<"\t"
  //          <<y_top<<"\t"<<endl;
  for( int x = x_bot; x<=x_top;++x)
  {
    for( int y = y_bot; y<=y_top;++y)
    {
      box->append((hash_.at(x+int(hash_dims_[0])*y)));
      //	 box->append((hash_.at(j+int(hash_dims_[0])*k)));
      // 	 cout<<j<<"\t"<<k<<endl;
      // 	 cout<<"appending box at: "<<j+hash_dims_[0]*k<<endl;
    }
  }
  //     cout<<"+++++++++++++++"<<endl;
}


void hash_shelf::get_region( int n, int m,
			     vector<const particle*> & out,int range) const {
  
  if(n<0||m<0||range<0)
    throw "hash_shelf::get_region: nonsensical arugements";
  

  
  int x_bot = (((n-range)>=0)?(n-range):0);
  int x_top = ((n+range)<((int)hash_dims_[0]-1)?(n+range):((int)hash_dims_[0]-1));
  int y_bot = ((m-range)>0?(m-range):0);
  int y_top = ((m+range)<((int)hash_dims_[1]-1)?(m+range):((int)hash_dims_[1]-1));
  
  int nsize= 0;
  for( int x = x_bot; x<=x_top;++x)
    for( int y = y_bot; y<=y_top;++y)
      nsize += (hash_.at(x+int(hash_dims_[0])*y))->get_size();

  out.clear();
  out.reserve(nsize);
  
  for( int x = x_bot; x<=x_top;++x)
  {
    for( int y = y_bot; y<=y_top;++y)
    {
      hash_box* cur_box = (hash_.at(x+int(hash_dims_[0])*y));
      vector<particle *>::const_iterator it_end = cur_box->end();
      for(vector<particle *>::const_iterator it = cur_box->begin();
	  it!=it_end;++it)
      {
	out.push_back(*it);
      }
      
    }
  }
}


void hash_shelf::get_region_px( int n, 
				vector<const particle*> & out ,
				int range_in_px) const
{
  get_region(n,out,ceil(range_in_px/ppb_));
  
}



void hash_shelf::get_region( int n, 
			     vector<const particle*> & out ,
			     int range) const
{
  get_region(n%int(hash_dims_[0]), n/int(hash_dims_[0]), out, range);

}



void hash_shelf::get_region( int n, 
			     hash_box* box,int range) const
{
  get_region(n%int(hash_dims_[0]), n/int(hash_dims_[0]), box, range);

}

void hash_shelf::get_region( particle* n,
			     hash_box* box, int range) const
{
  get_region(hash_function(n),box,range);
  
}

void hash_shelf::get_region( int n,
			     std::list<particle*>& in_list, 
			     int range) const
{
  hash_box tmp;
  get_region(n,&tmp,range);
  tmp.box_to_list(in_list);
}




  /**
     Remakes the hash table using the same particles using  new
     pixel per box count
     @param PPB
     new pixel per box value
   */
void hash_shelf::rehash(unsigned int PPB){
  list<particle*> tmp;
  shelf_to_list(tmp);

  ppb_ = PPB;
  //rebuilds the hash table
  init2();
  list< particle*>::iterator myEnd = tmp.end();
  
  for(list<particle*>::iterator it = tmp.begin(); it!=myEnd; ++it)
      push(*it);
}

list<particle_track*> * hash_shelf::shelf_to_list() const{
  list<particle_track*>* tmp = new list<particle_track*>;
  for( vector<hash_box* >::const_iterator cur_box = hash_.begin(); cur_box<hash_.end(); ++cur_box)
    {
      
      for(vector<particle*>::iterator cur_part = (*cur_box)->begin();
	  cur_part!=(*cur_box)->end(); ++cur_part)
	{
	  tmp->push_back(static_cast<particle_track*>(*cur_part));
	}
    }

  return tmp;
}


void hash_shelf::shelf_to_list(std::list<particle*> &tmp) const{
  tmp.clear();
  for(vector<hash_box*>::const_iterator cur_box = hash_.begin(); cur_box<hash_.end(); ++cur_box)
    {
      
      for(vector<particle*>::iterator cur_part = (*cur_box)->begin();
	  cur_part!=(*cur_box)->end(); ++cur_part)
	{
	  tmp.push_back(*cur_part);
	}
    }
}

void hash_shelf::shelf_to_list(std::list<const particle*> &tmp) const{
  tmp.clear();
  for(vector<hash_box*>::const_iterator cur_box = hash_.begin(); cur_box<hash_.end(); ++cur_box)
    {
      
      for(vector<particle*>::const_iterator cur_part = (*cur_box)->begin();
	  cur_part!=(*cur_box)->end(); ++cur_part)
	{
	  tmp.push_back(*cur_part);
	}
    }
}


void hash_shelf::compute_mean_forward_disp(utilities::Tuple & cum_disp_in){
  cumulative_disp_ = cum_disp_in;
  mean_forward_disp_.clear();
  int count = 0;
  const particle_track* current_part = NULL;
  for(vector<hash_box*>::iterator cur_box = hash_.begin(); 
      cur_box<hash_.end(); ++cur_box){
    for(vector<particle*>::iterator cur_part = (*cur_box)->begin();
	cur_part!=(*cur_box)->end(); ++cur_part) {
      current_part = static_cast<particle_track*>(*cur_part);
      if(current_part->get_next() != NULL){
	mean_forward_disp_ += (current_part->get_raw_forward_disp());
	++count;
      }
      
    }
  }
  if(count > 0)
    mean_forward_disp_ /= count;
  
  cum_disp_in += mean_forward_disp_;
  
}

int hash_shelf::img_area() const
{
  return int(img_dims_.prod());

}

hash_shelf::~hash_shelf()
{
  for(vector<hash_box*>::iterator it = hash_.begin(); it<hash_.end(); ++it)
  {
    delete *it;
    *it = NULL;
    
  }
  
}





void hash_shelf::set_next_shelf(hash_shelf* next)
{
  if(next_ !=NULL)
  {
    throw "the next shelf is already set";
  }
  next_ = next;
}


bool hash_shelf::step_forwards(int n, const hash_shelf* & dest)const{

  if (next_ == NULL)
  {
    dest = NULL;
    return false;
  }

  if(n==1)
  {
    dest = next_;
    return true;
  }
  if(n<1)
  {
    dest = this;
    return true;
  }
  return step_forwards(--n,dest);
  
}

void hash_shelf::nearest_neighbor_array(utilities::Array & pos_array,
					utilities::Array & nn_array, float range)const
{
//   cout<<"particle_count_"<<particle_count_<<endl;
  
  nn_array.resize(particle_count_);
  pos_array.resize(particle_count_);

  
    
  list<particle*> current_box;
  list<particle*> current_region;
  
  for(int j = 0; j<(int)hash_.size(); ++j)
  {
    int buffer = 2;
    hash_[j]->box_to_list(current_box);
    get_region(j,current_region,buffer);
    if(current_box.empty())
    {
      continue;
    }
    
    // tac 2009-07-22
    // infinite loop if no particles 
    while(current_region.size()<=1 && buffer<hash_dims_[0] &&buffer<hash_dims_[1])
    {
      ++buffer;
      get_region(j,current_region,buffer);
    }
    
    for(list<particle*>::const_iterator box_part = current_box.begin();
	box_part != current_box.end();++box_part)
    {
      const particle* box_part_ptr = *box_part;
      const particle* nn_prtcle = current_region.front();
      float min_r= 999999999;
      
      
      for(list<particle*>::const_iterator region_part = ++(current_region.begin());
	  region_part!= current_region.end();++region_part)
      {
	const particle* region_part_ptr = *region_part;
	if(region_part_ptr == box_part_ptr)
	{
	  continue;
	}
	
	float sep_r = box_part_ptr->distancesq(region_part_ptr);
	if (sep_r<min_r)
	{
	  min_r = sep_r;
	  nn_prtcle = region_part_ptr;
	  if(sep_r == 0)
	  {
	    box_part_ptr->print();
	    nn_prtcle->print();
	    
	  }
	  
	}
      }
      nn_array.push(nn_prtcle->get_position() - box_part_ptr->get_position());
      pos_array.push(box_part_ptr->get_position());
    }
  }
}

void hash_shelf::next_nearest_neighbor_array(utilities::Array & pos_array,
					utilities::Array & nn_array,
					utilities::Array & nnn_array)const
{
//   cout<<"particle_count_"<<particle_count_<<endl;

  nn_array.resize(particle_count_);
  nnn_array.resize(particle_count_);
  pos_array.resize(particle_count_);

  
    
  list<particle*> current_box;
  list<particle*> current_region;
  
  for(int j = 0; j<(int)hash_.size(); ++j)
  {
    int buffer = 2;
    hash_[j]->box_to_list(current_box);
    get_region(j,current_region,buffer);
    if(current_box.empty())
    {
      continue;
    }
    
    // tac 2009-07-22
    // infinite loop if no particles 
    while(!(current_region.size()>3) && buffer<hash_dims_[0] &&buffer<hash_dims_[1])
    {
      ++buffer;
      get_region(j,current_region,buffer);
    }
    
    for(list<particle*>::const_iterator box_part = current_box.begin();
	box_part != current_box.end();++box_part)
    {
      const particle* box_part_ptr = *box_part;
      const particle* nn_prtcle = current_region.front();
      const particle* nnn_prtcle = current_region.front();
      float nn_r= 999999999;
      float nnn_r= 999999999;
      
      for(list<particle*>::const_iterator region_part = ++(current_region.begin());
	  region_part!= current_region.end();++region_part)
      {
	const particle* region_part_ptr = *region_part;
	if(region_part_ptr == box_part_ptr)
	{
	  continue;
	}
	
	float sep_r = box_part_ptr->distancesq(region_part_ptr);
	if(sep_r == 0)
	{
	  box_part_ptr->print();
	  nn_prtcle->print();
	  continue;
	  // if the paritcles are on top of each other don't record them
	}
	if(sep_r<nnn_r)
	{
	  
	  if (sep_r<nn_r)
	  {
	    nnn_r = nn_r;
	    nnn_prtcle = nn_prtcle;
	    
	    nn_r = sep_r;
	    nn_prtcle = region_part_ptr;
	  }
	  else
	  {
	    nnn_r = sep_r;
	    nnn_prtcle = region_part_ptr;
	    
	  }
	}
      }
      nn_array.push(nn_prtcle->get_position() - box_part_ptr->get_position());
      nnn_array.push(nnn_prtcle->get_position() - box_part_ptr->get_position());
      pos_array.push(box_part_ptr->get_position());
    }
  }
}


