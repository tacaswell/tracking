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


#include "accum_sofq.h"
#include <set>
#include "enum_utils.h"
#include "wrapper_i.h"


#include "wrapper_i_hdf.h"

#include "filter.h"
#include "master_box_t.h"
#include "hash_case.h"

#include "read_config.h"

using tracking::Accum_sofq;
using utilities::Tuplef;
using utilities::Tuplei;
using tracking::particle;

using utilities::Filter_trivial;
using utilities::Filter_basic;
using utilities::D_TYPE;
using utilities::Wrapper_i_hdf;

using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;


using utilities::Read_config;




using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::logic_error;

using utilities::Tuple;



static string APP_NAME = "sofq :: ";

int main(int argc, char * argv[])
{

  		   
  string data_file ;
  string out_file ;
  string pram_file ;
  
  
    
  int optchar;
  bool found_i=false,found_o= false,found_c = false;
    
  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'i':
      data_file = string(optarg);
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
  
  cout<<"file to read in: "<<data_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;

  // parsing of parameters


  float max_range;
  int nbins;
  string grp_name;

  Read_config app_prams(pram_file,"sofq");
  if(!(app_prams.contains_key("max_range") &&
       app_prams.contains_key("nbins") ))
    throw logic_error(APP_NAME + " parameter file does not contain enough parameters");
  try
  {
    app_prams.get_value("max_range",max_range);
    app_prams.get_value("nbins",nbins);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  int read_comp_num, write_comp_num,dset_num;
  Read_config comp_prams(pram_file,"comps");
  if(!(comp_prams.contains_key("read_comp")&&
       comp_prams.contains_key("write_comp")))
    throw logic_error(APP_NAME + 
		      " parameter file does not contain both read and write comp nums");
  try
  {
    comp_prams.get_value("read_comp",read_comp_num);
    comp_prams.get_value("write_comp",write_comp_num);
    comp_prams.get_value("dset",dset_num);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  

  
  Tuplei bins(nbins);
  Tuplef range(max_range);

  D_TYPE tmp[] = {utilities::D_XPOS,
		  utilities::D_YPOS,
		  utilities::D_DX,
		  utilities::D_DY,
		  utilities::D_R2,
		  utilities::D_E
  };
  set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+6);

  
    
  Wrapper_i_hdf wh(data_file,data_types,read_comp_num,5,1);

    

  Master_box box;
  Filter_basic filt(data_file,read_comp_num);
  //    Filter_trivial filt;
    

  box.init(wh,filt);
    

    
  cout<<"total number of particles is: "<<box.size()<<endl;;
    
    
  Tuplei dims = wh.get_dims();
  




  
  
  Accum_sofq test2(range,bins);
  box.compute_accum(test2);
  test2.display();
  


  return -1;

}
