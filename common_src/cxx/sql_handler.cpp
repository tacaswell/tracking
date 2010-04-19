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
#include <stdexcept>

using utilities::SQL_handler;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::invalid_argument;


using namespace boost::gregorian;


SQL_handler::SQL_handler(const string & db_name)
{
  // open database
  int rc = sqlite3_open(db_name.c_str(), &db_);
  if(rc!=SQLITE_OK)
  {
    cerr<<"SQL error"<<endl;
    throw "failed to open database";
    
  }
  // set foreign key pragma
  char * err;
  rc = sqlite3_exec(db_,"PRAGMA foreign_keys = ON;",NULL,NULL,&err);
  if(rc!=SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", err);
    sqlite3_free(err);
  }
}

SQL_handler::~SQL_handler()
{
  sqlite3_close(db_);
}


void SQL_handler::add_comp(int dset_key,const string &fin,const string & fout,const string & function)
{
  char * err = NULL;

  int rc;

  // assemble command string
  std::ostringstream o;
  o<<"insert into comps (dset_key,date,fin,fout,function) values (";
  o<<dset_key<<',';
  o<<"'"<<to_iso_extended_string(day_clock::local_day())<<"',";
  o<<"'"<<fin<<"','"<<fout<<"','"<<function<<"');";
  
  
  // shove into data base
  rc = sqlite3_exec(db_,o.str().c_str(),NULL,NULL,&err);
  // check result
  if(rc!=SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", err);
    string str_err = err;
    sqlite3_free(err);
    throw invalid_argument(str_err);
  }


  
}
