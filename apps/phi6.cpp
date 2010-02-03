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

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;

using utilities::Pair;
using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;


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


  // make hcase
      
  particle::set_neighborhood_range(neighbor_range);
  
  h_case.pass_fun_to_shelf(&Hash_shelf::fill_in_neighborhood);
  h_case.pass_fun_to_part(&particle::fill_phi_6);


  // output everything back to the hdf file
}
