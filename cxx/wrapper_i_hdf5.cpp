//Copyright 2008,2009 Thomas A Caswell
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
#include "particle_track.h"

using H5::H5File;
using H5::DataSpaceIException;
using H5::DataSetIException ;
using H5::FileIException;
using H5::Group;
using H5::DataSet;
using H5::DataSpace;
using H5::PredType;

using utilities::Tuple;


using namespace tracking;

const int BUFF_LEN = 50;

Wrapper_i_hdf5::Wrapper_i_hdf5(map<wrapper::p_vals, int> wrap):
  wrapper_i_base(wrap),file_name_("processed_30um_27-2_0.h5")
{
  
}
double Wrapper_i_hdf5::get_value(int, tracking::wrapper::p_vals) const
{
  
}



void Wrapper_i_hdf5::print()const
{
  cout<<file_name_<<endl;
}



int Wrapper_i_hdf5::num_entries() const
{
      
  H5File* file = new H5File( file_name_, H5F_ACC_RDONLY );
  Group*  group;
  
  
  int total_part = 0;
    

    for(unsigned int j = 0; j<file->getNumObjs();++j)
    {

      
      group = new Group(file->openGroup(file->getObjnameByIdx(j)));

      total_part += ((group->openDataSet("x")).getSpace()).getSimpleExtentNpoints();
    
      
      delete group;
      group = NULL;
      
    
    }

    delete file;
    return total_part;
    

}


void Wrapper_i_hdf5::fill_master_box(master_box_t<particle_track> & master_box) const
{
  std::set<wrapper::p_vals> data_type_tmp = this->get_data_types();
  
  particle_base::intialize_wrapper_in(this);
  particle_base::intialize_data_types(&data_type_tmp);
  // open file
  H5File* file = new H5File( file_name_, H5F_ACC_RDONLY );

  int number_of_frames =file->getNumObjs();

  number_of_frames = 25;
  
  // logic to check that the frames are well formed, match the pattern, etc


  
  for(int j = 0; j<number_of_frames; ++j)
  {
    char buffer [BUFF_LEN];
    sprintf(buffer,"frame%05d",j);
    
    Group * group = new Group(file->openGroup(buffer));
    DataSet * data_x = new DataSet(group->openDataSet("x"));
    DataSet * data_y = new DataSet(group->openDataSet("y"));
    int num_particles =  (data_x->getSpace()).getSimpleExtentNpoints();
    hsize_t tmp_dim = 1;
    
    DataSpace fspace(1,&tmp_dim);
    DataSpace mspace(1,&tmp_dim);
    

    
    for(int i = 0; i<num_particles;++i)
    {
      float x,y;
      hssize_t tmp_offset = i;
      
      fspace.offsetSimple(&tmp_offset);
      
      data_x->read(&x, PredType::NATIVE_FLOAT,mspace,fspace);
      data_y->read(&y, PredType::NATIVE_FLOAT,mspace,fspace);

      master_box.push(new particle_track(i,Tuple(x,y),j));
      

      //      read (void *buf, const DataType &mem_type, const DataSpace &mem_space=DataSpace::ALL, const DataSpace &file_space=DataSpace::ALL, const DSetMemXferPropList &xfer_plist=DSetMemXferPropList::DEFAULT) const
    }
    delete group;

    

  }
  



  delete file;
  

}
