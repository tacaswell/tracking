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
using utilities::Wrapper_i_hdf;
using utilities::D_TYPE;


using std::cout;
using std::endl;
using std::string;
using std::set;
using std::vector;


using tracking::particle_base;

Wrapper_i_hdf::Wrapper_i_hdf(std::string fname,std::set<utilities::D_TYPE> dtypes)
   :  file_name_(fname),data_types_(dtypes)
{
  
  
  try
  {
    H5File * file = new H5File( file_name_, H5F_ACC_RDONLY );  

    frame_count_ = file->getNumObjs();
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
	  d_mapi_.set_lookup(*it,i_count);
	  
	  break;
	case V_FLOAT:
	  break;
	case V_COMPLEX:
	  break;
	  
	}
	
    }
  

    // examine the file, sort out the number of everything

    // fill in data
    
    // clean up hdf stuff
  }
  catch(...)
  {
    // clean up data if it died
    throw "wrapper_i_hdf: constructor error";
    
  }
  

  

}

Wrapper_i_hdf::~Wrapper_i_hdf()
{
  // clean up data
}












bool Wrapper_i_hdf::contains_type(utilities::D_TYPE in) const
{
  set<D_TYPE>::iterator it = data_types_.find(in);
  return it != data_types_.end();
}

std::set<D_TYPE>Wrapper_i_hdf::get_data_types() const 
{
  return set<D_TYPE>(data_types_);
}
