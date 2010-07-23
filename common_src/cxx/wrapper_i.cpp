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
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting


#include "wrapper_i.h"
#include <stdexcept>
#include "md_store.h"
using utilities::Wrapper_in;
using utilities::MD_store;
using std::logic_error;
using std::vector;


const MD_store* Wrapper_in::get_MD_store(unsigned int j)const
{
  return md_stores_.at(j);
}
MD_store* Wrapper_in::get_MD_store(unsigned int j)
{
  return md_stores_.at(j);
}

void Wrapper_in::set_MD_store_size(unsigned int j)
{
  if(j<md_stores_.size())
  {
    for(unsigned int k = j; j<md_stores_.size();++k)
    {
      delete md_stores_[k];
      md_stores_[k] = NULL;
    }
  }
  
  md_stores_.resize(j,NULL);
}

unsigned int Wrapper_in::get_MD_store_size()
{
  return md_stores_.size();
}


void Wrapper_in::set_MD_store(unsigned int j, MD_store * in)
{
  if(md_stores_.at(j))
    throw logic_error("MD_store already exist for this plane ");
  md_stores_.at(j)  = in;
  
  
}

void Wrapper_in::clear_MD_store(unsigned int j)
{
  delete md_stores_.at(j);
  md_stores_.at(j) = NULL;
}

Wrapper_in::~Wrapper_in()
{
  for(vector<MD_store*>::iterator it = md_stores_.begin();
      it!= md_stores_.end();++it)
  {
    delete *it;
    *it = NULL;
  }
}

Wrapper_in::Wrapper_in():
  md_stores_(0,NULL)
{
}
