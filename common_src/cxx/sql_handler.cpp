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

#include "sql_handler.h"
#include <sqlite3.h>
#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>


using utilities::SQL_handler;
using std::string;
using std::cout;
using std::endl;

using namespace boost::gregorian;


SQL_handler::SQL_handler(const string & db_name)
{
  int rc = sqlite3_open(db_name.c_str(), &db_);
  cout<<rc<<endl;

}

SQL_handler::~SQL_handler()
{
  sqlite3_close(db_);
}

void SQL_handler::add_comp(int dset_key,const string &fin,const string & fout,const string & function)
{
  char * err = NULL;
  int rc;

  std::ostringstream o;
  o<<"insert into comps (dset_key,date,fin,fout,function) values (";
  o<<dset_key<<',';
  o<<"'"<<to_iso_extended_string(day_clock::local_day())<<"',";
  o<<"'"<<fin<<"','"<<fout<<"','"<<function<<"');";
  
  

  rc = sqlite3_exec(db_,o.str().c_str(),NULL,NULL,&err);
  if(rc!=SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", err);
    sqlite3_free(err);
  }
}
