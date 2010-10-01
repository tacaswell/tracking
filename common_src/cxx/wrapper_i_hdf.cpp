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
#include <stdexcept>

#include "wrapper_i_hdf.h"
#include "master_box_t.h"
#include "particle_base.h"
#include "enum_utils.h"
#include "attr_list_hdf.h"
#include "md_store.h"

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

using utilities::Tuplef;
using utilities::Wrapper_i_hdf;
using utilities::D_TYPE;
using utilities::Attr_list_hdf;


using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;
using std::complex;
using std::invalid_argument;
using std::logic_error;
using std::runtime_error;
using std::pair;


using tracking::particle;



Wrapper_i_hdf::Wrapper_i_hdf(std::string fname,
			     const std::set<utilities::D_TYPE> &dtypes,
			     int comp_number,
			     unsigned int start,
			     int f_count)
  :  file_name_(fname),data_types_set_(dtypes), total_part_count_(0),two_d_data_(true),start_(start)
{
  make_dtype_pairs(comp_number);
  priv_init(f_count);
  
}
Wrapper_i_hdf::Wrapper_i_hdf(std::string fname,
			     const std::set<utilities::D_TYPE> &dtypes,
			     int comp_number,
			     unsigned int start,
			     int f_count,
			     bool two_d_data)
  :  file_name_(fname),data_types_set_(dtypes),total_part_count_(0),two_d_data_(two_d_data),start_(start)
{
  make_dtype_pairs(comp_number);
  priv_init(f_count);
  
}


Wrapper_i_hdf::Wrapper_i_hdf(std::string fname,
	      const std::set<std::pair<utilities::D_TYPE,int > > &dtypes,
	      unsigned int start,
	      int f_count,
	      bool two_d_data)
   :  file_name_(fname),data_types_(dtypes),total_part_count_(0),two_d_data_(two_d_data),start_(start)
{
  make_dtype_set();
  priv_init(f_count);
  
}


void Wrapper_i_hdf::make_dtype_pairs(int comp_nuber)
{
  for(set<D_TYPE>::const_iterator it = data_types_set_.begin();
      it != data_types_set_.end(); ++it)
    data_types_.insert(pair<D_TYPE,int>(*it,comp_nuber));
}


void Wrapper_i_hdf::make_dtype_set()
{
  for(set<pair<D_TYPE, int> >::const_iterator it = data_types_.begin();
      it != data_types_.end(); ++it)
	data_types_set_.insert((*it).first);
}





void Wrapper_i_hdf::priv_init(int fr_count)
{ 
  
  try
  {
    
    
    H5File * file = new H5File( file_name_, H5F_ACC_RDONLY );  
    if(two_d_data_)
    {
      const string nop_str = "number-of-planes";
      Group g = file->openGroup("/");
      Attr_list_hdf atr_list(&g);
      if(!atr_list.contains_attr(nop_str))
	throw logic_error("wrapper_i_hdf: number-of-planes not found in file");
      atr_list.get_value(nop_str,frame_count_);
    }
    else
    {
      /**
	 @todo deal with this better, don't have any data 
       */
      frame_count_ = 1;
    }
    

    

    if(fr_count != 0)
    {
      if(fr_count + start_ > frame_count_)
	throw runtime_error("wrapper_i_hdf: asking for too many frames");
      frame_count_ = fr_count;
      
    }
    
    
    
    // logic to set up data maps and data storage
    int i_count =0;
    int f_count =0;
    int c_count =0;
    
    for(set<pair<D_TYPE,int> >::iterator it = data_types_.begin();
	it != data_types_.end();++it)
    {
      D_TYPE cur = (*it).first;
      
      switch(v_type(cur))
      {
      case V_INT:
	data_i_.push_back(vector<int*>(frame_count_));
	d_mapi_.set_lookup(cur,i_count++);
	break;
      case V_FLOAT:
	data_f_.push_back(vector<float*>(frame_count_));
	d_mapf_.set_lookup(cur,f_count++);
	break;
      case V_COMPLEX:
	data_c_.push_back(vector<complex<float>*>(frame_count_));
	d_mapc_.set_lookup(cur,c_count++);
	break;
      case V_STRING:
      case V_BOOL:
      case V_GUID:
      case V_TIME:
      case V_UINT:
      case V_ERROR:
	throw logic_error("wrapper_i_hdf: The data type should not have been " + VT2str_s(v_type(cur)));
      }
    }

    frame_c_.reserve(frame_count_);
    if(two_d_data_)
      frame_zdata_.resize(frame_count_);
    

    // set the size of the md_store
    set_Md_store_size(frame_count_);
    
    // fill in data
    // assume that the frames run from 0 to frame_count_
    for(unsigned int j = 0; j<frame_count_;++j)
    {
      string frame_name = format_name(j+start_);
      Group * frame = new Group(file->openGroup(frame_name));
      
      Attr_list_hdf g_attr_list(frame);
      
      set_Md_store(j,new Md_store(g_attr_list));
      
      
      if(two_d_data_)
      {
	if(!g_attr_list.contains_attr("z-position"))
	  throw logic_error("wrapper_i_hdf: z-position not found");
	g_attr_list.get_value("z-position",frame_zdata_[j]);
      }
      
      
      for(set<pair<D_TYPE,int> >::iterator it = data_types_.begin();
	  it != data_types_.end();++it)
      {
	
	if(two_d_data_ && ((*it).first)==utilities::D_ZPOS)
	  continue;

	// ***************	
	DataSet * dset = new DataSet(frame->openDataSet(format_dset_name((*it).first,(*it).second)));
	// ***************	
	DataSpace dspace = dset-> getSpace();
	dspace.selectAll();
	int part_count = dspace.getSimpleExtentNpoints();
	
	// if the first data set for this frame set the number of particles
	if(frame_c_.size()==j)
	  frame_c_.push_back(part_count);
	// if the part_count is less than a previous dataset, set the
	// number of particles to be the smaller number.  This
	// shouldn't result in memory leaks as the bare arrays are
	// never returned
	else if(frame_c_.at(j) > part_count)
	  frame_c_.at(j) = part_count;
	// if the current set has more than a previous set, keep the
	// old value.  these checks are a kludge, need to deal with
	// this better at the level of writing out the data
	else if(frame_c_.at(j) < part_count)
	  continue;
	// if(frame_c_.at(j) != part_count)
	//   throw runtime_error("wrapper_i_hdf: data sets different sizes");
	D_TYPE cur_type = (*it).first;
	
	switch(v_type(cur_type))
	{
	case V_INT:
	  data_i_.at(d_mapi_(cur_type)).at(j) = new int [part_count];
	  dset->read(data_i_.at(d_mapi_(cur_type)).at(j),PredType::NATIVE_INT);
	  
	  break;
	case V_FLOAT:
	  data_f_.at(d_mapf_(cur_type)).at(j) = new float [part_count];
	  dset->read(data_f_.at(d_mapf_(cur_type)).at(j),PredType::NATIVE_FLOAT);
	  break;
	case V_COMPLEX:
	  throw logic_error("not implemented yet");
	  
	  break;
	case V_STRING:
	case V_BOOL:
	case V_GUID:
	case V_TIME:
	case V_UINT:
	case V_ERROR:
	  throw logic_error("wrapper_i_hdf: The data type should not have been " + VT2str_s(v_type(cur_type)));
      	}
	
	// clean up hdf stuff
	dset->close();
	delete dset;
	dset = NULL;
      }
      frame->close();
      
      delete frame;
      frame = NULL;
      
    }
    file->close();
    

    delete file;
    
    file = NULL;
    
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
    
    throw runtime_error("wrapper_i_hdf: constructor error");
    
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
    throw runtime_error("wrapper_i_hdf: wrapper does not contain " + DT2str_s(type));
  
  
  if(v_type(type) != V_INT)
    throw runtime_error("wrapper_i_hdf: wrong data type, not int");
  out = data_i_.at(d_mapi_(type)).at(frame)[ind]    ;
  

  return out;
}

float Wrapper_i_hdf::get_value(float& out,
			       int ind,D_TYPE type, int frame) const 
{
  if(!contains_type(type))
    throw runtime_error("wrapper_i_hdf: wrapper does not contain " + DT2str_s(type));
  
  
  if(v_type(type) != V_FLOAT)
    throw runtime_error("wrapper_i_hdf: wrong data type, not float");


  
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
    throw runtime_error("wrapper_i_hdf: wrapper does not contain " + DT2str_s(type));
  
  if(v_type(type) != V_COMPLEX)
    throw runtime_error("wrapper_i_hdf: wrong data type, not complex");
  out = data_c_.at(d_mapc_(type)).at(frame)[ind]    ;
  

  return out;
}







std::set<D_TYPE>Wrapper_i_hdf::get_data_types() const 
{
  return set<D_TYPE>(data_types_set_);
}
int Wrapper_i_hdf::get_num_entries(unsigned int frame)const
{
  return frame_c_.at(frame);
}

int Wrapper_i_hdf::get_num_entries()const
{
  return total_part_count_;
}



int Wrapper_i_hdf::get_num_frames() const
{
  return frame_count_;
}
      

bool Wrapper_i_hdf::contains_type(utilities::D_TYPE in) const
{
  set<D_TYPE>::iterator it = data_types_set_.find(in);
  return it != data_types_set_.end();
}


string Wrapper_i_hdf::format_name(int in)
{
  std::ostringstream o;
  o.width(6);
  o.fill('0');
  o<<std::right<<in;
  return  "frame" + o.str();
}


Tuplef Wrapper_i_hdf::get_dims()const
{
  
  Tuplef tmp;
  

  H5File file =  H5File( file_name_, H5F_ACC_RDONLY );  
  Group  group = Group(file.openGroup("/"));

  Attr_list_hdf attr_list(&group);

  if(!attr_list.contains_attr("version"))
    throw invalid_argument("input file does not have a version");
  
  int file_version = 0;
  if(attr_list.get_value("version",file_version) != 1)
    throw invalid_argument("input file is not the right version");

  if(attr_list.contains_attr("dims"))
  {
    attr_list.get_value("dims",tmp);
  }
  group.close();
  file.close();
  
    

  return Tuplef(tmp);
  
}

float Wrapper_i_hdf::get_xy_scale() const
{

  /**
     /todo make it actually ask the hdf for the calibration,
   */
  return (6.45/60);

  // in principle this should ask the hdf file for the calibration
  // and do something clever, but this is a hack

  // float scale;
  // H5File file = H5File( file_name_, H5F_ACC_RDONLY );  
  // Group  group = Group(file.openGroup("/frame000000"));
  // Attr_list_hdf attr_list(&group);
  
  // if(attr_list.contains_attr("spatial-calibration-x"))
  // {
  //   attr_list.get_value("spatial-calibration-x",scale);
  // }
  // return(scale);
  
}
