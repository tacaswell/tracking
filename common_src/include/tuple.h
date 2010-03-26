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
#ifndef TUPLE_T
#define TUPLE_T

#include <iostream>
#include <sstream>
#include <stdexcept>
namespace utilities{
/**
   Class for carrying around the posistion and velocity vectors.
   The logic for making this class is a memory concern that there
   will be millions of these objects.  Also to make some specfic
   matlab like functions simpler to implement
*/
template<class T,int length>
class Tuple{
public:
  /**
     Over loads for comparison	
  */
  bool operator==(const Tuple& b)const;
  
  /**
     Over loads for copying
  */
  void operator= (const Tuple& y);
  /**
     Over loads for negation
  */
  Tuple operator- ( );

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
  Tuple operator+ (const Tuple& y) const;
  /**
     Over loads for constant addition
  */
  Tuple operator+ (const T y)const;
  /**
     adds Tuple to the current Tuple
  */
  void operator+= (const Tuple& y);
  /**
     adds constant to the current Tuple
  */
  void operator+= (const T y);



  /**
     Over loads for element-wise subtraction
  */
  Tuple operator- ( const Tuple& y)const;
  /**
     Over loads for constant subtraction
  */
  Tuple operator- ( const T y)const;
  /**
     subtracts Tuple to the current Tuple
  */
  void operator-= (const Tuple& y);
  /**
     subtracts constant to the current Tuple
  */
  void operator-= (const T y);



  /**
     Over loads for constant multilplication
  */
  Tuple operator* (const T y) const;
  /**
     Over loads for elementwise mustiplication
  */
  Tuple operator* (const Tuple& y) const;
  /**
     element wise multilication of the current Tuple
  */
  void operator*= (const Tuple& y);
  /**
     multiplies current Tuple by a constant
  */
  void operator*= (const T y);



  /**
     Over loads for element-wise division
  */
  Tuple operator/ ( const Tuple& y) const;
  /**
     element wise division of the current Tuple
  */
  void operator/= (const Tuple& y);
  /**
     devides current Tuple by a constant
  */
  void operator/= (const T y);



  /**
     square of the magnitude of the Tuple assuming euclidian space
  */
  T magnitude_sqr () const;

  /**
     magnitude of the Tuple assuming euclidian space
  */
  T magnitude () const;

  /**
     take the dot product between two tuple
  */
  T dot(const Tuple & y) const;
   
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
  Tuple direction() const;

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
  Tuple();

  /**
     Copy constructor
  */
  Tuple(const Tuple & y);

  /**
     returns the distance squared between two points
  */
  T dist_sqr(const Tuple& y)const;
  

  /**
     Destructor
  */
  ~Tuple();
  
  /**
     hard coded 2-D constructor 
  */
  Tuple(T x,T y);
  /**
     hard coded 3-D constructor
   */
  Tuple(T x,T y,T z);
  
  /**
     Takes in an array, assumes it is the length length
  */
  Tuple(const T * );

  
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
     
   */
  const static int length_ = length;
  
  /**
     Template for converting data types
   */
  template<class T2>
  Tuple<T,length>(const Tuple<T2,length>&);
  

  /**
     Returns the length
   */
  int get_len()const
  {
    return length;
  }
  
protected:
  /**
     pointer to data
  */
  T data_[length];

private:
};

  
/**
   prints out the tuple to cout
   lifted almost whole cloth from c++/4.3/complex
*/
template<typename _CharT, class _Traits,class T,int length>
std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os, const Tuple<T,length>& in)
{

    
  std::basic_ostringstream<_CharT, _Traits> s;
  s.flags(__os.flags());
  s.imbue(__os.getloc());
  s.precision(__os.precision());

    
  s << '(' ;
  s<< in[0]  ;
  for(int j = 1;j<in.get_len();++j)
    s<< ','<<in[j]  ;
  s << ')';

  return __os << s.str();
}


template<class T,int length>
bool Tuple<T,length>::operator==(const Tuple<T,length>& b)const
{
  for(int j = 0;j<length; ++j)
    if(data_[j] != b.data_[j])
      return false;
  return true;
}

template<class T,int length>
void Tuple<T,length>::operator= (const Tuple<T,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] = y.data_[j];
}
template<class T,int length>
T& Tuple<T,length>::operator[] (int i){
  if(i<0 || i>= length)
    throw "Tuple range error"; 		// place holder for proper error handling
  return data_[i];
}
template<class T,int length>
const T& Tuple<T,length>::operator[] (int i) const{
  if(i<0 || i>= length)
    throw "Tuple range error"; 		// place holder for proper error handeling
  return data_[i];
}
template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator- (){
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}
template<class T,int length>

Tuple<T,length> Tuple<T,length>::operator+ (const Tuple<T,length>& y) const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator+ (const T y)const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
template<class T,int length>
void Tuple<T,length>::operator+= (const Tuple<T,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] += y.data_[j] ;

}
template<class T,int length>
void Tuple<T,length>::operator+= (const T y){
  for(int j = 0;j<length; ++j)
    data_[j] += y ;

}

template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator- (const Tuple<T,length>& y) const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator- (const T y) const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
template<class T,int length>
void Tuple<T,length>::operator-= (const Tuple<T,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] -= y.data_[j] ;

}
template<class T,int length>
void Tuple<T,length>::operator-= (const T y){
  for(int j = 0;j<length; ++j)
    data_[j] -= y ;
}


template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator* (const Tuple<T,length>& y) const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator* (const T y) const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
template<class T,int length>
void Tuple<T,length>::operator*= (const Tuple<T,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] *= y.data_[j] ;
}
template<class T,int length>
void Tuple<T,length>::operator*= (const T y){
  for(int j = 0;j<length; ++j)
    data_[j] *= y ;
}



template<class T,int length>
void Tuple<T,length>::operator/= (const Tuple<T,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] /= y.data_[j] ;

}
template<class T,int length>
void Tuple<T,length>::operator/= (const T y){
  for(int j = 0;j<length; ++j)
    data_[j] /= y ;
}
template<class T,int length>
Tuple<T,length> Tuple<T,length>::operator/ (const Tuple<T,length>& y) const{
  Tuple<T,length> tmp;
  for(int j = 0;j<length; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}




template<class T,int length>
void Tuple<T,length>::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}

template<class T,int length>
T Tuple<T,length>::magnitude_sqr() const{
  T tmp = 0;
  for(int j = 0;j<length; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}
template<class T,int length>
T Tuple<T,length>::magnitude() const
{
  return sqrt(magnitude_sqr());
}
template<class T,int length>
T Tuple<T,length>::dot(const Tuple<T,length> & y) const
{
  T tmp = 0;
  for(int j = 0;j<length; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}

template<class T,int length>
T Tuple<T,length>::prod()const
{
  T tmp = 1;
  for(int j = 0;j<length; ++j)
    tmp *= data_[j];
  return tmp;
  
}
template<class T,int length>
T Tuple<T,length>::sum()const
{
  T tmp = 0;
  for(int j = 0;j<length; ++j)
    tmp += data_[j];
  return tmp;
} 
template<class T,int length>
void Tuple<T,length>::make_unit(){
  T tmp = magnitude();
  for(int j = 0;j<length; ++j)
    data_[j] /= tmp ;
}
template<class T,int length>
Tuple<T,length> Tuple<T,length>::direction() const{
  Tuple<T,length> tmp = (*this);
  tmp.make_unit();
  return tmp;

}

template<class T,int length>
void Tuple<T,length>::clear(){
  for(int j = 0;j<length; ++j)
    data_[j] =0 ;
}

template<class T,int length>
Tuple<T,length>::Tuple(){
  //  data_= (T*) new T[length];
  for(int j = 0;j<length; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}

// tac 2009-07-17
// added 
template<class T,int length>
Tuple<T,length>::Tuple(const Tuple<T,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] = y.data_[j];
}

template<class T,int length>
template<class T2>
Tuple<T,length>::Tuple(const Tuple<T2,length>& y){
  for(int j = 0;j<length; ++j)
    data_[j] = (T) y[j];
}


// tac 2009-10-07
// added because the constructors and destructors of Tuple<T,length>s are taking ~5% of run time
template<class T,int length>
T Tuple<T,length>::dist_sqr(const Tuple<T,length>& y)const
{
  T out =0;
  
  for(int j = 0;j<length;++j)
  {
    out += (data_[j]-y.data_[j])*(data_[j]-y.data_[j]);
  }
  return out;
}

template<class T,int length>
Tuple<T,length>::~Tuple<T,length>(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
  //   data_ = NULL;
}
template<class T,int length>
Tuple<T,length>::Tuple(T x,T y)
{
  if(length != 2)
    throw std::logic_error("Tuple, using wrong number of arguments for this instantiation");
  data_[0] = x;
  data_[1] = y;
}


template<class T,int length>
Tuple<T,length>::Tuple(T x,T y,T z)
{
  if(length != 3)
    throw std::logic_error("Tuple, using wrong number of arguments for this instantiation");
  
  data_[0] = x;
  data_[1] = y;
  data_[2] = z;
}

template<class T,int length>
Tuple<T,length>::Tuple(const T * in )
{
  for( int j = 0;j<length;++j)
    data_[j] = in[j];
}

template<class T,int length>
const T * Tuple<T,length>::get_ptr() const
{
  return data_;
}

template<class T,int length>
T * Tuple<T,length>::get_ptr() 
{
  return data_;
}
    
  

}

#endif
