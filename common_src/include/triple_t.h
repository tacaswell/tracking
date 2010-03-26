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



#ifndef TRIPLE_T
#define TRIPLE_T

#include <iostream>
#include <sstream>
namespace utilities{
/**
   Class for carrying around the posistion and velocity vectors.
   The logic for making this class is a memory concern that there
   will be millions of these objects.  Also to make some specfic
   matlab like functions simpler to implement
*/
template<class T>
class Triple_t{
public:
  /**
     Over loads for comparison	
  */
  bool operator==(const Triple_t& b)const;
  
  /**
     Over loads for copying
  */
  void operator= (const Triple_t& y);
  /**
     Over loads for negation
  */
  Triple_t operator- ( );

  /**
     Overides random access
  */
  T& operator[] (int i);	
  /**
     Overides random access
  */
  const T& operator[] (int i) const;

  /**
     Over loads for element-wise addition
  */
  Triple_t operator+ (const Triple_t& y) const;
  /**
     Over loads for constant addition
  */
  Triple_t operator+ (const T y)const;
  /**
     adds Triple_t to the current Triple_t
  */
  void operator+= (const Triple_t& y);
  /**
     adds constant to the current Triple_t
  */
  void operator+= (const T y);



  /**
     Over loads for element-wise subtraction
  */
  Triple_t operator- ( const Triple_t& y)const;
  /**
     Over loads for constant subtraction
  */
  Triple_t operator- ( const T y)const;
  /**
     subtracts Triple_t to the current Triple_t
  */
  void operator-= (const Triple_t& y);
  /**
     subtracts constant to the current Triple_t
  */
  void operator-= (const T y);



  /**
     Over loads for constant multilplication
  */
  Triple_t operator* (const T y) const;
  /**
     Over loads for elementwise mustiplication
  */
  Triple_t operator* (const Triple_t& y) const;
  /**
     element wise multilication of the current Triple_t
  */
  void operator*= (const Triple_t& y);
  /**
     multiplies current Triple_t by a constant
  */
  void operator*= (const T y);



  /**
     Over loads for element-wise division
  */
  Triple_t operator/ ( const Triple_t& y) const;
  /**
     element wise division of the current Triple_t
  */
  void operator/= (const Triple_t& y);
  /**
     devides current Triple_t by a constant
  */
  void operator/= (const T y);



  /**
     square of the magnitude of the Triple_t assuming euclidian space
  */
  T magnitude_sqr () const;

  /**
     magnitude of the Triple_t assuming euclidian space
  */
  T magnitude () const;

  /**
     take the dot product between two tuple
  */
  T dot(const Triple_t & y) const;
   
  /**
     Ruturns the product of the elements
  */
  T prod()const;
  
  /**
     Ruturns the sum of the elements
  */
  T sum()const;
  

  /**
     makes a tuple a unit vector
  */
  void make_unit();

  /**
     makes a tuple a unit vector
  */
  Triple_t direction() const;

  /**
     prints the data
  */
  void print() const;

  /**
     sets tuple to (0,0)
  */

  void clear();

  /**
     empty constructor
  */
  Triple_t();

  /**
     Copy constructor
  */
  Triple_t(const Triple_t & y);

  /**
     returns the distance squared between two points
  */
  T dist_sqr(const Triple_t& y)const;
  

  /**
     Destructor
  */
  ~Triple_t();

  /**
     Length, 3 as this is a triple
  */
  const static int length_ =3;
  
  /**
     hard coded 2-D constructor 
  */
  Triple_t(T x,T y,T z);
  /**
     Takes in an array, assumes it is the length_ length_
  */
  Triple_t(const T * );

  
  /**
     returns a pointer to the internal data in a safe way.
     mostly to make it simpler to write out to hdf wrappers
  */
  const T * get_ptr() const;
  
  /**
     returns a pointer to the internal data in a unsafe way.
     mostly to make it simpler to read out from hdf wrappers
  */
  T * get_ptr() ;
  


  
protected:
  /**
     pointer to data
  */
  T data_[length_];

private:
};

  
/**
   prints out the tuple to cout
   lifted almost whole cloth from c++/4.3/complex
*/
template<typename _CharT, class _Traits,class T>
std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os, const Triple_t<T>& in)
{

    
  std::basic_ostringstream<_CharT, _Traits> s;
  s.flags(__os.flags());
  s.imbue(__os.getloc());
  s.precision(__os.precision());

    
  s << '(' ;
  s<< in[0]  ;
  for(int j = 1;j<(Triple_t<T>::length_);++j)
    s<< ','<<in[j]  ;
  s << ')';

  return __os << s.str();
}


template<class T>
bool Triple_t<T>::operator==(const Triple_t<T>& b)const
{
  for(int j = 0;j<length_; ++j)
    if(data_[j] != b.data_[j])
      return false;
  return true;
}

template<class T>
void Triple_t<T>::operator= (const Triple_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
template<class T>
T& Triple_t<T>::operator[] (int i){
  if(i<0 || i>= length_)
    throw "Triple range error"; 		// place holder for proper error handling
  return data_[i];
}
template<class T>
const T& Triple_t<T>::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "Triple range error"; 		// place holder for proper error handeling
  return data_[i];
}
template<class T>
Triple_t<T> Triple_t<T>::operator- (){
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}
template<class T>

Triple_t<T> Triple_t<T>::operator+ (const Triple_t<T>& y) const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
template<class T>
Triple_t<T> Triple_t<T>::operator+ (const T y)const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
template<class T>
void Triple_t<T>::operator+= (const Triple_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
template<class T>
void Triple_t<T>::operator+= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}

template<class T>
Triple_t<T> Triple_t<T>::operator- (const Triple_t<T>& y) const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
template<class T>
Triple_t<T> Triple_t<T>::operator- (const T y) const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
template<class T>
void Triple_t<T>::operator-= (const Triple_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
template<class T>
void Triple_t<T>::operator-= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}


template<class T>
Triple_t<T> Triple_t<T>::operator* (const Triple_t<T>& y) const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
template<class T>
Triple_t<T> Triple_t<T>::operator* (const T y) const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
template<class T>
void Triple_t<T>::operator*= (const Triple_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
template<class T>
void Triple_t<T>::operator*= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}



template<class T>
void Triple_t<T>::operator/= (const Triple_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
template<class T>
void Triple_t<T>::operator/= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
template<class T>
Triple_t<T> Triple_t<T>::operator/ (const Triple_t<T>& y) const{
  Triple_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}




template<class T>
void Triple_t<T>::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}

template<class T>
T Triple_t<T>::magnitude_sqr() const{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}
template<class T>
T Triple_t<T>::magnitude() const
{
  return sqrt(magnitude_sqr());
}
template<class T>
T Triple_t<T>::dot(const Triple_t<T> & y) const
{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}

template<class T>
T Triple_t<T>::prod()const
{
  T tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
template<class T>
T Triple_t<T>::sum()const
{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 
template<class T>
void Triple_t<T>::make_unit(){
  T tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}
template<class T>
Triple_t<T> Triple_t<T>::direction() const{
  Triple_t<T> tmp = (*this);
  tmp.make_unit();
  return tmp;

}

template<class T>
void Triple_t<T>::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

template<class T>
Triple_t<T>::Triple_t(){
  //  data_= (T*) new T[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}

// tac 2009-07-17
// added 
template<class T>
Triple_t<T>::Triple_t(const Triple_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}

// tac 2009-10-07
// added because the constructors and destructors of Triple_t<T>s are taking ~5% of run time
template<class T>
T Triple_t<T>::dist_sqr(const Triple_t<T>& y)const
{
  T out =0;
  
  for(int j = 0;j<length_;++j)
  {
    out += (data_[j]-y.data_[j])*(data_[j]-y.data_[j]);
  }
  return out;
}

template<class T>
Triple_t<T>::~Triple_t<T>(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
  //   data_ = NULL;
}
template<class T>
Triple_t<T>::Triple_t(T x,T y,T z)
{
  data_[0] = x;
  data_[1] = y;
  data_[2] = z;
}

template<class T>
Triple_t<T>::Triple_t(const T * in )
{
  for( int j = 0;j<length_;++j)
    data_[j] = in[j];
}

template<class T>
const T * Triple_t<T>::get_ptr() const
{
  return data_;
}

template<class T>
T * Triple_t<T>::get_ptr() 
{
  return data_;
}
    
  

}

#endif
