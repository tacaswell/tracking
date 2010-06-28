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

#ifndef SQL_HANDLER
#define SQL_HANDLER

#include<string>

// forward definition of sqlite3 class do get around include the header for sqlite3 in
// a lot of my code.
struct sqlite3;


namespace utilities
{
class Read_config;

/**
   Class to wrap around the sqlite3 interface to deal with making connections to
   data basese, cleaning up after them etc.  Will have both functions do to 
   specific things, ie add a computation, and a function that eats a string.
 */
class SQL_handler
{
public:
  SQL_handler(const std::string& db_name);
  ~SQL_handler();
  void add_comp(int dset_key,
		int comp_key,
		const std::string &fin,
		const std::string & fout,
		const std::string & function);
  void add_iden_comp_prams(const Read_config & prams,int dset_key,int comp_key);

private:
  sqlite3 * db_;
  


};

}


#endif
