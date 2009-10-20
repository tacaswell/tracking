//Copyright 2009 Thomas A Caswell
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
#include "pair.h"
#include<iostream>

#include <math.h>


using namespace utilities;

const int Pair::length_ ;
//bool Pair::allow_dim_change_ = false;

void Pair::operator= (const Pair& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
float& Pair::operator[] (int i){
  if(i<0 || i>= length_)
    throw "Pair range error"; 		// place holder for proper error handling
  return data_[i];
}
const float& Pair::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "Pair range error"; 		// place holder for proper error handeling
  return data_[i];
}

Pair Pair::operator- (){
  Pair tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}


Pair Pair::operator+ (const Pair& y) const{
  Pair tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
Pair Pair::operator+ (const float y)const{
  Pair tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
void Pair::operator+= (const Pair& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
 
void Pair::operator+= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}


Pair Pair::operator- (const Pair& y) const{
  Pair tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
Pair Pair::operator- (const float y) const{
  Pair tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
void Pair::operator-= (const Pair& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
void Pair::operator-= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}



Pair Pair::operator* (const Pair& y) const{
  Pair tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
Pair Pair::operator* (const float y) const{
  Pair tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
void Pair::operator*= (const Pair& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
void Pair::operator*= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}




void Pair::operator/= (const Pair& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
void Pair::operator/= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
Pair Pair::operator/ (const Pair& y) const{
  Pair tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}





void Pair::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}


float Pair::magnitude_sqr() const{
  float tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}

float Pair::magnitude() const
{
  return sqrt(magnitude_sqr());
}

float Pair::dot(const Pair & y) const
{
  float tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}


float Pair::prod()const
{
  float tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
float Pair::sum()const
{
  float tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 

void Pair::make_unit(){
  float tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}

Pair Pair::direction() const{
  Pair tmp = (*this);
  tmp.make_unit();
  return tmp;

}

void Pair::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

Pair::Pair(){
  //  data_= (float*) new float[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}

// tac 2009-07-17
// added 
Pair::Pair(const Pair& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}

// tac 2009-10-07
// added because the constructors and destructors of Pairs are taking ~5% of run time
float Pair::dist_sqr(const Pair& y)const
{
  float out =0;
  
  for(int j = 0;j<length_;++j)
  {
    out += (data_[j]-y.data_[j])*(data_[j]-y.data_[j]);
  }
  return out;
}


Pair::~Pair(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
//   data_ = NULL;
}

Pair::Pair(float x,float y)
{
  data_[0] = x;
  data_[1] = y;
}
