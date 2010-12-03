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
#include "sqlite3.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include "md_store.h"


using utilities::Md_store;

using utilities::SQL_handler;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::invalid_argument;
using std::runtime_error;



using namespace boost::gregorian;

/*
   Local helper function for formatting error messages from sqlite
 */
string err_format(string txt,int error);

/*
  Wrapper for sqlite3_exec 
 */

void exec_wrapper(sqlite3* db, std::string cmd);
/*
  local helper functions for binding.  These functions eat the status
messages from sqlite and either return  if everything is ok, or throw
an exception if there is any error.
 */
void txt_bind(sqlite3_stmt* stmt, int indx,const string &  txt);
void int_bind(sqlite3_stmt* stmt, int indx,int val);
void float_bind(sqlite3_stmt* stmt, int indx,float val);

/*
  local functions   
 */


SQL_handler::SQL_handler()
 :db_(NULL),
  conn_open_(false),
  trans_open_(false),
  trans_type_(F_NOFUNCTION)
{
}

SQL_handler::~SQL_handler()
{
  try
  {
    close();
  }
  catch(...)
  {
    cout<<"There was an error taking apart the sql_handler object.  Check database integrity"<<endl;
  }
}


SQL_handler::open_connection(const string & db_name)
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
    throw runtime_error("failed to open data base");
    
  }
  conn_open_ = true;
  
}

void SQL_handler::close()
{
  if(conn_open_ | db_)
  {
    // clean up any existing transaction by getting rid of it so
    // crashes don't make clutter
    if(trans_open_)
      rollback();
    sqlite3_close(db_);
  }
  db_ = NULL;
  conn_open_ = false;
}


int SQL_handler::start_comp(int dset_key,
		 int & comp_key,
		 F_TYPE f_type)
{
  int rc;
  char * err;
  string func_name;

  sqlite3_stmt * stmt;

  // make sure that there is an open data base connection
  if (!conn_open_)
    throw runtime_error("there is no open data base connection");
  // make sure there is not already an open transaction
  if(trans_open_)
    throw runtime_error("there is already an open transaction, can not open a new one");
  // check to make sure that the function type is valid, get name
  if(trans_type_ == F_NOFUNCTION)
    throw runtime_error("did not pass in a valid function");
  func_name = ftype_to_str(trans_type_);
  
  // open transaction
  exec_wrapper(db_,"BEGIN EXCLUSIVE TRANSACTION");
  
  // add computation to comps table
  
  // base statement
  const char * base_stmt = "insert into comps (dset_key,function) values (?,?);";
  
  // prepare statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  
  if(rc != SQLITE_OK)
    throw runtime_error("failed to prepare statement");
  // bind in the two arguements
  int_bind(stmt,1,dset_key);
  txt_bind(stmt,2,func_name);
  // acctually shove it into the data base
  rc  =  sqlite3_step(stmt);
  if(rc  != SQLITE_DONE)
    throw runtime_error(err_format("failed to execute sql statement",rc));
  rc = sqlite3_finalize(stmt);
  if(rc  != SQLITE_OK)
    throw runtime_error(err_format("failed to take apart",rc));
  
  comp_key = sqlite3_last_insert_rowid();
  
  // set object values
  trans_type_ = f_type;
  trans_open_ = true;
  comp_key_ = comp_key;
  
  return comp_key;
}

void SQL_handler::add_mdata(const Md_store & md_store)
{
  // sanity checks
  if (!conn_open_)
    throw runtime_error("there is no open data base connection");
  // make sure there is not already an open transaction
  if(!trans_open_)
    throw runtime_error("there is no open transaction");
  int comp_key;
  md_store.get_value("comp_key",comp_key);
  
  if (comp_key != comp_key_)
    throw runtime_error("comp key from creation and the md_store do not match");
  
  // switch on trans_type_.  Farm actual work out to local functions
  // to keep this function clean and readable
  switch(trans_type_)
  {
  case F_NOFUNCTION:
    throw logic_error("should never get to this point with out a real function type");
  case F_IDEN:
    md_iden(db_,md_store);
    break;
  case F_TRACKING:
    md_tracking(db_,md_store);
    break;
  case F_MSD:
    md_msd(db_,md_store);
    break;

  }
  
}

			    
  

void SQL_handler::commit()
{
  
  // check to make sure this action makes sense
  // make sure that there is an open data base connection
  if (!conn_open_)
    throw runtime_error("there is no open data base connection");
  // make sure there is not already an open transaction
  if(!trans_open_)
    throw runtime_error("there is no open transaction");
  exec_wrapper(db_,"END TRANSACTION");
  trans_open_ = false;
  trans_type_ = F_NOFUNCTION;
  
}

void SQL_handler::rollback()
{
  
  // check to make sure this action makes sense
  // make sure that there is an open data base connection
  if (!conn_open_)
    throw runtime_error("there is no open data base connection");
  // make sure there is not already an open transaction
  if(!trans_open_)
    throw runtime_error("there is no open transaction");
  exec_wrapper(db_,"ROLLBACK");
  trans_open_ = false;
  trans_type_ = F_NOFUNCTION;
  
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


void txt_bind(sqlite3_stmt* stmt, int indx,const string &  txt)
{
  int rc = sqlite3_bind_text(stmt,indx,txt.c_str(),-1,SQLITE_STATIC);
  
  if( rc!= SQLITE_OK)
    throw runtime_error(err_format("failed to bind sql_paramater error: ",rc));


  
}

void int_bind(sqlite3_stmt* stmt, int indx,int val)
{
  
  int rc =sqlite3_bind_int(stmt,indx,val) ;
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to bind sql_paramater ",rc));
 
}


void float_bind(sqlite3_stmt* stmt, int indx,float val)
{
  
  int rc =sqlite3_bind_double(stmt,indx,val) ;
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to bind sql_paramater ",rc));

  
}

string err_format(string txt,int error)
{
  return (*(std::stringstream*)(&((std::stringstream() << txt.c_str() << error)))).str();
}

std::string utilities::ftype_to_str(F_TYPE f)
{
  switch(f)
  {
  case F_NOFUNCTION:
    throw runtime_error("no function given");
  case F_IDEN:
    return "Iden";
  case F_TRACKING:
    return "tracking";
  case F_MSD:
    return "msd";
  }
  return "";
}

    
  
void exec_wrapper(sqlite3* db,std::string cmd)
{
  int rc;
  char * err;
  rc = sqlite3_exec(db,cmd.c_str(),NULL,NULL,&err);
  if(rc!=SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", err);
    sqlite3_free(err);
    throw runtime_error("failed on: " + cmd);
  }
}

  
    
