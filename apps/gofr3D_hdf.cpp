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


#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot


#include <unistd.h>
#include <vector>
#include "read_config.h"
#include <stdexcept>

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::vector;
using std::logic_error;

using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Tuple;

using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;

using utilities::Read_config;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_gofr;
typedef Generic_wrapper_hdf::F_TYPE F_TYPE;

static string APP_NAME = "gofr3D :: ";

int main(int argc, char * const argv[])
{
  
  
  string in_file;
  string out_file;
  string pram_file ;
  int optchar;
  bool found_i=false,found_o= false,found_c = false;


  
    
  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'i':
      in_file = string(optarg);
      found_i = true;
      break;
    case 'o':
      out_file = string(optarg);
      found_o = true;
      break;
    case 'c':
      pram_file = string(optarg);
      found_c = true;
      break;
      
    case '?':
    default:
      cout<<"-i input file name"<<endl;
      cout<<"-o output file name"<<endl;
      cout<<"-c parameter file name"<<endl;
      break;
    }
  }

  if(!(found_i && found_o && found_c))
  {
    cerr<<"input failed"<<endl;
    cout<<"-i input filename"<<endl;
    cout<<"-o output filename"<<endl;
    cout<<"-c configuration filename"<<endl;
    return -1;
  }
  
  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;

  
      
  float max_range;
  int nbins;
  string grp_name;
  int write_comp_number,read_comp_number,dset_num;


  
  
  Read_config app_prams(pram_file,"gofr3D");
  if(!(app_prams.contains_key("max_range") &&
       app_prams.contains_key("nbins") &&
       app_prams.contains_key("grp_name")))
    throw logic_error(APP_NAME + " parameter file does not contain enough parameters");
  try
  {
    app_prams.get_value("max_range",max_range);
    app_prams.get_value("nbins",nbins);
    app_prams.get_value("grp_name",grp_name);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("read_comp")&&
       comp_prams.contains_key("dset")&&
       comp_prams.contains_key("write_comp")))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
  try
  {
    comp_prams.get_value("read_comp",read_comp_number);
    comp_prams.get_value("write_comp",write_comp_number);
    comp_prams.get_value("dset",dset_num);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }



  
  try
  {


     
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
		    utilities::D_ZPOS,
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+3);
    
    /// @todo these parameters need to be passed in
    Wrapper_i_hdf wh;
    bool suc = wh.initialize(in_file,data_types,read_comp_number,0,1,false);

    

    Master_box box;
    
    Filter_trivial filt;
    box.init(wh,filt);
    

    
    cout<<"total number of particles is: "<<box.size()<<endl;;
    
    Tuple<float,3> dims = wh.get_dims();
    cout<<dims<<endl;
    
    hash_case hcase;
    hcase.init(box,dims,max_range,wh.get_num_frames());

    cout<<"hash case filled"<<endl;
    


    

    Corr_gofr gofr(nbins,max_range);
    hcase.compute_corr(gofr);
    cout<<"computed g(r)"<<endl;
    
    //    gofr.display();

      
    Generic_wrapper_hdf hdf_out(out_file,Generic_wrapper_hdf::F_DISK_RDWR);
    gofr.out_to_wrapper(hdf_out,grp_name,NULL);
    cout<<"wrote out g(r)"<<endl;

    
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
    
  } 
  
  return 0;
}

