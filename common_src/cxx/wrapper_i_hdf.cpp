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

#include "wrapper_i_hdf.h"
#include "master_box_t.h"
#include "pair.h"
#include "triple.h"
#include "particle_base.h"
#include "enum_utils.h"

using H5::H5File;
using H5::Exception;
using H5::DataSpaceIException;
using H5::DataSetIException ;
using H5::FileIException;
using H5::Group;
using H5::DataSet;
using H5::DataSpace;
using H5::PredType;
using H5::Attribute;
using H5::Group;

using utilities::Tuple;
using utilities::Wrapper_i_hdf;
using utilities::D_TYPE;


using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;
using std::complex;


using tracking::particle;

Wrapper_i_hdf::Wrapper_i_hdf(std::string fname,const std::set<utilities::D_TYPE> &dtypes,int start,int f_count)
  :  file_name_(fname),data_types_(dtypes),total_part_count_(0),two_d_data_(true)
{
  priv_init(f_count,start);
  
}
Wrapper_i_hdf::Wrapper_i_hdf(std::string fname,const std::set<utilities::D_TYPE> &dtypes,int start,int f_count,bool two_d_data)
  :  file_name_(fname),data_types_(dtypes),total_part_count_(0),two_d_data_(two_d_data)
{
  priv_init(f_count,start);
  
}


void Wrapper_i_hdf::priv_init(int f_count,unsigned int start)
{
  if (start <0)
    throw "wrapper_i_hdf: start must be positive";
  
  
  try
  {
    
    
    H5File * file = new H5File( file_name_, H5F_ACC_RDONLY );  
    if(two_d_data_)
    {
      Group * group = new Group(file->openGroup("/"));
      Attribute * tmpa =  new Attribute(group->openAttribute("number-of-planes"));
      tmpa->read(PredType::NATIVE_INT,&frame_count_);
      delete tmpa;
    }
    else
    {
      frame_count_ = 1;
    }
    

    if(f_count != 0)
    {
      if(f_count + start > frame_count_)
	throw "wrapper_i_hdf: asking for too many frames";
      frame_count_ = f_count;
      
    }
    
    
    
    // logic to set up data maps and data storage
    int i_count =0;
    int f_count =0;
    int c_count =0;
    
    for(set<D_TYPE>::iterator it = data_types_.begin();
	it != data_types_.end();++it)
    {
      switch(v_type(*it))
      {
      case V_INT:
	data_i_.push_back(vector<int*>(frame_count_));
	d_mapi_.set_lookup(*it,i_count++);
	break;
      case V_FLOAT:
	data_f_.push_back(vector<float*>(frame_count_));
	d_mapf_.set_lookup(*it,f_count++);
	break;
      case V_COMPLEX:
	data_c_.push_back(vector<complex<float>*>(frame_count_));
	d_mapc_.set_lookup(*it,c_count++);
	break;
      case V_ERROR:
	throw "wrapper_i_hdf: I have no idea why the type was error";
	break;
	
      }
    }

    frame_c_.reserve(frame_count_);
    if(two_d_data_)
      frame_zdata_.resize(frame_count_);
    
    
    // fill in data
    // assume that the frames run from 0 to frame_count_

    
    for(unsigned int j = 0; j<frame_count_;++j)
    {
      string frame_name = format_name(j+start);
      Group * frame = new Group(file->openGroup(frame_name));
      
      if(two_d_data_)
      {

	
	Attribute * tmpa =  new Attribute(frame->openAttribute("z-position"));
	tmpa->read(PredType::NATIVE_FLOAT,&frame_zdata_[j]);
	delete tmpa;
      }
      
      
      for(set<D_TYPE>::iterator it = data_types_.begin();
	  it != data_types_.end();++it)
      {
	
	if(two_d_data_ && (*it)==utilities::D_ZPOS)
	  continue;

	
	DataSet * dset = new DataSet(frame->openDataSet(DT2str_s(*it)));
	DataSpace dspace = dset-> getSpace();
	dspace.selectAll();
	int part_count = dspace.getSimpleExtentNpoints();
	
	if(frame_c_.size()==j)
	  frame_c_.push_back(part_count);
	else if(frame_c_.at(j) != part_count)
	  throw "wrapper_i_hdf: data sets different sizes";
	
	
	switch(v_type(*it))
	{
	case V_INT:
	  data_i_.at(d_mapi_(*it)).at(j) = new int [part_count];
	  dset->read(data_i_.at(d_mapi_(*it)).at(j),PredType::NATIVE_INT);
	  
	  break;
	case V_FLOAT:
	  data_f_.at(d_mapf_(*it)).at(j) = new float [part_count];
	  dset->read(data_f_.at(d_mapf_(*it)).at(j),PredType::NATIVE_FLOAT);
	  break;
	case V_COMPLEX:
	  throw "not implemented yet";
	  
	  break;
	case V_ERROR:
	  throw "wrapper_i_hdf: I have no idea why the type was error";
	  break;
	}
	// clean up hdf stuff
	delete dset;
      }
      delete frame;
      
    }
    
    delete file;

    // shift all of the z by the minimum to start at zero
    if(two_d_data_)
    {
      float min = frame_zdata_[0];
      for(unsigned int j = 0; j<frame_count_;++j)
	if(frame_zdata_[j]<min)
	  min = frame_zdata_[j];
      for(unsigned int j = 0; j<frame_count_;++j)
	frame_zdata_[j] -= min ;
    }
    
  }
  catch(Exception & e)
  {
    // clean up data if it died
    e.printError();
    
    throw "wrapper_i_hdf: constructor error";
    
  }
  
  for(unsigned int j= 0; j<frame_count_;++j)
    total_part_count_ += frame_c_.at(j);
}


Wrapper_i_hdf::~Wrapper_i_hdf()
{
  // clean up data
  clean_data();
  
}

void Wrapper_i_hdf::clean_data()
{
    vector<std::vector<int*> >::iterator i_o_end = data_i_.end();
  for(vector<std::vector<int*> >::iterator outer = data_i_.begin();
      outer != i_o_end; ++outer)
  {
    vector<int *>::iterator i_i_end = (*outer).end();
    for(vector<int *>::iterator inner = (*outer).begin();
	inner!=i_i_end;++inner)
    {
      delete [] *inner;
      *inner = NULL;
      
    }
  }
  vector<std::vector<float*> >::iterator f_o_end = data_f_.end();
  for(vector<vector<float*> >::iterator outer = data_f_.begin();
      outer != f_o_end; ++outer)
  {
    vector<float *>::iterator f_i_end = (*outer).end();
    for(vector<float *>::iterator inner = (*outer).begin();
	inner!=f_i_end;++inner)
    {
      delete [] *inner;
      *inner = NULL;
    }
  }
  vector<vector<complex<float>*> >::iterator c_o_end = data_c_.end();
  for(vector<vector<complex<float>*> >::iterator outer = data_c_.begin();
      outer != c_o_end; ++outer)
  {
    vector<complex<float> *>::iterator c_i_end = (*outer).end();
    for(vector<complex<float> *>::iterator inner = (*outer).begin();
	inner!=c_i_end;++inner)
    {
      delete [] *inner;
      *inner = NULL;
    }
  }
}







int Wrapper_i_hdf::get_value(int& out,
			     int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw "wrapper_i_hdf: wrapper does not contain this type";
  
  
  if(v_type(type) != V_INT)
    throw "wrapper_i_hdf: wrong data type, not int";
  out = data_i_.at(d_mapi_(type)).at(frame)[ind]    ;
  

  return out;
}

float Wrapper_i_hdf::get_value(float& out,
			       int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw "wrapper_i_hdf: wrapper does not contain this type";
  
  if(v_type(type) != V_FLOAT)
    throw "wrapper_i_hdf: wrong data type, not float";


  
  if(type == utilities::D_ZPOS && two_d_data_)
  {
    out = frame_zdata_.at(frame);
  }
  
  else
  {
    out = data_f_.at(d_mapf_(type)).at(frame)[ind]    ;
  }
  

  return out;
}

std::complex<float> Wrapper_i_hdf::get_value(std::complex<float>& out,
					     int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw "wrapper_i_hdf: wrapper does not contain this type";
  
  if(v_type(type) != V_COMPLEX)
    throw "wrapper_i_hdf: wrong data type, not complex";
  out = data_c_.at(d_mapc_(type)).at(frame)[ind]    ;
  

  return out;
}







std::set<D_TYPE>Wrapper_i_hdf::get_data_types() const 
{
  return set<D_TYPE>(data_types_);
}
int Wrapper_i_hdf::get_num_entries(int frame)const
{
  if(frame == -1)
    return total_part_count_;
  else
    return frame_c_.at(frame);
}


int Wrapper_i_hdf::get_num_frames() const
{
  return frame_count_;
}
      

bool Wrapper_i_hdf::contains_type(utilities::D_TYPE in) const
{
  set<D_TYPE>::iterator it = data_types_.find(in);
  return it != data_types_.end();
}


string Wrapper_i_hdf::format_name(int in)
{
  std::ostringstream o;
  o.width(6);
  o.fill('0');
  o<<std::right<<in;
  return  "frame" + o.str();
}


Tuple Wrapper_i_hdf::get_dims()const
{


  H5File * file = new H5File( file_name_, H5F_ACC_RDONLY );  
  Group * group = new Group(file->openGroup("/"));
  
  Attribute * tmpa =  new Attribute(group->openAttribute("dims"));
  float tmp[Tuple::length_] ;

  
  tmpa->read(PredType::NATIVE_FLOAT,tmp);
  delete tmpa;

  return Tuple(tmp);
  
}

float Wrapper_i_hdf::get_xy_scale() const
{

  
  H5File * file = new H5File( file_name_, H5F_ACC_RDONLY );  
  Group * group = new Group(file->openGroup("/frame000000"));
  Attribute * tmpa =  new Attribute(group->openAttribute("spatial-calibration-x"));
  float scale;
  
  tmpa->read(PredType::NATIVE_FLOAT,&scale);
  delete tmpa;
  
  return(scale);
  
}
