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

#include<iostream>

#include "ipp.h"

#include "iden/wrapper_i_plu.h"

using utilities::Wrapper_i_plu;

using std::cout;
using std::endl;

using std::vector;
using std::set;

using utilities::Data_map;

void Wrapper_i_plu::add_frame_data(Ipp32f data[][9], int frame,int num)
{
  if(data_.at(frame) !=NULL)
    throw "already data at this frame";

  cout<<"frame "<<frame<<endl;
  
  data_.at(frame) = data;
  frame_count_.at(frame) = num;
  count_+=num;
  
}

void Wrapper_i_plu::print()const
{
  cout<<"cotains: "<<count_<<endl;
  cout<<"frames: "<<data_.size()<<endl;
}


void Wrapper_i_plu::get_value(int& out,int ind,D_TYPE type, int frame) const
{
  if(V_INT!=v_type(type))
    throw "wrong data type";
  if(type == D_FRAME)
  {
    out = frame;
    return;
  }
  
  //  out = (int) (*(*(data_.at(frame) + ind*cols) + data_map_(type)));
  out = (int)(data_.at(frame)[ind][data_map_(type)]);
}


void Wrapper_i_plu::get_value(float& out,int ind,D_TYPE type, int frame) const
{
  if(V_FLOAT!=v_type(type))
    throw "wrong data type";
  
  //  out = (float)(*(*(data_.at(frame) + ind*cols) + data_map_(type)));
  out = (float)(data_.at(frame)[ind][data_map_(type)]);
}

Wrapper_i_plu::~Wrapper_i_plu()
{
  for(vector<Ipp32f (*)[9]>::iterator it = data_.begin();
      it!=data_.end();++it)
  {
    delete [] *it;
  }
}


Wrapper_i_plu::Wrapper_i_plu(int frames):data_(frames,NULL),frame_count_(frames,0),count_ (0)
{
  cout<<"data_ size: "<<data_.size()<<endl;
  
  data_map_.set_lookup(D_XPOS,1);
  data_map_.set_lookup(D_YPOS,2);
  data_map_.set_lookup(D_DX  ,3);
  data_map_.set_lookup(D_DY  ,4);
  data_map_.set_lookup(D_I   ,5);
  data_map_.set_lookup(D_R2  ,6);
  data_map_.set_lookup(D_MULT,7);
  data_map_.set_lookup(D_E   ,8);
 
  D_TYPE tmp[] = {D_XPOS,D_YPOS,D_DX,D_DY,D_I,D_R2,D_MULT,D_E,D_FRAME};
  data_types_ = set<D_TYPE>(tmp, tmp+10);
}

void Wrapper_i_plu::get_data_types(std::set<utilities::D_TYPE>& out) const
{
  out = set<D_TYPE>(data_types_);
}

int Wrapper_i_plu::get_num_entries(int j) const{
  if(j == -1)
    return count_;
  else
    return frame_count_.at(j);
  
}
  
bool Wrapper_i_plu::contains_type(utilities::D_TYPE in) const
{
  set<D_TYPE>::iterator it = data_types_.find(in);
  return it != data_types_.end();
}
