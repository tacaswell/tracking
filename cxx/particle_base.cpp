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
#include "particle.h"
#include <cmath>

using namespace tracking;
 
using std::vector;
using std::set;
using std::list;
using std::cout;
using std::endl;

using utilities::Tuple;
int particle_base::running_total_ = 0;

// static initialization
wrapper_o_base* particle_base::wrapper_out_ = NULL;
const wrapper_i_base* particle_base::wrapper_in_ = NULL;
std::set<wrapper::p_vals>* particle_base::data_types_ = NULL;


// constructors 
particle_base::particle_base( int i_ind):frame_(0){
  priv_init(i_ind);
  fill_position();
  
}

// tac 2009-07-17
// added
particle_base::particle_base( int i_ind,Tuple pos,int frame)
 :position_(pos),frame_(frame){
  priv_init(i_ind);
}



// private functions

// tac 2009-07-17
// added to streamline multiple constructors
void particle_base::priv_init(int i_ind)
{
  if(wrapper_in_ ==NULL)
    throw "wrapper_in_ not initialized";

  if(data_types_ ==NULL)
    throw "data_types_ not initialized";
  
  ind_ = i_ind;
  unq_id_ = running_total_++;
}



void particle_base::fill_position(){
  position_[0] = get_value(wrapper::d_xpos);
  position_[1] = get_value(wrapper::d_ypos);
}



// public functions

double particle_base::distancesq(const particle_base* part_in)const{

  
  //  return (position_ - (part_in->get_position())).magnitude_sqr();
  return (position_ - (part_in->position_ )).magnitude_sqr();

  //   double X =get_value(wrapper::d_xpos) - part_in->get_value(wrapper::d_xpos);
  //   double Y =get_value(wrapper::d_ypos) - part_in->get_value(wrapper::d_ypos);
  //   //  double Z =get_value(wrapper::d_zpos) - part_in->get_value(wrapper::d_zpos);
  //   return X*X + Y*Y ;//+ Z*Z;
}



void particle_base::set_particle() const{
  if(wrapper_out_ ==NULL)
    throw "wrapper_out_ not initialized";

  wrapper::p_vals tmp_type;
  map<wrapper::p_vals,int> * map_tmp = wrapper_out_->get_map_ptr();
  wrapper_out_->start_new_particle();


   for(map<wrapper::p_vals,int>::const_iterator it = map_tmp->begin();
       it!=map_tmp->end(); it++){
     tmp_type = (*it).first;
     
     
     wrapper_out_->set_new_value(tmp_type, get_value(tmp_type));
   }
  wrapper_out_->end_new_particle();
  
}


double particle_base::get_value(wrapper::p_vals type) const{
  //add check to make sure that the particle know about this
  //type
  if(type == wrapper::d_unqid)
    return (double)unq_id_;
  
  return wrapper_in_->get_value(ind_, type);
}

double particle_base::get_r(const utilities::Tuple & origin) const{
  return (position_ - origin).magnitude();
}

double particle_base::get_theta(const utilities::Tuple & origin) const{
  Tuple tmp =  (position_ - origin);
  return atan2(tmp[1], tmp[0]);
}

void particle_base::print()const{
  //    cout<<ind<<endl;
  cout<<unq_id_<<"\t";
  wrapper_in_->print(ind_);
}

void particle_base::intialize_wrapper_in(const wrapper_i_base* in){
  if(wrapper_in_ !=NULL)
    throw "Wrapper in already initialized";
  wrapper_in_ = in;
}


void particle_base::intialize_wrapper_out(wrapper_o_base* out){
  if(wrapper_out_ !=NULL)
    throw "Wrapper out already initialized";
  wrapper_out_ = out;
}


void particle_base::intialize_data_types(std::set<wrapper::p_vals>*  data_types){
  if(data_types_ !=NULL)
    throw "data types already initialized";
  data_types_ = data_types;
}
