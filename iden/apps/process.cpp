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
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting
#include <iostream>

#include "iden/wrapper_i_plu.h"
#include "iden/iden.h"
#include "iden/params1.h"

#include "master_box_t.h"
#include "particle_base.h"
#include "hash_case.h"
#include "tuple.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "H5Cpp.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

using std::cout;
using std::cerr;
using std::endl;
using std::set;
using std::string;

using utilities::Wrapper_i_plu;
using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Tuple;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;

using tracking::master_box_t;
using tracking::particle_base;
using tracking::hash_case;

using iden::Iden;
using iden::Params;

using H5::H5File;
using H5::Group;
using H5::Attribute;
using H5::PredType;

static string base_data_path = "/home/tcaswell/colloids/data/";
static string base_proc_path = "/home/tcaswell/colloids/processed/";


int main(int argc, const char * argv[])
{

  if(argc != 3)
  {
    cerr<< "wrong number of args args"<<endl;
    return 0;
  }
  
  
  string file_path = string(argv[1]);
  string file_name = string(argv[2]);
  
  string data_file = base_data_path + file_path + file_name + ".tif";
  string proc_file = base_proc_path + file_path + file_name + ".h5";

  cout<<data_file<<endl;
  cout<<proc_file<<endl;






  try
  {

    float hwhm,thresh;
    int feature_rad,dilation_rad, mask_rad,frame_c;
    {
      
      H5File * file = new H5File(proc_file,H5F_ACC_RDONLY);
      Group * group = new Group(file->openGroup("/"));

      Attribute * tmpa =  new Attribute(group->openAttribute("frame_count"));
      tmpa->read(PredType::NATIVE_INT,&frame_c);
      delete tmpa;
    
      tmpa = new Attribute(group->openAttribute("threshold"));
      tmpa->read(PredType::NATIVE_FLOAT,&thresh);
      delete tmpa;

      tmpa = new Attribute(group->openAttribute("p_rad"));
      tmpa->read(PredType::NATIVE_INT,&feature_rad);
      delete tmpa;

      tmpa = new Attribute(group->openAttribute("hwhm"));
      tmpa->read(PredType::NATIVE_FLOAT,&hwhm);
      delete tmpa;

      tmpa = new Attribute(group->openAttribute("d_rad"));
      tmpa->read(PredType::NATIVE_INT,&dilation_rad);
      delete tmpa;
    
      tmpa = new Attribute(group->openAttribute("mask_rad"));
      tmpa->read(PredType::NATIVE_INT,&mask_rad);
      delete tmpa;

      delete file;
      delete group;
      file= NULL;
      group=NULL;
      tmpa=NULL;
    }
    


    // replace these by reading the proc file
    
    Params p(feature_rad,hwhm,dilation_rad,thresh,mask_rad);
    p.PrintOutParameters(std::cout);
    cout<<frame_c<<endl;
    

  
    

    Iden iden(p);
    iden.set_fname(data_file);
    Wrapper_i_plu wp(1);
    
    iden.fill_wrapper(wp,frame_c,0);
    cout<<"number of entries in wrapper: "<<wp.get_num_entries(-1)<<endl;
    

    master_box_t<particle_base> box;
    //Filter_basic filt("25-0_mid_0.h5");
    Filter_trivial filt;
    
    box.init(wp,filt);
    //box.init(wh,filt);


    
    cout<<"total number of particles is: "<<box.size()<<endl;;
    



    
    for(int j =0;j<20;++j)
    {
      particle_base* p = box.get_particle(j);
      p->print();
    }
    

    
    Tuple dims(1392,520);
    hash_case hcase(box,dims,20,frame_c);
    //    hcase.print();
    
    //Wrapper_o_hdf hdf_w("25-0_mid_0.h5",wh.get_data_types());
    Wrapper_o_hdf hdf_w(proc_file,wp.get_data_types());
    
    try
    {
      hcase.output_to_wrapper(hdf_w);
    }
    catch(const char * err)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<err<<endl;
    }
    catch(...)
    {
      std::cerr<<"not right type"<<endl;
    }
    

    
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 
  
  return 0;
}

