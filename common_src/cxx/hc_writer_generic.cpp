//Copyright 2009 Thomas A Caswell
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

#include "hash_case_writer_generic.h"
#include "generic_wrapper_base.h"
#include "tuple.h"

#include "hash_case.h"
#include <list>
#include <complex>


using namespace utilities;
using std::list;
using std::complex;
using tracking::hash_case;
using tracking::hash_shelf;
using tracking::particle;


void Hash_case_writer_generic::write_hash_case(const hash_case&  hc)const
{
  output_wrapper_->initialize_wrapper();
  
  const hash_shelf* current_shelf;
  const particle* current_partcle;
  
  int frames = hc ->get_num_frames();
  for(int j = 0; j<frames;++j)
  {
    current_shelf = hc->return_shelf(j);
    list<const particle*> cs_list;
    current_shelf->shelf_to_list(cs_list);
    list<const particle*>::const_iterator myend = cs_list.end();
    
    for(list<const particle*>::const_iterator it = cs_list.begin();
	it!=myend;++it)
    {
      current_partcle = *it;
      output_wrapper_->start_new_row();

      Tuple pos = current_partcle->get_position();
      output_wrapper_->append_to_row(pos[0]);
      output_wrapper_->append_to_row(pos[1]);
      
      complex<float> sop= current_partcle->get_scaler_order_pram();
      output_wrapper_->append_to_row(real(sop));
      output_wrapper_->append_to_row(imag(sop));
      
      
      output_wrapper_->append_to_row(current_partcle->get_neighborhood_size());
      output_wrapper_->append_to_row(current_partcle->get_value(tracking::wrapper::D_FRAME));
      output_wrapper_->finish_row();
      
    }
    
  }
  output_wrapper_->finalize_wrapper();
  

}
