//Copyright 2010 Thomas A Caswell
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

//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

#include "corr.h"
using tracking::Corr;


void Corr::add_plane_temperature(float temp)
{
  temperature_sum_ += temp;
  ++plane_count_;
}
Corr::Corr():temperature_sum_(0),plane_count_(0){}

#include <stdexcept>
#include <cmath>
#include "hash_case.h"
#include "hash_shelf.h"
#include "hash_box.h"
#include "corr_case.h"
#include "md_store.h"
#include "part_def.h"
#include "particle_track.h"

using tracking::hash_box;
using tracking::Hash_shelf;
using tracking::hash_case;
using tracking::Corr_case;

using utilities::Md_store;
using utilities::Tuplei;
using utilities::Tuplef;

using std::vector;
using std::runtime_error;
using std::logic_error;
using std::cout;
using std::endl;


const std::string TEMPERATURE_STRING = "temperature";

unsigned int hash_case::compute_corr(Corr_case & in )const
{
  unsigned int hc_sz = h_case_.size();
  unsigned int cc_sz = in.size();
  
  if (cc_sz > hc_sz)
    throw runtime_error("the corr_case has more planes than the hash case");
  
  unsigned int step = hc_sz/cc_sz;
  unsigned int max_i = cc_sz * step;
  
  
  for(unsigned int j = 0; j<max_i;++j)
    h_case_[j]->compute_corr(*in[j/step]);

  return step;
  
}


void hash_case::compute_corr(Corr & in) const
{
  
  for(vector<Hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
    (*shelf_it)->compute_corr(in);
    //    cout<<"finished shelf"<<endl;
  }
}

void Hash_shelf::compute_corr(Corr & in)const
{

  // deal with temperature
  if(md_store_->contains_key(TEMPERATURE_STRING))
  {
    float temp;
    in.add_plane_temperature(md_store_->get_value(TEMPERATURE_STRING,temp));
  }
  

  float range = in.get_max_range();

  int buffer = (unsigned int)ceil(range/upb_);

#ifdef TESTING
  cout<<"corr buffer: "<<buffer<<endl;
#endif
  
  
  
  Tuplei bottom_corner, top_corner;
  // check top and bottom corners are in range
  for(int j = 0;j<Tuplei::length_;++j)
  {
    if(buffer>=hash_dims_[j])
      throw runtime_error("Hash_shelf::compute_corr buffer is bigger than a hash dimension");
    

    bottom_corner[j] = buffer;
    // add an extra +1 to avoid the boxes at the edges with partial populations
    top_corner[j] = hash_dims_[j] - buffer - 1;
    
  }
  
    
  Tuplei region_sides = top_corner - bottom_corner;
  int region_sz = (int)region_sides.prod();
  
  
  for(int j = 0;j<region_sz;++j)
  {
    Tuplei tmp = range_indx_to_tuple(j,region_sides);
    tmp += bottom_corner;
#ifdef TESTING
    cout<<"corr box: "<<tmp<<endl;
#endif
    int tmp_indx = tuple_to_indx(tmp);
    if(hash_.at(tmp_indx))
    {
      (hash_.at(tmp_indx))->compute_corr(in);;
    }
    
    
  }
  
}
void hash_box::compute_corr(Corr & in )const
{

  if(in.get_max_range()<=particle::get_neighborhood_range())
  {
    vector<particle*>::const_iterator myend = contents_.end();
    for(vector<particle*>::const_iterator it = contents_.begin();
	it!=myend;++it)
    {
      in.compute(*it,(*it)->get_neighborhood () );
    }
  }
  else
  {
    if(shelf_ ==NULL || hash_indx_ == -1)
      throw logic_error("hash_box: box not part of a shelf");

    vector <const particle *> nhood;
    shelf_->get_region_px(hash_indx_,nhood,in.get_max_range());
    
    int max_j = contents_.size();
    for(int j = 0; j<max_j;++j)
    {
      in.compute(contents_[j],nhood);
    }
  }
}

