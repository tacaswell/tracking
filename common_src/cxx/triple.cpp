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
#include "triple.h"
#include<iostream>

#include <math.h>


using namespace utilities;

const int Triple::length_ ;
//bool Triple::allow_dim_change_ = false;

void Triple::operator= (const Triple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}
float& Triple::operator[] (int i){
  if(i<0 || i>= length_)
    throw "Triple range error"; 		// place holder for proper error handling
  return data_[i];
}
const float& Triple::operator[] (int i) const{
  if(i<0 || i>= length_)
    throw "Triple range error"; 		// place holder for proper error handeling
  return data_[i];
}

Triple Triple::operator- (){
  Triple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -data_[j];
  return tmp;
}


Triple Triple::operator+ (const Triple& y) const{
  Triple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y.data_[j] + data_[j];
  return tmp;
}
Triple Triple::operator+ (const float y)const{
  Triple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = y + data_[j];
  return tmp;
}
void Triple::operator+= (const Triple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y.data_[j] ;

}
 
void Triple::operator+= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] += y ;

}


Triple Triple::operator- (const Triple& y) const{
  Triple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y.data_[j] + data_[j];
  return tmp;
}
Triple Triple::operator- (const float y) const{
  Triple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = -y + data_[j];
  return tmp;
}
void Triple::operator-= (const Triple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y.data_[j] ;

}
void Triple::operator-= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] -= y ;
}



Triple Triple::operator* (const Triple& y) const{
  Triple tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j]*y.data_[j] ;
  }
  return tmp;
}
Triple Triple::operator* (const float y) const{
  Triple tmp;
  for(int j = 0;j<length_; ++j){
    tmp.data_[j] = data_[j] * y ;
  }
  return tmp;
}
void Triple::operator*= (const Triple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y.data_[j] ;
}
void Triple::operator*= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] *= y ;
}




void Triple::operator/= (const Triple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y.data_[j] ;

}
void Triple::operator/= (const float y){
  for(int j = 0;j<length_; ++j)
    data_[j] /= y ;
}
Triple Triple::operator/ (const Triple& y) const{
  Triple tmp;
  for(int j = 0;j<length_; ++j)
    tmp.data_[j] = data_[j]/y.data_[j] ;
  return tmp;
}





void Triple::print() const{
  using std::cout;
  using std::endl;
  for(int j = 0;j<length_; ++j)
    cout<<data_[j] <<"\t";
  cout<<endl;
}


float Triple::magnitude_sqr() const{
  float tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*data_[j] ;
  return tmp;
}

float Triple::magnitude() const
{
  return sqrt(magnitude_sqr());
}

float Triple::dot(const Triple & y) const
{
  float tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j]*y.data_[j] ;
  return tmp;
}


float Triple::prod()const
{
  float tmp = 1;
  for(int j = 0;j<length_; ++j)
    tmp *= data_[j];
  return tmp;
  
}
float Triple::sum()const
{
  float tmp = 0;
  for(int j = 0;j<length_; ++j)
    tmp += data_[j];
  return tmp;
} 

void Triple::make_unit(){
  float tmp = magnitude();
  for(int j = 0;j<length_; ++j)
    data_[j] /= tmp ;
}

Triple Triple::direction() const{
  Triple tmp = (*this);
  tmp.make_unit();
  return tmp;

}

void Triple::clear(){
  for(int j = 0;j<length_; ++j)
    data_[j] =0 ;
}

Triple::Triple(){
  //  data_= (float*) new float[length_];
  for(int j = 0;j<length_; ++j)
    data_[j] = 0;
  //  std::cout<<"make"<< std::endl;

}

// tac 2009-07-17
// added 
Triple::Triple(const Triple& y){
  for(int j = 0;j<length_; ++j)
    data_[j] = y.data_[j];
}

// tac 2009-10-07
// added because the constructors and destructors of Triples are taking ~5% of run time
float Triple::dist_sqr(const Triple& y)const
{
  float out =0;
  
  for(int j = 0;j<length_;++j)
  {
    out += (data_[j]-y.data_[j])*(data_[j]-y.data_[j]);
  }
  return out;
}


Triple::~Triple(){
  //  std::cout<<"died"<< std::endl;
  // delete[] data_;
//   data_ = NULL;
}

Triple::Triple(float x,float y,float z)
{
  data_[0] = x;
  data_[1] = y;
  data_[2] = z;
  
}

Triple::Triple(const float * in )
{
  for( int j = 0;j<length_;++j)
    data_[j] = in[j];
}
