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


#include <iostream>
#include <sstream>
#include <stdexcept>
#include "attr_list_hdf.h"
#include "wrapper_o_hdf_group.h"
#include "H5Cpp.h"

#include "particle_track.h"


#include "track_box.h"

#include "md_store.h"
//using namespace H5;


using utilities::Wrapper_o_hdf_group;


using std::string;
using std::set;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::complex;
using std::logic_error;
using std::runtime_error;

using H5::H5File;
using H5::CommonFG;
using H5::Group;
using H5::DataSpace;
using H5::H5File;
using H5::Attribute;
using H5::PredType;
using H5::CompType;
using H5::DataSet;
using H5::DSetCreatPropList;
using H5::Exception;


using utilities::D_TYPE;
using utilities::MD_store;
using utilities::Tuple;

using tracking::particle;
const int Wrapper_o_hdf_group::csize_ = 400;


Wrapper_o_hdf_group::Wrapper_o_hdf_group(CommonFG * parent, const std::string & g_name,
					 std::set<D_TYPE> d_add,
					 int p_count,
					 int size,
					 int comp_num,
					 GROUP_T type
					 ):
  parent_(parent),comp_number_(comp_num),p_count_(p_count),size_(size),added_count_(0),group_type_(type),d_types_add_(d_add)
{

  try
  {
    

    // make a new entry or open group entry in parent
    if(type==NEW_GROUP)
      group_ = new Group(parent_->createGroup(g_name));
    else
      group_ = new Group(parent_->openGroup(g_name));
  }
  catch(Exception &e)
  {
    cerr<<"trouble opening/making the group in wrapper_o_hdf_group"<<endl;
    e.printError();
    throw e;
    
  }
  

  
  int_map_   = Data_map();
  float_map_ = Data_map();
  complex_map_=Data_map();
  dset_map_   =Data_map();

  int i_c=0,f_c=0,c_c = 0,d_c = 0;

  
  // for complex data types
  CompType ctype(sizeof(complex_t));
  ctype.insertMember("re",HOFFSET(complex_t,re),PredType::NATIVE_FLOAT);
  ctype.insertMember("im",HOFFSET(complex_t,im),PredType::NATIVE_FLOAT);
 
  // set up property lists for data sets     
  float fillvalue_f = -1234;
  DSetCreatPropList plist_f;
  plist_f.setFillValue(PredType::NATIVE_FLOAT,&fillvalue_f);
    

  int fillvalue_i = -1234;
  DSetCreatPropList plist_i;
  plist_i.setFillValue(PredType::NATIVE_INT,&fillvalue_i);
    
    
  complex_t fillvalue_c;
  fillvalue_c.im = -1234;
  fillvalue_c.re = -1234;
  DSetCreatPropList plist_c;
  plist_c.setFillValue(ctype,&fillvalue_c);
    

  if(size_ > csize_*5)
  {
    hsize_t tmp = csize_;
    plist_f.setChunk(1,&tmp);
    plist_f.setDeflate(9);

    plist_i.setChunk(1,&tmp);
    plist_i.setDeflate(9);

    tmp = csize_/2;
      
    plist_c.setChunk(1,&tmp);
    plist_c.setDeflate(9);
  }
	

  // set up data space
  hsize_t dim  = size_;
  DataSpace space(1,&dim);
  
  
  

   
  // go through add set and allocate arrays for storing data and make data sets
  for(set<D_TYPE>::const_iterator it = d_types_add_.begin(); 
      it!=d_types_add_.end();++it)
  { 
    switch(v_type(*it))
    {
    case V_INT:
      int_data_.push_back(new int[size_]);
      dsets_.push_back(new DataSet(group_->createDataSet(format_dset_name(*it,comp_number_),PredType::NATIVE_INT,space,plist_i)));
      int_map_.set_lookup(*it,i_c++);
      break;
    case V_FLOAT:
      float_data_.push_back(new float[size_]);
      dsets_.push_back( new DataSet(group_->createDataSet(format_dset_name(*it,comp_number_),PredType::NATIVE_FLOAT,space,plist_f)));
      float_map_.set_lookup(*it,f_c++);
      break;
    case V_COMPLEX:
      complex_data_.push_back(new complex_t[size_]);
      dsets_.push_back( new DataSet(group_->createDataSet(format_dset_name(*it,comp_number_),ctype,space,plist_c)));
      complex_map_.set_lookup(*it,c_c++);
      break;
    case V_STRING:
      throw "type should not be string";
    case V_ERROR:
      throw "type not registered, wrapper_o_hdf";
    }
    
    dset_map_.set_lookup(*it,d_c++);
  }
}


void Wrapper_o_hdf_group::store_particle(const particle * p_in)
{
  int part_index = -1;
  

  // set particle index
  
  if(group_type_ == APPEND_GROUP)
  {
    part_index = p_in->get_ind();
    ++added_count_;
  }
  else
    part_index = added_count_++;



  // do sanity checks
  if((added_count_>p_count_))
  {
    cout<<"part_count_ "<<added_count_<<'\t';
    cout<<"group_max_count_ "<<p_count_<<endl;
    cout<<"wrapper_size "<<size_<<endl;
    throw logic_error("wrapper_o_hdf: trying to add too many particles to group");
  }
  if(part_index == -1)
  {
    cout<<part_index<<endl;
    cout<<added_count_<<endl;
    
    throw "wrapper_o_hdf_group: something wrong, index = -1";
  
  }
  
  // set values to temp storage
  for(set<D_TYPE>::const_iterator current_type = d_types_add_.begin();
      current_type!=d_types_add_.end();++current_type)
  {
    //  set_value(*current_type,p_in);
    D_TYPE type = *current_type;
    
    int tmpi = 0;
    float tmpf = 0;
    complex<float> tmpc(0,0);
    switch(v_type(type))
    {
    case V_INT:
      p_in->get_value(type,tmpi);
      int_data_.at(int_map_(type))[part_index] = tmpi;
      break;
    case V_FLOAT:
      p_in->get_value(type,tmpf);
      float_data_.at(float_map_(type))[part_index] = tmpf;
      break;
    case V_COMPLEX:
      p_in->get_value(type,tmpc);
      complex_t tmp_val;
      tmp_val.re = tmpc.real();
      tmp_val.im = tmpc.imag();
      complex_data_.at(complex_map_(type))[part_index] =tmp_val;
      break;
    case V_STRING:
      throw "type should not be string";
    case V_ERROR:
      throw "you have hit a type that is not defined in enum_utils::v_type";
      break;
    }

  }

    
}

void Wrapper_o_hdf_group::store_particle_pos(const Tuple<float,3> & cord_in,float I)
{
  // check to make sure group mode is appropriate 
  if(group_type_ != NEW_GROUP)
    throw logic_error("store_particle_pos:: incompatible group_type");
  
  // some sanity check to make sure that this is enough information to fill the wrapper
  int part_index = added_count_++;
  
  
  
  // float testf = cord_in[0];
  // const utilities::Tuple<int,3> test(1,2,3);
  // test.print();
  // testf = test[0];
  
  
  if((added_count_>p_count_))
  {
    cout<<"part_count_ "<<added_count_<<'\t';
    cout<<"group_max_count_ "<<p_count_<<endl;
    throw logic_error("wrapper_o_hdf: trying to add too many particles to group");
  }
  // set values to temp storage
  for(set<D_TYPE>::const_iterator current_type = d_types_add_.begin();
      current_type!=d_types_add_.end();++current_type)
  {
    //  set_value(*current_type,p_in);
    D_TYPE type = *current_type;
    
    
    complex<float> tmpc(0,0);
    switch(type)
    {
    case D_XPOS:
      float_data_.at(float_map_(type))[part_index] = cord_in[0];
      break;
    case D_YPOS:
      float_data_.at(float_map_(type))[part_index] = cord_in[1];
      break;
    case D_ZPOS:
      float_data_.at(float_map_(type))[part_index] = cord_in[2];
      break;
    case D_I:
      float_data_.at(float_map_(type))[part_index] = I;
      break;
    default:
      
      throw logic_error("wapper_o_hdf_group::store_particle_pos \n\t Can not use this function with any types but X,Y,Z,I");
    }

  }

  


}


void Wrapper_o_hdf_group::add_meta_store(const MD_store * md_in)
{
  if(!md_in)
    throw runtime_error("Wrapper_o_hdf_group::add_meta_store: MD_store pointer is null");
  
  Attr_list_hdf atrlst(group_);
  unsigned int num_entries = md_in->size();
  int tmpi;
  float tmpf;
  string tmps;
  for(unsigned int j = 0; j<num_entries; ++j)
  {
    string key = md_in->get_key(j);
    switch(md_in->get_type(j))
    {
      // integer data
    case V_INT:
    
      atrlst.set_value(key,md_in->get_value(j,tmpi));
      break;
      // float data
    case V_FLOAT:
    
      atrlst.set_value(key,md_in->get_value(j,tmpf));
      break;
      // complex data
    case V_STRING:
    
      atrlst.set_value(key,md_in->get_value(j,tmps));
      break;
    default:
      throw logic_error("should not have hit default");
      
    }
  

  }
}



void Wrapper_o_hdf_group::write_to_disk()
{
  // make sure the particle is closed
  if(added_count_ != p_count_)
    throw logic_error("not enough particles added");
  
    
  // for complex data types
  CompType ctype(sizeof(complex_t));
  ctype.insertMember("re",HOFFSET(complex_t,re),PredType::NATIVE_FLOAT);
  ctype.insertMember("im",HOFFSET(complex_t,im),PredType::NATIVE_FLOAT);



  
  for(set<D_TYPE>::const_iterator it = d_types_add_.begin(); 
      it!=d_types_add_.end();++it)
  {
    D_TYPE dtype = *it;
    DataSet * dset = dsets_.at(dset_map_(dtype));
    
    
    switch(v_type(*it))
    {
      // integer data
    case V_INT:
      dset->write(int_data_.at(int_map_(dtype)),PredType::NATIVE_INT);
      break;
      // float data
    case V_FLOAT:
      dset->write(float_data_.at(float_map_(dtype)),PredType::NATIVE_FLOAT);
      break;
      // complex data
    case V_COMPLEX:
      dset->write(complex_data_.at(complex_map_(dtype)),ctype);
	
      break;
    default:
      throw logic_error("should not have hit default");
    }
    
  }
}


Wrapper_o_hdf_group::~Wrapper_o_hdf_group()
{
      
  // clean up
  for(vector<int*>::iterator it = int_data_.begin();
      it != int_data_.end();++it)
    delete [] *it;
  int_data_.clear();
    
  for(vector<float*>::iterator it = float_data_.begin();
      it != float_data_.end();++it)
    delete [] *it;
  float_data_.clear();
    
  for(vector<complex_t*>::iterator it = complex_data_.begin();
      it != complex_data_.end();++it)
    delete [] *it;
  complex_data_.clear();

  for(vector<DataSet *>::iterator it = dsets_.begin();
      it != dsets_.end();++it)
    delete *it;
  dsets_.clear();
    
  delete group_;
  group_ = NULL;
    
}



template <class T>
void Wrapper_o_hdf_group::set_meta_data(const std::string & key, const T & val)
{
  Attr_list_hdf attr_list(group_);
  attr_list.set_value(key,val);
}
  
template <class T>
T Wrapper_o_hdf_group::get_meta_data(const std::string & key, T &  val) const 
{
  Attr_list_hdf attr_list(group_);
  return attr_list.get_value(key,val);
}
 
template void Wrapper_o_hdf_group::set_meta_data(const std::string & key, const int & val);
template void Wrapper_o_hdf_group::set_meta_data(const std::string & key, const float & val);
template void Wrapper_o_hdf_group::set_meta_data(const std::string & key, const string & val);
template void Wrapper_o_hdf_group::set_meta_data(const std::string & key, const Tuple<float,2> & val);
template void Wrapper_o_hdf_group::set_meta_data(const std::string & key, const Tuple<float,3> & val);

template int Wrapper_o_hdf_group::get_meta_data(const std::string & key, int & val) const;
template float Wrapper_o_hdf_group::get_meta_data(const std::string & key, float & val)const;
template string Wrapper_o_hdf_group::get_meta_data(const std::string & key, string & val)const;
template Tuple<float,3> 
Wrapper_o_hdf_group::get_meta_data(const std::string & key,  Tuple<float,3> & val)const;
template Tuple<float,2> 
Wrapper_o_hdf_group::get_meta_data(const std::string & key,  Tuple<float,2> & val)const;
