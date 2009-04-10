#include "touple.h"
#include<iostream>

#include <math.h>


using namespace utilities;

const int Touple::length_ ;
//bool Touple::allow_dim_change_ = false;

void Touple::operator= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
double& Touple::operator[] (int i){
  if(i<0 || i>= length_)
    throw "error"; 		// place holder for proper error handeling
  return data_[i];
}
const double& Touple::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "error"; 		// place holder for proper error handeling
  return data_[i];
}

Touple Touple::operator- (){
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}


Touple Touple::operator+ (const Touple& y) const{
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
Touple Touple::operator+ (const double y)const{
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
void Touple::operator+= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
 
void Touple::operator+= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}


Touple Touple::operator- (const Touple& y) const{
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
Touple Touple::operator- (const double y) const{
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
void Touple::operator-= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
void Touple::operator-= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}



Touple Touple::operator* (const Touple& y) const{
  Touple tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
Touple Touple::operator* (const double y) const{
  Touple tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
void Touple::operator*= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
void Touple::operator*= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}




void Touple::operator/= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
void Touple::operator/= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
Touple Touple::operator/ (const Touple& y) const{
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}





void Touple::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}


double Touple::magnitude_sqr() const{
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}

double Touple::magnitude() const
{
  return sqrt(magnitude_sqr());
}

double Touple::dot(const Touple & y) const
{
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}


double Touple::prod()const
{
  double tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
double Touple::sum()const
{
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 

void Touple::make_unit(){
  double tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}

Touple Touple::direction() const{
  Touple tmp = (*this);
  tmp.make_unit();
  return tmp;

}

void Touple::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

Touple::Touple(){
  //  data_= (double*) new double[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}


Touple::~Touple(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
//   data_ = NULL;
}
