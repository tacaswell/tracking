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

#include "cl_parse.h"
#include <iostream>

using utilities::CL_parse;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::invalid_argument;

CL_parse::CL_parse(int argc,char *  const  argv[]):
  argv_(argv),argc_(argc)
{
}
  
void CL_parse::parse() throw(std::invalid_argument)
{
  int optchar;
  bool found_i=false,found_o= false,found_c = false;
  found_db_ = false;
  
  while((optchar = getopt(argc_,argv_,"i:o:c:")) !=-1)
  {
    switch(optchar)
    {
    case 'i':
      fin_ = string(optarg);
      found_i = true;
      break;
    case 'o':
      fout_ = string(optarg);
      found_o = true;
      break;
    case 'c':
      fpram_ = string(optarg);
      found_c = true;
      break;
    case 'd':
      fdb_ = string(optarg);
      found_db_ = true;
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
    throw invalid_argument("CL_parse: did not find all of the needed arguements");
    
  }
}
