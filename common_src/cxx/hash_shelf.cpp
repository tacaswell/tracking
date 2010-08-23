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
#include <cmath>


#include "hash_shelf.h"
#include "hash_box.h"
#include "track_box.h"
#include "particle_track.h"

#include <stdexcept>
using std::runtime_error;

using namespace tracking;



using utilities::Tuplef;
using utilities::Tuplei;
using utilities::Null_field;
using utilities::Ll_range_error;
using utilities::Wrapper_out;

using tracking::Corr;


using std::list;
using std::cout;
using std::endl;
using std::vector;
using std::logic_error;


const int max_hash_dim_prod = 100*100*100;


void Hash_shelf::push(particle * p){
  try{
    //cout<<hash_function(p)<<endl;
    
    (hash_.at(hash_function(p)))->push(p);
    int tmp_ind = p->get_ind();
    if(tmp_ind > max_part_indx_)
      max_part_indx_ = tmp_ind;
    
  }
  catch (std::exception& e)    {
    cout << e.what() << endl;
    float tmp;
    
    std::cout<<"hash value: "<<hash_function(p)<<"\t"<<"hash table size "<<hash_.size()<<std::endl;
    std::cout<<p->get_value(utilities::D_YPOS,tmp)<<"\t"<<p->get_value(utilities::D_XPOS,tmp)<<std::endl;
    p->print();
    throw;

  }
  
  ++particle_count_;
//   cout<<"particle: "<<endl;
//   p->print();
//   cout<<"hash_box: "<<hash_function(p)<<endl;
}






Hash_shelf::Hash_shelf(utilities::Tuplef imgsz, 
		       float upb, 
		       int i_frame,
		       bool own_part,
		       float z_offset
		       )
  :md_store_(NULL),
   plane_number_(i_frame), 
   next_(NULL),particle_count_(0),
   max_part_indx_(0),
   hash_dims_(0),
   hash_cum_dim_(0),
   img_dims_(imgsz),
   upb_(upb),
   z_offset_(z_offset)
{  
  priv_init(own_part);
  
}

void Hash_shelf::priv_init(bool own_part)
{
  
  int rank = Tuplef::length_;
  hash_dims_.clear();
  hash_.clear();

  int hash_dim_prod = 1;
  
  for(int j = 0; j<rank;++j)
  {
    hash_dims_[j] = (int)ceil(img_dims_[j]/upb_);
    
    // (((int)(img_dims_[j]))%upb_==0?
    //  ((int)img_dims_[j])/upb_:
    //  (((int)img_dims_[j])/upb_)+1);
    hash_dim_prod*=hash_dims_[j];
    
    
    unsigned int tmp_prod = 1;
    for(int k = 0;k<(j);++k)
    {
      tmp_prod*=hash_dims_[k];
    }
    hash_cum_dim_[j] = tmp_prod;
    
      
  }
  
  if(hash_dim_prod > max_hash_dim_prod)
    throw std::invalid_argument("The hashing size given will make too many hash boxes, dieing ungracefully");
  


  hash_.reserve(hash_dim_prod);
  for(int j = 0; j<hash_dim_prod;j++)
    hash_.push_back(new hash_box(this,j,own_part));
}





void Hash_shelf::shelf_to_list(std::list<particle*> &tmp) const{
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

void Hash_shelf::shelf_to_list(std::list<const particle*> &tmp) const{
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


void Hash_shelf::compute_mean_forward_disp(utilities::Tuplef & cum_disp_in){
  cumulative_disp_ = cum_disp_in;
  mean_forward_disp_.clear();
  int count = 0;
  const particle_track* current_part = NULL;
  for(vector<hash_box*>::iterator cur_box = hash_.begin(); 
      cur_box<hash_.end(); ++cur_box)
  {
    for(vector<particle*>::iterator cur_part = (*cur_box)->begin();
	cur_part!=(*cur_box)->end(); ++cur_part) 
    {
      current_part = static_cast<particle_track*>(*cur_part);
      if(current_part->get_next() != NULL)
      {
	mean_forward_disp_ += (current_part->get_raw_forward_disp());
	++count;
      }
    }
  }
  if(count > 0)
    mean_forward_disp_ /= count;
  
  cum_disp_in += mean_forward_disp_;
  
}

Hash_shelf::~Hash_shelf()
{
  for(vector<hash_box*>::iterator it = hash_.begin(); it<hash_.end(); ++it)
  {
    delete *it;
    *it = NULL;
    
  }
  md_store_ = NULL;
  
}





void Hash_shelf::set_next_shelf(Hash_shelf* next)
{
  if(next_ !=NULL)
  {
    throw runtime_error("the next shelf is already set");
  }
  next_ = next;
}


bool Hash_shelf::step_forwards(int n, const Hash_shelf* & dest)const{

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


// may have trouble with int/float changes
int Hash_shelf::tuple_to_indx(const Tuplei & cord)const
{
  unsigned int indx = 0;
  int length = Tuplei::length_;
  for(int j = 0;j<length;++j)
  {
    if(cord[j]>=hash_dims_[j])
      throw runtime_error("Hash_shelf::tuple_to_indx cord out of hash dimensions");
    
    
    indx += ((unsigned int)cord[j])*hash_cum_dim_[j];
  }
  return indx;
}



Tuplei Hash_shelf::indx_to_tuple(int indx) const
{
  if(!(indx <(int) hash_.size()))
    throw logic_error("index is greater than hash length");
  
  Tuplei cord ;
  for(int j = 0;j<Tuplei::length_;++j)
  {
    cord[j] = (indx/hash_cum_dim_[j])%((int)hash_dims_[j]);
  }
  return Tuplei(cord);
  
}
Tuplei Hash_shelf::range_indx_to_tuple(int indx,const Tuplei& side) const
{
  Tuplei cord ;
  for(int j = 0;j<Tuplei::length_;++j)
  {
    int tmp_prod = 1;
    for(int k = 0;k<(j);++k)
    {
      tmp_prod*=(int)side[k];
    }
    cord[j] = (indx/tmp_prod)%((int)side[j]);
  }
  return Tuplei(cord);
  
}
  

unsigned int Hash_shelf::hash_function(const particle* p) const
{

  Tuplef cur_pos = p->get_position();
  cur_pos/=upb_;
  
  unsigned int indx = 0;
  int length = Tuplef::length_;
  for(int j = 0;j<length;++j)
  {
    if(cur_pos[j]>=hash_dims_[j])
      throw runtime_error("Hash_shelf::tuple_to_indx cord out of hash dimensions");
    
    
    indx += ((unsigned int)cur_pos[j])*hash_cum_dim_[j];
  }
  return indx;
  
// tac 2010-03-25 Changed to copying the code from this function to
// avoid extra casts to and from 

  //return tuple_to_indx(cur_pos);
  

}


void Hash_shelf::test()
{
  cout<<hash_dims_<<endl;
  
  Tuplei base;
#if DIM_COUNT == 2
  Tuplei inc1 = Tuplei(1,0);
  Tuplei inc2 = Tuplei(0,1);
#elif DIM_COUNT == 3
  Tuplei inc1 = Tuplei(1,0,0);
  Tuplei inc2 = Tuplei(0,1,0);
  Tuplei inc3 = Tuplei(0,0,1);
#endif
  
  cout<<tuple_to_indx(base)<<'\t'<<base<<endl;
  for(int j = 0; j<5;++j)
  {
    base+=inc1;
    cout<<base<<'\t'<<tuple_to_indx(base)<<'\t'<<indx_to_tuple(tuple_to_indx(base)) <<endl;
    for(int k= 0; k<5;++k)
    { 
      base+=inc2;
      cout<<base<<'\t'<<tuple_to_indx(base)<<'\t'<<indx_to_tuple(tuple_to_indx(base)) <<endl;

#if DIM_COUNT == 3
      for(int m= 0; m<5;++m)
      { 
	base+=inc3;
	cout<<base<<'\t'<<tuple_to_indx(base)<<'\t'<<indx_to_tuple(tuple_to_indx(base)) <<endl;
      }
      base[2] = 0;
      
#endif
    }
    base[1] = 0;
    
  }

  for(int k = 0; k<200;++k)
  {
    cout<<"-----"<<k<<'\t'<<indx_to_tuple(k)<<"-----"<<endl;
    vector<const particle*> yar;
    
    get_region(k,yar,1);
  }
  
  

}



void Hash_shelf::get_region(int n,vector<const particle*> & out_vector,int range) const
{
  bool testing = false;
  
  Tuplei center = indx_to_tuple(n);
  Tuplei bottom_corner, top_corner,region_sides;
  make_region_corners(center,bottom_corner,top_corner,region_sides,range);
  int region_sz = (int)region_sides.prod();
  for(int j = 0;j<region_sz;++j)
  {
    Tuplei tmp = range_indx_to_tuple(j,region_sides);
    tmp += bottom_corner;
    if(testing)
       cout<<j <<'\t'<<tmp<<endl;
    
    int tmp_indx = tuple_to_indx(tmp);
    hash_box* cur_box = (hash_.at(tmp_indx));
    vector<particle *>::const_iterator it_end = cur_box->end();
    for(vector<particle *>::const_iterator it = cur_box->begin();
	it!=it_end;++it)
    {
      out_vector.push_back(*it);
    }
    
  }
}


void Hash_shelf::get_region(int n,list< particle*> & out_vector,int range) const
{
  Tuplei center = indx_to_tuple(n);
  Tuplei bottom_corner, top_corner,region_sides;
  make_region_corners(center,bottom_corner,top_corner,region_sides,range);
  int region_sz = (int)region_sides.prod();
  
  
  for(int j = 0;j<region_sz;++j)
  {
    Tuplei tmp = range_indx_to_tuple(j,region_sides);
    tmp += bottom_corner;
    
    int tmp_indx = tuple_to_indx(tmp);
    hash_box* cur_box = (hash_.at(tmp_indx));
    vector<particle *>::const_iterator it_end = cur_box->end();
    for(vector<particle *>::const_iterator it = cur_box->begin();
	it!=it_end;++it)
    {
      out_vector.push_back(*it);
    }
    
  }
}


void Hash_shelf::get_region_px( int n, std::vector<const particle*> & out ,
		    float range_in_units) const
{
  get_region(n,out,(int)ceil(range_in_units/upb_));

}

void Hash_shelf::print()const
{
  cout<<"hash dimensions: "<<hash_dims_<<endl;
  cout<<"contains: "<<particle_count_<<endl;

  // for(int j = 0; j<hash_dims_[0] ;++j)
  // {
  //   for(int k = 0; k<hash_dims_[1];++k)
  //   {
  //     cout<<hash_[tuple_to_indx(Tuple(j,k))]->get_size()<<'\t';
  //   }
  //   cout<<endl;
  // }
  
  
}

void Hash_shelf::make_region_corners(const Tuplei & center, 
				     Tuplei & bottom_corner,
				     Tuplei & top_corner,
				     Tuplei & region_sides,
				     const int range)const
{
  // check top and bottom corners are in range
  for(int j = 0;j<Tuplei::length_;++j)
  {
    bottom_corner[j] = (((center[j]-range)>=0)?(center[j]-range):0);
    top_corner[j] = (((center[j]+range)<hash_dims_[j])?(center[j]+range):hash_dims_[j]-1);
  }
  region_sides = top_corner - bottom_corner +1;
  
#ifdef TESTING
  {
    cout<<"bottom: "<<bottom_corner<<endl;
    cout<<"top: "<<top_corner<<endl;
    cout<<"region_sides: "<<region_sides<<endl;
    cout<<"region_sz: "<<region_sides.prod()<<endl;
  }
#endif

}


void Hash_shelf::get_region(particle* n,hash_box * out_box,int range) const
{
  Tuplei center = indx_to_tuple(hash_function(n));
  Tuplei bottom_corner, top_corner,region_sides;
  make_region_corners(center,bottom_corner,top_corner,region_sides,range);
  int region_sz = (int)region_sides.prod();
  
  
  for(int j = 0;j<region_sz;++j)
  {
    Tuplei tmp = range_indx_to_tuple(j,region_sides);
    tmp += bottom_corner;
    
    int tmp_indx = tuple_to_indx(tmp);
    hash_box* cur_box = (hash_.at(tmp_indx));
    out_box->append(cur_box);
    
  }
}


list<particle_track*> * Hash_shelf::shelf_to_list() const{
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
