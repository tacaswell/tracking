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

#include "wrapper_i_hdf5.h"
#include "master_box_t.h"
#include "tuple.h"
#include "particle_base.h"
#include "enum_utils.h"

using H5::H5File;
using H5::DataSpaceIException;
using H5::DataSetIException ;
using H5::FileIException;
using H5::Group;
using H5::DataSet;
using H5::DataSpace;
using H5::PredType;

using utilities::Tuple;
using utilities::Wrapper_i_hdf5;
using utilities::D_TYPE;


using std::cout;
using std::endl;
using std::string;
using std::set;

using tracking::master_box_t;
using tracking::particle;


const int BUFF_LEN = 50;

void Wrapper_i_hdf5::check_group(int frame)const
{
  
  if(!file_open_)
    throw "wrapper_i_hdf: file not open";
  
  if(frame != current_frame_)
  {
    delete current_group_;
    current_group_ = new Group(file_->openGroup(format_name(frame)));
    current_frame_ = frame;
  }
  
  
}

Wrapper_i_hdf5::Wrapper_i_hdf5(string fname,set<utilities::D_TYPE> d_types):
  file_name_(fname),data_types_(d_types)
{
  
}

int Wrapper_i_hdf5::get_value(int& out,
	      int ind,D_TYPE type, int frame) const
{
  check_group(frame);
  
  DataSet dset = current_group_->openDataSet(DT2str_s(type));
  DataSpace fspace = dset.getSpace();
  fspace.selectNone();
  hsize_t cord= ind;
  fspace.selectElements(H5S_SELECT_SET,1,&cord);
  hsize_t dims = 1;
  DataSpace mspace = DataSpace(1,&dims);
  
  dset.read(&out,PredType::NATIVE_INT,mspace,fspace);
  
  
  
  return out;
}

float Wrapper_i_hdf5::get_value(float& out,
		int ind,D_TYPE type, int frame) const 
{
  check_group(frame);
  
  DataSet dset = current_group_->openDataSet(DT2str_s(type));
  DataSpace fspace = dset.getSpace();
  fspace.selectNone();
  hsize_t cord= ind;
  fspace.selectElements(H5S_SELECT_SET,1,&cord);
  hsize_t dims = 1;
  DataSpace mspace = DataSpace(1,&dims);
  
  dset.read(&out,PredType::NATIVE_FLOAT,mspace,fspace);
  
  
  
  return out;
}

std::complex<float> Wrapper_i_hdf5::get_value(std::complex<float>& out,
			      int ind,D_TYPE type, int frame) const 
{
  
  throw "not implemented yet";
  

  

  return out;
}





int Wrapper_i_hdf5::get_num_entries(int frame) const
{
    
  if(!file_open_)
    throw "wrapper_i_hdf: file not open";

  if(frame ==-1)
  {
    Group*  group;
    int total_part = 0;
    int frame_count = file_->getNumObjs();
    
    for(unsigned int j = 0; j<frame_count;++j)
    {
      group = new Group(file_->openGroup(file_->getObjnameByIdx(j)));
      total_part += ((group->openDataSet("x")).getSpace()).getSimpleExtentNpoints();
      delete group;
      group = NULL;
    }
    return total_part;
  }
  
  if(frame != current_frame_)
  {
    delete current_group_;
    current_group_ = new Group(file_->openGroup(format_name(frame)));
    current_frame_ = frame;
  }
  return ((current_group_->openDataSet("x")).getSpace()).getSimpleExtentNpoints();
  
    

}

  
bool Wrapper_i_hdf5::contains_type(utilities::D_TYPE in) const
{
  set<D_TYPE>::iterator it = data_types_.find(in);
  return it != data_types_.end();
}

std::set<D_TYPE>Wrapper_i_hdf5::get_data_types() const 
{
  return set<D_TYPE>(data_types_);
}

int Wrapper_i_hdf5::get_num_frames()const
{
  if(!file_open_)
    throw "wrapper_i_hdf5: file not open";
  
  return file_->getNumObjs();
}

void Wrapper_i_hdf5::open_file()
{
  if(file_open_)
    throw "wrapper_i_hdf5: file already open";
  
  file_ = new H5File( file_name_, H5F_ACC_RDONLY );
  current_frame_ = -1;
  current_group_ = NULL;
  file_open_ = true;
}

void Wrapper_i_hdf5::close_file()
{
  if(!file_open_)
    return;
  
  
  delete current_group_;
  current_group_ = NULL;
  delete file_;
  file_ = NULL;
  current_frame_ = -1;
  file_open_ = false;
}

Wrapper_i_hdf5::~Wrapper_i_hdf5()
{
  close_file();
}



// void Wrapper_i_hdf5::fill_master_box(master_box_t<particle> & master_box) const
// {
//   std::set<utilities::D_TYPE> data_type_tmp = this->get_data_types();
  
//   particle::intialize_wrapper_in(this);
//   particle::intialize_data_types(&data_type_tmp);
//   // open file
//   H5File* file = new H5File( file_name_, H5F_ACC_RDONLY );

//   int number_of_frames =file->getNumObjs();

//   number_of_frames = 25;
  
//   // logic to check that the frames are well formed, match the pattern, etc


  
//   for(int j = 0; j<number_of_frames; ++j)
//   {
//     char buffer [BUFF_LEN];
//     sprintf(buffer,"frame%05d",j);
    
//     Group * group = new Group(file->openGroup(buffer));
//     DataSet * data_x = new DataSet(group->openDataSet("x"));
//     DataSet * data_y = new DataSet(group->openDataSet("y"));
//     int num_particles =  (data_x->getSpace()).getSimpleExtentNpoints();
//     hsize_t tmp_dim = 1;
    
//     DataSpace fspace(1,&tmp_dim);
//     DataSpace mspace(1,&tmp_dim);
    

    
//     for(int i = 0; i<num_particles;++i)
//     {
//       float x,y;
//       hssize_t tmp_offset = i;
      
//       fspace.offsetSimple(&tmp_offset);
      
//       data_x->read(&x, PredType::NATIVE_FLOAT,mspace,fspace);
//       data_y->read(&y, PredType::NATIVE_FLOAT,mspace,fspace);

//       master_box.push(new particle(i,Tuple(x,y),j));
      

//       //      read (void *buf, const DataType &mem_type, const DataSpace &mem_space=DataSpace::ALL, const DataSpace &file_space=DataSpace::ALL, const DSetMemXferPropList &xfer_plist=DSetMemXferPropList::DEFAULT) const
//     }
//     delete group;

    

//   }
  



//   delete file;
  

// }



string Wrapper_i_hdf5::format_name(int in)
{
  std::ostringstream o;
  o.width(6);
  o.fill('0');
  o<<std::right<<in;
  return  "frame" + o.str();
}
