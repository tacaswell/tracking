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
#include <map>
#include <vector>
#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"
#include "pair.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "hash_shelf.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"
#include "read_config.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::map;
using std::vector;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Pair;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;
using utilities::Read_config;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Hash_shelf;
using tracking::Corr_gofr;

int main(int argc, char * const argv[])
{

   
  string in_file; //= "test.h5";	
  string out_file; //= "test.h5";	
  string grp_name; //= "frame";
  
  float neighbor_range ;
  

  
  
  int optchar;
  bool found_i=false,found_o= false,found_f=false;

  
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
      {
  	vector<string> names;
  	names.push_back("neighbor_range");
	names.push_back("grp_name");
  	Read_config rc(string(optarg),names,"phi6");
  	if(!rc.get_val("neighbor_range",neighbor_range))
  	{
  	  cerr<<"neighbor_range not found"<<endl;
  	  return -1;
  	}
  	if(!rc.get_val("grp_name",grp_name))
  	{
  	  cerr<<"grp_name not found"<<endl;
  	  return -1;
  	}
	
  	found_f = true;
  	break;
      }
    case '?':
    default:
      cout<<"-i input filename"<<endl;
      cout<<"-o output filename"<<endl;
      cout<<"-c configuration filename"<<endl;
    break;
    }
  }

  if(!(found_i && found_o && found_f))
  {
    cerr<<"input failed"<<endl;
    cout<<"-i input filename"<<endl;
    cout<<"-o output filename"<<endl;
    cout<<"-c configuration filename"<<endl;
    return -1;
  }
  
  

  // add check to make sure the input and output are the same file
    
  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  cout<<"Parameters: "<<endl;
  cout<<"  neighbor range: "<<neighbor_range<<endl;



  // make hcase

  try
  {
    
  
    //nonsense to get the map set up
    map<utilities::D_TYPE, int> contents;
    utilities::D_TYPE tmp[] = {utilities::D_XPOS,
			       utilities::D_YPOS};
    set<D_TYPE> data_types = set<D_TYPE>(tmp,tmp+2);
    
      
    // set up the input wrapper
    Wrapper_i_hdf wh(in_file,data_types);
  

    // fill the master_box
    Master_box box;
    //  Filter_basic filt(h5_file);
    Filter_trivial filt;
    box.init(wh,filt);
    cout<<"master_box contains "<<box.size()<<" particles"<<endl;
  
    hash_case hcase(box,wh.get_dims(),neighbor_range,wh.get_num_frames());
    cout<<"hash case filled"<<endl;
    
    
      
    particle::set_neighborhood_range(neighbor_range);
  
    hcase.pass_fun_to_shelf(&Hash_shelf::fill_in_neighborhood);
    hcase.pass_fun_to_part(&particle::fill_phi_6);


    // output everything back to the hdf file

  
    set<D_TYPE> d2;
    d2.insert(utilities::D_S_ORDER_PARAMETER);

  
    Wrapper_o_hdf hdf_w(out_file,d2,"frame",
			false,false,false);

    
    try
    {
      hcase.output_to_wrapper(hdf_w);
    }
    catch(const char * err)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<err<<endl;
      return -1;
    }
    catch(...)
    {
      std::cerr<<"unknown error type"<<endl;
      return -1;
    }
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  } 
  catch( char const * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
  } 
  catch(...)
  {
    std::cerr<<"uncaught error"<<endl;
    return -1;
  }

  return 0;
  


}
