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
#include "master_box.h"
#include "params_file.h"
using namespace tracking;

master_box::master_box(params_file* params_in,params_file* params_out ){
  
  in_wrapper = params_in->make_wrapper_in();
  out_wrapper = params_out->make_wrapper_out();
  
  data_types = in_wrapper->get_data_types();
  data_types.insert(wrapper::D_UNQID);

  
  contents.reserve(in_wrapper->num_entries());
  
  particle::intialize_wrapper_in(in_wrapper);
  particle::intialize_wrapper_out(out_wrapper);
  particle::intialize_data_types(&data_types);

  for(int j = 0; j<in_wrapper->num_entries(); ++j){
    contents.push_back( new particle(j));
  }

}

master_box::master_box(params_file* params_in,params_file* params_out, int n ){
  
  in_wrapper = params_in->make_wrapper_in();
  out_wrapper = params_out->make_wrapper_out();
  
  data_types = in_wrapper->get_data_types();
  data_types.insert(wrapper::D_UNQID);

  particle::intialize_wrapper_in(in_wrapper);
  particle::intialize_wrapper_out(out_wrapper);
  particle::intialize_data_types(&data_types);

  contents.reserve(in_wrapper->num_entries());
  for(int j = 0; j<in_wrapper->num_entries(); ++j){
    contents.push_back( new particle_track(j));
  }

}

void master_box::print(){
  for(unsigned int j = 0; j<contents.size();j++)
    contents.at(j)->print();
}

master_box::~master_box(){
    //deletes the particles it made
  for(unsigned int j = 0; j<contents.size();j++)
    {
      delete contents.at(j);
    }
  //deletes the wrapper objects
  delete in_wrapper;
  delete out_wrapper;
}

