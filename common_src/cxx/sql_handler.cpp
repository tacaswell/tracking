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
#include "read_config.h"


using utilities::Read_config;

using utilities::SQL_handler;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::invalid_argument;
using std::runtime_error;


using namespace boost::gregorian;


SQL_handler::SQL_handler(const string & db_name)
{
  // open database
  int rc = sqlite3_open(db_name.c_str(), &db_);
  if(rc!=SQLITE_OK)
  {
    cerr<<"SQL error"<<endl;
    throw runtime_error("failed to open database");
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

string err_format(string txt,int error)
{
  return (*(std::stringstream*)(&((std::stringstream() << txt.c_str() << error)))).str();
}

int txt_bind(sqlite3_stmt* stmt, int indx,const string &  txt)
{
  int rc = sqlite3_bind_text(stmt,indx,txt.c_str(),-1,SQLITE_STATIC);
  
  if( rc!= SQLITE_OK)
    throw runtime_error(err_format("failed to bind sql_paramater error: ",rc));

  return rc;
  
}

int int_bind(sqlite3_stmt* stmt, int indx,int val)
{
  
  int rc =sqlite3_bind_int(stmt,indx,val) ;
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to bind sql_paramater ",rc));
  return rc;
  
}


int float_bind(sqlite3_stmt* stmt, int indx,float val)
{
  
  int rc =sqlite3_bind_int(stmt,indx,val) ;
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to bind sql_paramater ",rc));
  return rc;
  
}


void SQL_handler::add_comp(int dset_key,int comp_key,const string &fin,const string & fout,const string & function)
{
  int rc;
  sqlite3_stmt * stmt;
  string cur_date = to_iso_extended_string(day_clock::local_day());
  
  const char * base_stmt = "insert into comps (dset_key,date,fin,fout,function,comp_key) values (?,?,?,?,?,?);";
  
  
  if(sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL) != SQLITE_OK)
    throw runtime_error("failed to prepare statement");
  
  int_bind(stmt,1,dset_key);
  txt_bind(stmt,2,cur_date);
  txt_bind(stmt,3,fin);
  txt_bind(stmt,4,fout);
  txt_bind(stmt,5,function);
  int_bind(stmt,6,comp_key);
  
  rc  =  sqlite3_step(stmt);
  if(rc  != SQLITE_DONE)
    throw runtime_error(err_format("failed to execute sql statement",rc));
  sqlite3_finalize(stmt);
  
}

void SQL_handler::add_iden_comp_prams(const Read_config & prams,int dset_key,int comp_key)
{
  float tmp_float;
  int tmp_int;
  string tmp_str;
  
  int rc;
  sqlite3_stmt * stmt;

  const char * base_stmt = "insert into Iden_prams (dset_key,comp_key,threshold,top_cut,p_rad,hwhm,d_rad,mask_rad,shift_cut,rg_cut,e_cut) values (?,?,?,?,?,?,?,?,?,?,?);";

  
  
  if(sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL) != SQLITE_OK)
    throw runtime_error("failed to prepare statement");
 

  int_bind(stmt,1,dset_key);
  int_bind(stmt,2,comp_key);
  float_bind(stmt,3,prams.get_value("threshold",tmp_float));
  float_bind(stmt,4,prams.get_value("top_cut",tmp_float));
  int_bind(stmt,5,prams.get_value("p_rad",tmp_int));
  float_bind(stmt,6,prams.get_value("hwhm",tmp_float));
  int_bind(stmt,7,prams.get_value("d_rad",tmp_int));
  int_bind(stmt,8,prams.get_value("mask_rad",tmp_int));
  float_bind(stmt,9,prams.get_value("shift_cut",tmp_float));
  float_bind(stmt,10,prams.get_value("rg_cut",tmp_float));
  float_bind(stmt,11,prams.get_value("e_cut",tmp_float));
  
   
  rc  =  sqlite3_step(stmt);
  if(rc  != SQLITE_DONE)
    throw runtime_error(err_format("failed to execute sql statement",rc));
  sqlite3_finalize(stmt);
 
  


}
