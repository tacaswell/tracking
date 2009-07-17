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

using H5::H5File;
using H5::DataSpaceIException;
using H5::DataSetIException ;
using H5::FileIException;
using H5::Group;
using H5::DataSet;
using H5::DataSpace;
using namespace tracking;


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
