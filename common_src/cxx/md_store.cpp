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



#include "md_store.h"
#include "enum_utils.h"

#include <string>
#include <stdexcept>

#include <iostream>
#include <sstream>


#include <unistd.h>
#include <errno.h>


using utilities::Md_store;


using std::string;
using std::vector;

using std::cerr;
using std::cout;
using std::endl;

using std::logic_error;


//taken from
//http://www.codeguru.com/forum/showthread.php?t=231054
// the third parameter of from_string() should be 
// one of std::hex, std::dec or std::oct
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

template <class T>
std::string to_string(T in)
{
  std::ostringstream o;
  o<<in;
  return o.str();
}


void Md_store::add_element(std::string & key,std::string & type, std::string & value)
{
  entries_.push_back(Md_element(key,type,value));
}

void Md_store::add_element(const char * key,const char * type, const char * value)
{
  entries_.push_back(Md_element(key,type,value));
}

void Md_store::add_element(const char * key,float val)
{
  entries_.push_back(Md_element(key,"float",to_string(val).c_str()));
}

void Md_store::add_element(const char * key,int val)
{
  entries_.push_back(Md_element(key,"int",to_string(val).c_str()));
}


void Md_store::print() const
{
  for(unsigned int j = 0; j<entries_.size();++j)
    cout<<'('<<entries_[j].key<<','<<entries_[j].type<<','<<entries_[j].value<<')'<<endl;
}

  
Md_store::~Md_store()
{
}

int Md_store::get_key_index(const string& key) const
{
  int max = entries_.size();
  for(int j = 0;j<max;++j)
    if(entries_[j].key.compare(key) == 0)
      return j;
  throw logic_error("Md_store::does not contain key: " + key);
}


bool Md_store::contains_key(const string& key) const
{
  int max = entries_.size();
  for(int j = 0;j<max;++j)
    if(entries_[j].key.compare(key) ==0)
      return true;
  
  return false;
}

float Md_store::get_value(int j,float & val)const
{
  if(str2VT_s(entries_[j].type) == utilities::V_FLOAT)
  {
    if( from_string<float> (val,entries_[j].value,std::dec))
      return val;
    else
      throw logic_error("Md_store:: failure to parse " + entries_[j].key);
  }
  else
    throw logic_error("Read_congig::get_value, expect pram of type: float, found type: " + entries_[j].type);
}


int Md_store::get_value(int j,int & val)const
{
  
  if(str2VT_s(entries_[j].type) == utilities::V_INT)
    if(from_string<int> (val,entries_[j].value,std::dec))
      return val;
    else 
      throw logic_error("Md_store:: failure to parse " + entries_[j].key);
  else
    throw logic_error("Read_congig::get_value, expect pram of type: int, found type: " + entries_[j].type);
}



string Md_store::get_value(int j,string & val)const
{
  
  if(str2VT_s(entries_[j].type) == utilities::V_STRING)
  {
    val = entries_[j].value;
    return val;
  }
  else
    throw logic_error("Read_congig::get_value, expect pram of type: string, found type: " + entries_[j].type);
}

template <class T>
T Md_store::get_value(const string& key,T & val)const
{
  int j = get_key_index(key);
  return get_value(j,val);
  
}

template int Md_store::get_value(const string& key,int & val)const;
template float Md_store::get_value(const string& key,float & val)const;
template string Md_store::get_value(const string& key,string & val)const;

