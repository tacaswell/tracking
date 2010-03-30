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
template <class T,int N>
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
  T& operator()(const Tuple<int,N> & pos);
  /**
     Returns a const reference to at element
   */
  const T& operator()(const Tuple<int,N> & pos)const;
  
  
  /**
     Returns a reference to at element
   */
  T& operator()(int pos);
  /**
     Returns a const reference to at element
   */
  const T& operator()(int pos)const;
  

  ND_Array(const Tuple<int,N> & dim);
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
  /**
     Rank of matrix
   */
  const static int rank_ =  N;
  
  

private:
  
  /// pointer to the data
  T * data_ptr_;
  /// The dimensions
  const Tuple<int,N>  dims_;
  /**
     Pointer to the data in dims_
   */
  const int * dims_ptr_;
  
  /// the cumulative dimensions 
  Tuple<int,N>  cum_dims_;
  /**
     Pointer to data in cum_dims_
   */
  const int * cum_dims_ptr_;
  
  /**
     Total number of elements
   */
  int elm_count_;
  
  
  int cord_to_indx(const Tuple<int,N> & pos) const;
  Tuple<int,N>  indx_to_cord(int indx) const;
  
  /**
     Inrements to the next 2-D plane to 
   */
  bool increment_cord( Tuple<int,N> & cord)const;
  
  
  
};
template <class T,int N>
T& ND_Array<T,N>::operator()(const Tuple<int,N> & pos)
{
  return data_ptr_[cord_to_indx(pos)];
}
template <class T,int N>
const T& ND_Array<T,N>::operator()(const Tuple<int,N> & pos)const
{
  return data_ptr_[cord_to_indx(pos)];
}


template <class T,int N>
T& ND_Array<T,N>::operator()(int pos)
{
  if(!(pos < elm_count_))
     throw std::out_of_range("posistion out of range");
  return data_ptr_[pos];
}

template <class T,int N>
const T& ND_Array<T,N>::operator()(int pos) const
{
  if(!(pos < elm_count_))
     throw std::out_of_range("posistion out of range");
  return data_ptr_[pos];
}


template <class T,int N>
ND_Array<T,N>::ND_Array(const Tuple<int,N> & dims):
  dims_(dims),cum_dims_(0)
{
  int dim_prod = 1;
  
  
  
  
  for(int j = 0; j<rank_;++j)
  {
    dim_prod*=dims_[j];
    
    
    unsigned int tmp_prod = 1;
    for(int k = 0;k<(j);++k)
    {
      tmp_prod*=dims_[k];
    }
    cum_dims_[j] = tmp_prod;
  }
  elm_count_ = dims_.prod();
  
  data_ptr_= new T[dim_prod] ;
  dims_ptr_ = dims_.get_ptr();
  cum_dims_ptr_ = cum_dims_.get_ptr();
  

}

template <class T,int N>
ND_Array<T,N>::~ND_Array()
{
  delete [] data_ptr_;
}

template <class T,int N>
int ND_Array<T,N>::cord_to_indx(const Tuple<int,N> & in)const
{
  int indx = 0;
  
  for(int j = 0; j<rank_;++j)
  {
    int tmp = in[j];
    
    if (tmp>dims_ptr_[j])
      throw std::out_of_range("cord out of dimensions");
    indx +=tmp*cum_dims_ptr_[j];
  }
  return indx;

}
template <class T,int N>
Tuple<int,N> ND_Array<T,N>::indx_to_cord(int indx)const
{
  if(!(indx<elm_count_))
    throw std::out_of_range("Index out of range");
  
  const Tuple<int,N> & cord;
  for(int j = 0;j<N;++j)
    cord[j] = (indx/cum_dims_ptr_[j])%dims_ptr_[j];
  
  return  Tuple<int,N> (cord);
  
}

template <class T,int N>
void ND_Array<T,N>::fill_test() 
{
  int max = dims_.prod();
  for(int j= 0; j<max;++j)
    data_ptr_[j] = j;
  
}


template <class T,int N>
void ND_Array<T,N>::print() const
{
  using std::cout;
  using std::endl;

  Tuple<int,N>  plane;
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

template <class T,int N>
bool ND_Array<T,N>::increment_cord(Tuple<int,N> & cord)const
{

  for(int j = rank_-1;j>1;--j)
  {
    if(cord[j]+1 < dims_ptr_[j])
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
