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

using namespace tracking;
 
using std::vector;
using std::set;
using std::list;
using std::cout;
using std::endl;

int particle_base::running_total = 0;

particle_base::particle_base(wrapper_i_base * i_data, 
			     wrapper_o_base* o_out,
			     int i_ind,  set<wrapper::p_vals>* contents_in){
    wrapper_in = i_data;
    wrapper_out = o_out;
    ind = i_ind;
    data_types = contents_in;
    
    unq_id = running_total++;
  };



double particle_base::distancesq(particle_base* part_in){
  double X =get_value(wrapper::d_xpos) - part_in->get_value(wrapper::d_xpos);
  double Y =get_value(wrapper::d_ypos) - part_in->get_value(wrapper::d_ypos);
  //  double Z =get_value(wrapper::d_zpos) - part_in->get_value(wrapper::d_zpos);
  return X*X + Y*Y ;//+ Z*Z;
}



void particle_base::set_particle(){

  wrapper::p_vals tmp_type;
  map<wrapper::p_vals,int> * map_tmp = wrapper_out->get_map_ptr();
  wrapper_out->start_new_particle();
    
  for(map<wrapper::p_vals,int>::iterator it = map_tmp->begin();
      it!=map_tmp->end(); it++){
    tmp_type = (*it).first;
    wrapper_out->set_new_value(tmp_type, get_value(tmp_type));
  }
  
  wrapper_out->end_new_particle();
  
}


  double particle_base::get_value(wrapper::p_vals type){
    //add check to make sure that the particle know about this
    //type
    if(type == wrapper::d_unqid)
      return (double)unq_id;

    return wrapper_in->get_value(ind, type);
  }

  void particle_base::print(){
    //    cout<<ind<<endl;
    cout<<unq_id<<"\t";
    wrapper_in->print(ind);
  }
