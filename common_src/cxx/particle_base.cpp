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
#include "particle_base.h"
#include "particle_track.h"
#include "wrapper_i.h"

#include <algorithm>
#include <cmath>
#include <list>
#include <stdexcept>
using std::runtime_error;

using namespace tracking;
 
using std::vector;
using std::set;
using std::list;
using std::cout;
using std::endl;
using std::complex;

using utilities::Tuplef;
using utilities::Wrapper_in;
//int particle_base
float particle_base::max_neighborhood_range_ = 0;

// static initialization
const Wrapper_in* particle_base::wrapper_in_ = NULL;



// constructors 
particle_base::particle_base( int i_ind,int frame):ind_(i_ind),frame_(frame){
  priv_init();
  fill_position();
  
}

// tac 2009-07-17
// added
particle_base::particle_base( int i_ind,Tuplef pos,int frame)
  :ind_(i_ind),position_(pos),frame_(frame){
  priv_init();
}



// private functions

// tac 2009-07-17
// added to streamline multiple constructors
void particle_base::priv_init()
{
  if(wrapper_in_ ==NULL)
    throw runtime_error("wrapper_in_ not initialized");
  
}



void particle_base::fill_position(){
  wrapper_in_->get_value(position_[0],ind_,utilities::D_XPOS,frame_);
  wrapper_in_->get_value(position_[1],ind_,utilities::D_YPOS,frame_);
#if DIM_COUNT == 3
  wrapper_in_->get_value(position_[2],ind_,utilities::D_ZPOS,frame_);
#endif
}






// public functions

float particle_base::distancesq(const particle_base* part_in)const
{
  //return (position_ - (part_in->position_ )).magnitude_sqr();
  return position_.dist_sqr(part_in->position_ );
}





float particle_base::get_value(utilities::D_TYPE type,float & data_io ) const
{
  if(v_type(type) != utilities::V_FLOAT)
    throw runtime_error("particle_base: wrong V_TYPE");
  // add special cases for stuff the particle knows about
  wrapper_in_->get_value(data_io,ind_,type,frame_);
  return data_io;
}



int particle_base::get_value(utilities::D_TYPE type,int & data_io  ) const
{
  if(v_type(type) != utilities::V_INT)
    throw runtime_error("particle_base: wrong V_TYPE");
 
  switch(type)
  {
  case utilities::D_FRAME:
    data_io = frame_;
    break;
  case utilities::D_INDEX:
    data_io = ind_;
    break;
  case utilities::D_N_SIZE:
    data_io = neighborhood_.size();
    break;
  default:
    wrapper_in_->get_value(data_io,ind_,type,frame_);
  }
  
  return data_io;
}

complex<float> particle_base::get_value(utilities::D_TYPE type,complex<float>&data_io) const
{
  if(v_type(type) != utilities::V_COMPLEX)
    throw runtime_error("particle_base: wrong V_TYPE");

  switch(type)
  {
  case utilities::D_S_ORDER_PARAMETER:
    data_io = s_order_parameter_;
    break;
    
  default:
    wrapper_in_->get_value(data_io,ind_,type,frame_);
  }
  
  return data_io;
}




float particle_base::get_r(const utilities::Tuplef & origin) const{
  return (position_ - origin).magnitude();
}

float particle_base::get_theta(const utilities::Tuplef & origin) const{
  Tuplef tmp =  (position_ - origin);
  return atan2(tmp[1], tmp[0]);
}

void particle_base::print()const{
  //float tmp;
  cout<<"particle "<<frame_<<','<<ind_<<") at "<<position_;
  
  // cout<<position_<<"\t\t";
//   // float tmp;
// //   wrapper_in_->get_value(tmp,ind_,utilities::D_XPOS,frame_);
// //   cout<<'['<<tmp<<',';
// //   wrapper_in_->get_value(tmp,ind_,utilities::D_YPOS,frame_);
// //   cout<<tmp<<']';
//   cout<<neighborhood_.size()<<"\t";
//   cout<<s_order_parameter_<<"\t";
//   cout<<abs(s_order_parameter_);
  cout<<endl;
}

void particle_base::intialize_wrapper_in(const Wrapper_in* in){
  if(wrapper_in_ !=NULL)
    throw runtime_error("Wrapper in already initialized");
  wrapper_in_ = in;
}


void particle_base::clear_wrapper_in()
{
  wrapper_in_ = NULL;
}




bool particle_base::lthan(const particle_base* a,const particle_base* b)const
{
  if(distancesq(a)<distancesq(b))
  {
    return true;
  }
  else
  {
    return false;
  }
}

struct mysortobj
{
  const particle_base* tmp;
  mysortobj(const particle_base* in):
    tmp(in)
  {
  }
  bool operator()(const tracking::particle_base* a,const tracking::particle_base* b)
  {
    if(tmp->distancesq(a)< tmp->distancesq(b))
    {
      return true;
    }
    else
    {
      return false;
    } 
  }
};

void particle_base::sort_neighborhood()
{
  mysortobj lthan_obj(this);
  sort(neighborhood_.begin(),neighborhood_.end(),lthan_obj);
}

bool particle_base::no_neighborhood_repeats() const
{
  vector<const particle*>::const_iterator myend = neighborhood_.end();
  
  for(vector<const particle*>::const_iterator outer = neighborhood_.begin(); outer!= myend;++outer)
  {
    vector<const particle*>::const_iterator inner = outer;
    ++inner;
    for(; inner!= myend;++inner)
    {
      if((*inner)==(*outer))
      {
	return false;
      }
    }
  }
  return true;
}

bool particle_base::add_to_neighborhood(const particle* in)
{
  if(in->frame_ != frame_)
  {
    return false;
  }
  if(in ==this)
    return false;
  
  if(distancesq(in)<(max_neighborhood_range_*max_neighborhood_range_))
  {
    neighborhood_.push_back(in);
    return true;
  }
  return false;
  
}

void particle_base::fill_phi_6()
{
  try
  {
    s_order_parameter_ = compute_phi_6();
  }
  catch(const char * er)
  {
    std::cerr<<er<<endl;
    print();
  }
  
}


complex<float> particle_base::compute_phi_6()const 
{
  complex<float> phi6(0,0);
  if(neighborhood_.size() !=0)
  {
    
    complex<float> i(0,1);
  
    vector<const particle*>::const_iterator myend = neighborhood_.end();
  
    for(vector<const particle*>::const_iterator cur_nei = neighborhood_.begin();
	cur_nei!=myend;++cur_nei)
    {
      Tuplef tmp = (*cur_nei)->get_position()-position_;
      // angle made with the y-axis (I think, doesn't really matter for this
      float theta6 = atan2(tmp[0],tmp[1])*6;
      phi6 += exp(i*theta6);
    }
    phi6/=(float)neighborhood_.size();
  }
  
  
  return phi6;
}

template <class T>
T particle_base::get_wrapper_value(utilities::D_TYPE type,T & val) const
{
  wrapper_in_->get_value(val,ind_,type,frame_);
  return val;
}

template int particle_base::get_wrapper_value(utilities::D_TYPE type,int & val) const;
//template unsigned int particle_base::get_wrapper_value(utilities::D_TYPE type,unsigned int & val) const;
  

const utilities::Tuplef particle_base::get_disp(const particle_base* part_in)const
{
  
  if(part_in==NULL)
    throw runtime_error("");
  
  return part_in->position_ - position_;
  
}
