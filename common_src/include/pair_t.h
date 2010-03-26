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
#ifndef PAIR_T
#define PAIR_T

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
class Pair_t{
public:
  /**
     Over loads for comparison	
  */
  bool operator==(const Pair_t& b)const;
  
  /**
     Over loads for copying
  */
  void operator= (const Pair_t& y);
  /**
     Over loads for negation
  */
  Pair_t operator- ( );

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
  Pair_t operator+ (const Pair_t& y) const;
  /**
     Over loads for constant addition
  */
  Pair_t operator+ (const T y)const;
  /**
     adds Pair_t to the current Pair_t
  */
  void operator+= (const Pair_t& y);
  /**
     adds constant to the current Pair_t
  */
  void operator+= (const T y);



  /**
     Over loads for element-wise subtraction
  */
  Pair_t operator- ( const Pair_t& y)const;
  /**
     Over loads for constant subtraction
  */
  Pair_t operator- ( const T y)const;
  /**
     subtracts Pair_t to the current Pair_t
  */
  void operator-= (const Pair_t& y);
  /**
     subtracts constant to the current Pair_t
  */
  void operator-= (const T y);



  /**
     Over loads for constant multilplication
  */
  Pair_t operator* (const T y) const;
  /**
     Over loads for elementwise mustiplication
  */
  Pair_t operator* (const Pair_t& y) const;
  /**
     element wise multilication of the current Pair_t
  */
  void operator*= (const Pair_t& y);
  /**
     multiplies current Pair_t by a constant
  */
  void operator*= (const T y);



  /**
     Over loads for element-wise division
  */
  Pair_t operator/ ( const Pair_t& y) const;
  /**
     element wise division of the current Pair_t
  */
  void operator/= (const Pair_t& y);
  /**
     devides current Pair_t by a constant
  */
  void operator/= (const T y);



  /**
     square of the magnitude of the Pair_t assuming euclidian space
  */
  T magnitude_sqr () const;

  /**
     magnitude of the Pair_t assuming euclidian space
  */
  T magnitude () const;

  /**
     take the dot product between two tuple
  */
  T dot(const Pair_t & y) const;
   
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
  Pair_t direction() const;

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
  Pair_t();

  /**
     Copy constructor
  */
  Pair_t(const Pair_t & y);

  /**
     returns the distance squared between two points
  */
  T dist_sqr(const Pair_t& y)const;
  

  /**
     Destructor
  */
  ~Pair_t();

  /**
     The length of tuple.  This should match the dimenion of the tracking.
     Exactly how this will all fit together will be seen.
  */
  const static int length_ =2;
  
  /**
     hard coded 2-D constructor 
  */
  Pair_t(T x,T y);
  /**
     Takes in an array, assumes it is the length_ length_
  */
  Pair_t(const T * );

  
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
operator<<(std::basic_ostream<_CharT, _Traits>& __os, const Pair_t<T>& in)
{

    
  std::basic_ostringstream<_CharT, _Traits> s;
  s.flags(__os.flags());
  s.imbue(__os.getloc());
  s.precision(__os.precision());

    
  s << '(' ;
  s<< in[0]  ;
  for(int j = 1;j<(Pair_t<T>::length_);++j)
    s<< ','<<in[j]  ;
  s << ')';

  return __os << s.str();
}


template<class T>
bool Pair_t<T>::operator==(const Pair_t<T>& b)const
{
  for(int j = 0;j<length_; ++j)
    if(data_[j] != b.data_[j])
      return false;
  return true;
}

template<class T>
void Pair_t<T>::operator= (const Pair_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
template<class T>
T& Pair_t<T>::operator[] (int i){
  if(i<0 || i>= length_)
    throw "Pair range error"; 		// place holder for proper error handling
  return data_[i];
}
template<class T>
const T& Pair_t<T>::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "Pair range error"; 		// place holder for proper error handeling
  return data_[i];
}
template<class T>
Pair_t<T> Pair_t<T>::operator- (){
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}
template<class T>

Pair_t<T> Pair_t<T>::operator+ (const Pair_t<T>& y) const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
template<class T>
Pair_t<T> Pair_t<T>::operator+ (const T y)const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
template<class T>
void Pair_t<T>::operator+= (const Pair_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
template<class T>
void Pair_t<T>::operator+= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}

template<class T>
Pair_t<T> Pair_t<T>::operator- (const Pair_t<T>& y) const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
template<class T>
Pair_t<T> Pair_t<T>::operator- (const T y) const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
template<class T>
void Pair_t<T>::operator-= (const Pair_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
template<class T>
void Pair_t<T>::operator-= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}


template<class T>
Pair_t<T> Pair_t<T>::operator* (const Pair_t<T>& y) const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
template<class T>
Pair_t<T> Pair_t<T>::operator* (const T y) const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
template<class T>
void Pair_t<T>::operator*= (const Pair_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
template<class T>
void Pair_t<T>::operator*= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}



template<class T>
void Pair_t<T>::operator/= (const Pair_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
template<class T>
void Pair_t<T>::operator/= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
template<class T>
Pair_t<T> Pair_t<T>::operator/ (const Pair_t<T>& y) const{
  Pair_t<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}




template<class T>
void Pair_t<T>::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}

template<class T>
T Pair_t<T>::magnitude_sqr() const{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}
template<class T>
T Pair_t<T>::magnitude() const
{
  return sqrt(magnitude_sqr());
}
template<class T>
T Pair_t<T>::dot(const Pair_t<T> & y) const
{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}

template<class T>
T Pair_t<T>::prod()const
{
  T tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
template<class T>
T Pair_t<T>::sum()const
{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 
template<class T>
void Pair_t<T>::make_unit(){
  T tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}
template<class T>
Pair_t<T> Pair_t<T>::direction() const{
  Pair_t<T> tmp = (*this);
  tmp.make_unit();
  return tmp;

}

template<class T>
void Pair_t<T>::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

template<class T>
Pair_t<T>::Pair_t(){
  //  data_= (T*) new T[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}

// tac 2009-07-17
// added 
template<class T>
Pair_t<T>::Pair_t(const Pair_t<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}

// tac 2009-10-07
// added because the constructors and destructors of Pair_t<T>s are taking ~5% of run time
template<class T>
T Pair_t<T>::dist_sqr(const Pair_t<T>& y)const
{
  T out =0;
  
  for(int j = 0;j<length_;++j)
  {
    out += (data_[j]-y.data_[j])*(data_[j]-y.data_[j]);
  }
  return out;
}

template<class T>
Pair_t<T>::~Pair_t<T>(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
  //   data_ = NULL;
}
template<class T>
Pair_t<T>::Pair_t(T x,T y)
{
  data_[0] = x;
  data_[1] = y;
}

template<class T>
Pair_t<T>::Pair_t(const T * in )
{
  for( int j = 0;j<length_;++j)
    data_[j] = in[j];
}

template<class T>
const T * Pair_t<T>::get_ptr() const
{
  return data_;
}

template<class T>
T * Pair_t<T>::get_ptr() 
{
  return data_;
}
    
  

}

#endif
