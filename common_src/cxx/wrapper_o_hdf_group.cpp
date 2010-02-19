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

#include "wrapper_o_hdf_group.h"
#include "H5Cpp.h"
#include "particle_base.h"
#include "particle_track.h"


#include "track_box.h"
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


using utilities::D_TYPE;

using tracking::particle;
const int Wrapper_o_hdf_group::csize_ = 400;


Wrapper_o_hdf_group::Wrapper_o_hdf_group(CommonFG * parent, const std::string & g_name,
					 std::set<D_TYPE> d_add,
					 int size,
					 int comp_num,
					 bool new_group
					 ):
  parent_(parent),comp_number_(comp_num),size_(size),new_group_(new_group),added_count_(0),d_types_add_(d_add)
{
  // make a new entry or open group entry in parent
  if(new_group)
    group_ = new Group(parent_->createGroup(g_name));
  else
    group_ = new Group(parent_->openGroup(g_name));
  

  
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
  
  if(new_group_)
    part_index = added_count_++;
  else
  {
    part_index = p_in->get_ind();
    added_count_++;
  }


  // do sanity checks
  if((added_count_>size_))
  {
    cout<<"part_count_ "<<added_count_<<'\t';
    cout<<"group_max_count_ "<<size_<<endl;
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
    case V_ERROR:
      throw "you have hit a type that is not defined in enum_utils::v_type";
      break;
    }

  }

    
}




void Wrapper_o_hdf_group::write_to_disk()
{
  // make sure the particle is closed
  if(added_count_ != size_)
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
      throw "should not have hit default";
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
