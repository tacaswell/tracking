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

Touple Touple::operator+ (const Touple& y){
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}


Touple Touple::operator- (const Touple& y){
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}

Touple Touple::operator- (){
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}


Touple Touple::operator/ (const Touple& y){
  Touple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}


double& Touple::operator[] (int i){
  if(i<0 || i>= length_)
    throw "error"; 		// place holder for proper error handeling
  return data_[i];
}


void Touple::operator+= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}

void Touple::operator+= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;
}



void Touple::operator-= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}

void Touple::operator-= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}

void Touple::operator/= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}

void Touple::operator/= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}

void Touple::operator*= (const Touple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;

}

void Touple::operator*= (const double y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}



void Touple::print(){
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}


double Touple::magnitude_sqr(){
  double tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}

double Touple::magnitude(){
  return sqrt(magnitude_sqr());
}

void Touple::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

Touple::Touple(){
  data_= (double*) new double[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;


}


Touple::~Touple(){
  //  std::cout<<"died"<< std::endl;
  delete[] data_;
  data_ = NULL;
}