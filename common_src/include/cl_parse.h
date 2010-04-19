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

#include <string>
#include <stdexcept>

namespace utilities
{


/**
   A class to wrap the parsing of the command line to extract the input file, output file
   and parameter file
 */
class CL_parse
{
public:
  /**
     Constructor.  Does nothing but sets fields
   */
  CL_parse(int argc,char * const argv[]);
  /**
     Parses the command line arguments.  Throws 
   */
  void parse() throw(std::invalid_argument);
  void get_fin(std::string & fin_str) const
  {fin_str = fin_;}
  void get_fout(std::string & fout_str) const
  {fout_str = fout_;}
  void get_fpram(std::string & fpram_str) const
  {fpram_str = fpram_;}
private:
  std::string fin_;
  std::string fout_;
  std::string fpram_;
  char * const*  argv_;
  const int argc_;
  
};
}

  
  
  
