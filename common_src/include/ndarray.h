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
#ifndef ARRAY_ND
#define ARRAY_ND


#include "part_def.h"
#include <iostream>
namespace utilities{


/**
   A N-D array (set by the 
*/
template <class T,class T_dim>
class ND_Array{
public:
  /**
     Returns a const pointer to the data.  This is useful for dealing
     with wrappers.
   */
  const T * data_ptr() const{
    return data_ptr_;
  }
  
  /**
     Returns a reference to at element
   */
  T& operator()(T_dim pos);
  /**
     Returns a const reference to at element
   */
  const T& operator()(T_dim pos)const;
  
  ND_Array(T_dim dim);
  ~ND_Array();
  
  /**
     Prints a representation of the 
   */
  void print() const;
  
  /**
     Fills the array with the index of the positions,
     this assumes that the type is int or similar
   */
  void fill_test();
  
private:
  // pointer to the data
  T * data_ptr_;
  // The dimensions
  T_dim dims_;
  // the cumulative dimensions 
  T_dim cum_dims_;
  
  int cord_to_indx(const T_dim& pos) const;
  T_dim indx_to_cord(int indx) const;
  
  /**
     Inrements to the next 2-D plane to 
   */
  bool increment_cord(T_dim & cord)const;
  
  
  
};
template <class T,class T_dim>
T& ND_Array<T,T_dim>::operator()(T_dim pos)
{
  return data_ptr_[cord_to_indx(pos)];
}
template <class T,class T_dim>
const T& ND_Array<T,T_dim>::operator()(T_dim pos)const
{
  return data_ptr_[cord_to_indx(pos)];
}

template <class T,class T_dim>
ND_Array<T,T_dim>::ND_Array(T_dim dims)
{
  int rank = dims.get_len();
  
  int dim_prod = 1;
  
  dims_ = dims;
  
  
  for(int j = 0; j<rank;++j)
  {
    dim_prod*=dims_[j];
    
    
    unsigned int tmp_prod = 1;
    for(int k = 0;k<(j);++k)
    {
      tmp_prod*=dims_[k];
    }
    cum_dims_[j] = tmp_prod;
  }

  data_ptr_= new T[dim_prod] ;
}

template <class T,class T_dim>
ND_Array<T,T_dim>::~ND_Array()
{
  delete [] data_ptr_;
}

template <class T,class T_dim>
int ND_Array<T,T_dim>::cord_to_indx(const T_dim & in)const
{
  return (in*cum_dims_).sum();
}
template <class T,class T_dim>
T_dim ND_Array<T,T_dim>::indx_to_cord(int indx)const
{
  T_dim cord;
  for(int j = 0;j<T_dim::length_;++j)
    cord[j] = (indx/cum_dims_[j])%dims_[j];
  
  return T_dim(cord);
  
}

template <class T,class T_dim>
void ND_Array<T,T_dim>::fill_test() 
{
  int max = dims_.prod();
  for(int j= 0; j<max;++j)
    data_ptr_[j] = j;
  
}


template <class T,class T_dim>
void ND_Array<T,T_dim>::print() const
{
  using std::cout;
  using std::endl;

  T_dim plane;
  do
  {
    cout<<"plane "<<plane<<endl;
    int offset = cord_to_indx(plane);
    for(int j = 0;j<dims_[0];++j)
    {
      for(int k = 0;k<dims_[1];++k)
      {
	cout<<data_ptr_[offset + j + cum_dims_[1]*k]<<'\t';
      }
      cout<<endl;
      
    }
    
  }
  while(increment_cord(plane));
  
      
}

template <class T,class T_dim>
bool ND_Array<T,T_dim>::increment_cord(T_dim & cord)const
{
  int rank = T_dim::length_;
  for(int j = rank-1;j>1;--j)
  {
    if(cord[j]+1 < dims_[j])
    {
      ++cord[j];
      return true;
    }
    else
      cord[j] = 0;
    
  }
  return false;
}


}





#endif
