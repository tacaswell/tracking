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
#include "pair.h"
#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"
#include "corr_gofr.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot


#include <unistd.h>
#include <vector>
#include "read_config.h"


using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::vector;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Triple;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;
using utilities::Read_config;


using tracking::Master_box;
using tracking::particle;
using tracking::hash_case;
using tracking::Corr_gofr;



int main(int argc, char * const argv[])
{
  
  float max_range = 10;
  int nbins;
  string proc_file;
  string out_file;
  string grp_name;
  
  int optchar;
  bool found_i=false,found_o= false,found_f=false;

  
  while((optchar = getopt(argc,argv,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'i':
      proc_file = string(optarg);
      found_i = true;
      break;
    case 'o':
      out_file = string(optarg);
      found_o = true;
      break;
    case 'c':
      {
	vector<string> names;
	names.push_back("max_range");
	names.push_back("nbins");
	names.push_back("grp_name");
	Read_config rc(string(optarg),names,"gofr3D");
	if(!rc.get_val("max_range",max_range))
	{
	  cerr<<"max_range not found"<<endl;
	  return -1;
	}
	if(!rc.get_val("nbins",nbins))
	{
	  cerr<<"nbins not found"<<endl;
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
  
  
    
  cout<<"file to read in: "<<proc_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  cout<<"Parameters: "<<endl;
  cout<<"  max range: "<<max_range<<endl;
  cout<<"  nbins: "<<nbins<<endl;

  

  
  try
  {


     
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
		    utilities::D_ZPOS,
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+3);
    
    /// @TODO these parameters need to be passed in
    Wrapper_i_hdf wh(proc_file,data_types,0,1,false);

    

    Master_box box;
    
    
    //Filter_basic filt(proc_file);
    Filter_trivial filt;
    box.init(wh,filt);
    

    
    cout<<"total number of particles is: "<<box.size()<<endl;;
    
    Triple dims = wh.get_dims();
    cout<<dims<<endl;
    
    hash_case hcase(box,dims,max_range,wh.get_num_frames());

    cout<<"hash case filled"<<endl;
    
// //     particle::set_neighborhood_range(101);
// //     hcase.fill_in_neighborhood();
    
    
//     //hcase.print();

    
     Corr_gofr gofr(nbins,(float)max_range,grp_name);
     hcase.compute_corr(gofr);
     cout<<"computed g(r)"<<endl;
    
     //     gofr.display();

      
    Generic_wrapper_hdf hdf_out(out_file,true);
    gofr.out_to_wrapper(hdf_out);
    cout<<"wrote out g(r)"<<endl;

    
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
    return -1;
    
  } 
  
  return 0;
}

