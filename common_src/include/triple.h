//Copyright 2009-2010 Thomas A Caswell
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
class Triple{
public:
  /**
     Over loads for comparison	
  */
  bool operator==(const Triple& b)const;
  
  /**
     Over loads for copying
  */
  void operator= (const Triple& y);
  /**
     Over loads for negation
  */
  Triple operator- ( );

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
  Triple operator+ (const Triple& y) const;
  /**
     Over loads for constant addition
  */
  Triple operator+ (const T y)const;
  /**
     adds Triple to the current Triple
  */
  void operator+= (const Triple& y);
  /**
     adds constant to the current Triple
  */
  void operator+= (const T y);



  /**
     Over loads for element-wise subtraction
  */
  Triple operator- ( const Triple& y)const;
  /**
     Over loads for constant subtraction
  */
  Triple operator- ( const T y)const;
  /**
     subtracts Triple to the current Triple
  */
  void operator-= (const Triple& y);
  /**
     subtracts constant to the current Triple
  */
  void operator-= (const T y);



  /**
     Over loads for constant multilplication
  */
  Triple operator* (const T y) const;
  /**
     Over loads for elementwise mustiplication
  */
  Triple operator* (const Triple& y) const;
  /**
     element wise multilication of the current Triple
  */
  void operator*= (const Triple& y);
  /**
     multiplies current Triple by a constant
  */
  void operator*= (const T y);



  /**
     Over loads for element-wise division
  */
  Triple operator/ ( const Triple& y) const;
  /**
     element wise division of the current Triple
  */
  void operator/= (const Triple& y);
  /**
     devides current Triple by a constant
  */
  void operator/= (const T y);



  /**
     square of the magnitude of the Triple assuming euclidian space
  */
  T magnitude_sqr () const;

  /**
     magnitude of the Triple assuming euclidian space
  */
  T magnitude () const;

  /**
     take the dot product between two tuple
  */
  T dot(const Triple & y) const;
   
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
  Triple direction() const;

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
  Triple();

  /**
     Copy constructor
  */
  Triple(const Triple & y);

  /**
     returns the distance squared between two points
  */
  T dist_sqr(const Triple& y)const;
  

  /**
     Destructor
  */
  ~Triple();

  /**
     Length, 3 as this is a triple
  */
  const static int length_ =3;
  
  /**
     hard coded 2-D constructor 
  */
  Triple(T x,T y,T z);
  /**
     Takes in an array, assumes it is the length_ length_
  */
  Triple(const T * );

  
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
  

    
  /**
     Template for converting
   */
  template<class T2>
  Triple<T>(const Triple<T2>&);


  
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
operator<<(std::basic_ostream<_CharT, _Traits>& __os, const Triple<T>& in)
{

    
  std::basic_ostringstream<_CharT, _Traits> s;
  s.flags(__os.flags());
  s.imbue(__os.getloc());
  s.precision(__os.precision());

    
  s << '(' ;
  s<< in[0]  ;
  for(int j = 1;j<(Triple<T>::length_);++j)
    s<< ','<<in[j]  ;
  s << ')';

  return __os << s.str();
}


template<class T>
bool Triple<T>::operator==(const Triple<T>& b)const
{
  for(int j = 0;j<length_; ++j)
    if(data_[j] != b.data_[j])
      return false;
  return true;
}

template<class T>
void Triple<T>::operator= (const Triple<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
template<class T>
T& Triple<T>::operator[] (int i){
  if(i<0 || i>= length_)
    throw "Triple range error"; 		// place holder for proper error handling
  return data_[i];
}
template<class T>
const T& Triple<T>::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "Triple range error"; 		// place holder for proper error handeling
  return data_[i];
}
template<class T>
Triple<T> Triple<T>::operator- (){
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}
template<class T>

Triple<T> Triple<T>::operator+ (const Triple<T>& y) const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
template<class T>
Triple<T> Triple<T>::operator+ (const T y)const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
template<class T>
void Triple<T>::operator+= (const Triple<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
template<class T>
void Triple<T>::operator+= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}

template<class T>
Triple<T> Triple<T>::operator- (const Triple<T>& y) const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
template<class T>
Triple<T> Triple<T>::operator- (const T y) const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
template<class T>
void Triple<T>::operator-= (const Triple<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
template<class T>
void Triple<T>::operator-= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}


template<class T>
Triple<T> Triple<T>::operator* (const Triple<T>& y) const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
template<class T>
Triple<T> Triple<T>::operator* (const T y) const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
template<class T>
void Triple<T>::operator*= (const Triple<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
template<class T>
void Triple<T>::operator*= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}



template<class T>
void Triple<T>::operator/= (const Triple<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
template<class T>
void Triple<T>::operator/= (const T y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
template<class T>
Triple<T> Triple<T>::operator/ (const Triple<T>& y) const{
  Triple<T> tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}




template<class T>
void Triple<T>::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}

template<class T>
T Triple<T>::magnitude_sqr() const{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}
template<class T>
T Triple<T>::magnitude() const
{
  return sqrt(magnitude_sqr());
}
template<class T>
T Triple<T>::dot(const Triple<T> & y) const
{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}

template<class T>
T Triple<T>::prod()const
{
  T tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
template<class T>
T Triple<T>::sum()const
{
  T tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 
template<class T>
void Triple<T>::make_unit(){
  T tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}
template<class T>
Triple<T> Triple<T>::direction() const{
  Triple<T> tmp = (*this);
  tmp.make_unit();
  return tmp;

}

template<class T>
void Triple<T>::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

template<class T>
Triple<T>::Triple(){
  //  data_= (T*) new T[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}

// tac 2009-07-17
// added 
template<class T>
Triple<T>::Triple(const Triple<T>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}


template<class T>
template<class T2>
Triple<T>::Triple(const Triple<T2>& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = (T) y[j];
}



// tac 2009-10-07
// added because the constructors and destructors of Triple<T>s are taking ~5% of run time
template<class T>
T Triple<T>::dist_sqr(const Triple<T>& y)const
{
  T out =0;
  
  for(int j = 0;j<length_;++j)
  {
    out += (data_[j]-y.data_[j])*(data_[j]-y.data_[j]);
  }
  return out;
}

template<class T>
Triple<T>::~Triple<T>(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
  //   data_ = NULL;
}
template<class T>
Triple<T>::Triple(T x,T y,T z)
{
  data_[0] = x;
  data_[1] = y;
  data_[2] = z;
}

template<class T>
Triple<T>::Triple(const T * in )
{
  for( int j = 0;j<length_;++j)
    data_[j] = in[j];
}

template<class T>
const T * Triple<T>::get_ptr() const
{
  return data_;
}

template<class T>
T * Triple<T>::get_ptr() 
{
  return data_;
}
    
  

}

#endif
