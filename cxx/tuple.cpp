#include "tuple.h"
#include<iostream>

#include <math.h>


using namespace utilities;

const int Tuple::length_ ;
//bool Tuple::allow_dim_change_ = false;

void Tuple::operator= (const Tuple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
double& Tuple::operator[] (int i){
  if(i<0 || i>= length_)
    throw "Tuple range error"; 		// place holder for proper error handling
  return data_[i];
}
const double& Tuple::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "Tuple range error"; 		// place holder for proper error handeling
  return data_[i];
}

Tuple Tuple::operator- (){
  Tuple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}


Tuple Tuple::operator+ (const Tuple& y) const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
Tuple Tuple::operator+ (const double y)const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
void Tuple::operator+= (const Tuple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
 
void Tuple::operator+= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}


Tuple Tuple::operator- (const Tuple& y) const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
Tuple Tuple::operator- (const double y) const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
void Tuple::operator-= (const Tuple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
void Tuple::operator-= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}



Tuple Tuple::operator* (const Tuple& y) const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
Tuple Tuple::operator* (const double y) const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
void Tuple::operator*= (const Tuple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
void Tuple::operator*= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}




void Tuple::operator/= (const Tuple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
void Tuple::operator/= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
Tuple Tuple::operator/ (const Tuple& y) const{
  Tuple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}





void Tuple::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}


double Tuple::magnitude_sqr() const{
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}

double Tuple::magnitude() const
{
  return sqrt(magnitude_sqr());
}

double Tuple::dot(const Tuple & y) const
{
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}


double Tuple::prod()const
{
  double tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
double Tuple::sum()const
{
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 

void Tuple::make_unit(){
  double tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}

Tuple Tuple::direction() const{
  Tuple tmp = (*this);
  tmp.make_unit();
  return tmp;

}

void Tuple::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

Tuple::Tuple(){
  //  data_= (double*) new double[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}


Tuple::~Tuple(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
//   data_ = NULL;
}
