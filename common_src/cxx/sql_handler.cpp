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
using std::logic_error;





/*
   Local helper function for formatting error messages from sqlite
 */
string err_format(string txt,int error);
string rc_formatter(int rc);

/*
  Wrapper for sqlite3_exec 
 */

void exec_wrapper(sqlite3* db, std::string cmd);
/*
  local helper functions for binding.  These functions eat the status
messages from sqlite and either return  if everything is ok, or throw
an exception if there is any error.
 */
void text_bind(sqlite3_stmt* stmt, int indx,const string &  txt);
void int_bind(sqlite3_stmt* stmt, int indx,int val);
void float_bind(sqlite3_stmt* stmt, int indx,float val);


/*
  Class function bodies
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
    close_connection();
  }
  catch(...)
  {
    cout<<"There was an error taking apart the sql_handler object.  Check database integrity"<<endl;
  }
}


void SQL_handler::open_connection(const string & db_name)
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

void SQL_handler::close_connection()
{
  int rc;
  // don't need to roll back my hand as this will automatically roll
  // back all open transaction.  Don't need to check if the connection
  // is open because passing a NULL in is harmless.
  // http://www.sqlite.org/c3ref/close.html

  rc = sqlite3_close(db_);
  if(rc!=SQLITE_OK)
    throw runtime_error(err_format("failed to properly close db",rc));
  
  

  db_ = NULL;
  conn_open_ = false;
}


int SQL_handler::start_comp(int dset_key,
		 int & comp_key,
		 F_TYPE f_type)
{
  int rc;



  sqlite3_stmt * stmt;

  // make sure that there is an open data base connection
  if (!conn_open_)
    throw runtime_error("there is no open data base connection");
  // make sure there is not already an open transaction
  if(trans_open_)
    throw runtime_error("there is already an open transaction, can not open a new one");
  // check to make sure that the function type is valid, get name
  if(f_type == F_NOFUNCTION)
    throw runtime_error("did not pass in a valid function");

  
  // open transaction
  exec_wrapper(db_,"BEGIN EXCLUSIVE TRANSACTION");
  
  // add computation to comps table
  
  // base statement
  const char * base_stmt = "insert into comps (dset_key,func_key) values (?,?);";
  
  // prepare statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to prepare statement, start",rc));
  // bind in the two arguements
  int_bind(stmt,1,dset_key);
  int_bind(stmt,2,(int)f_type);

  // actually shove it into the data base, don't need to check this return value, see doc
  rc  =  sqlite3_step(stmt);
  // clean up
  rc = sqlite3_finalize(stmt);
  if(rc  != SQLITE_OK)
    throw runtime_error(err_format("failed to take apart",rc));
  
  comp_key = sqlite3_last_insert_rowid(db_);
  
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
  case F_IDEN_AVG:
  case F_IDEN:
    iden_md_fun(md_store);
    break;
  case F_TRACKING:
    tracking_md_fun(md_store);
    break;
  case F_MSD:
    msd_md_fun(md_store);
    break;
  case F_MSD_SWEEP:
    msd_sweep_md_fun(md_store);
    break;
  
  case F_GOFR:
    gofr_md_fun(md_store);
    break;
  case F_VANHOVE:
    vanHove_md_fun(md_store);
    break;
  case F_GOFR_BY_PLANE:
    gofr_by_plane_md_fun(md_store);
    break;
  case F_PHI6:
    phi6_md_fun(md_store);
    break;
  case F_GOFR3D:
  case F_LINK3D:
  
  case F_TRACK_STATS:
    throw logic_error("not implemented yet");
    
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
  // make sure that there is an open data base connection, if not bail
  if (!conn_open_)
    return;
  // make sure there is not already an open transaction, if not bail
  if(!trans_open_)
    return;
  exec_wrapper(db_,"ROLLBACK");
  trans_open_ = false;
  trans_type_ = F_NOFUNCTION;
  
}



void SQL_handler::make_test_db(string fname)
{
  if(fname.compare("") == 0)
  {
    cout<<"using in memory db"<<endl;
    fname = ":memory:";
  }
  cout<<fname<<endl;
   
  open_connection(fname);
  exec_wrapper(db_,
	       "CREATE TABLE dsets ("
	       "dset_key INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
	       "fname TEXT NOT NULL UNIQUE, "
	       "dtype TEXT NOT NULL,"
	       "ddate DATE NOT NULL,"
	       "sname TEXT NOT NULL"
	       ");");
  
  exec_wrapper(db_,
	       "CREATE TABLE comps("
	       "comp_key INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
	       "func_key INTEGER NOT NULL,"
	       "dset_key INTEGER NOT NULL,"
	       "FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),"
	       "FOREIGN KEY(func_key) REFERENCES func_names(func_key)"
	       ");");
  
  exec_wrapper(db_,
	       "CREATE TABLE func_names("
	       "func_key INTEGER PRIMARY KEY,"
	       "func_name TEXT NOT NULL UNIQUE"
	       ");");
  


  exec_wrapper(db_,
	       "CREATE TABLE iden("
	       "comp_key INTEGER PRIMARY KEY,"
	       "dset_key INTEGER NOT NULL,"
	       "threshold FLOAT NOT NULL,"
	       "hwhm FLOAT NOT NULL,"
	       "p_rad INTEGER NOT NULL,"
	       "d_rad INTEGER NOT NULL,"
	       "mask_rad INTEGER NOT NULL,"
	       "top_cut FLOAT NOT NULL,"
	       "frames_avged INTEGER NOT NULL,"
	       "FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),"
	       "FOREIGN KEY(comp_key) REFERENCES comps(comp_key)"
	       ");");
 
  exec_wrapper(db_,
	       "CREATE TABLE tracking ("
	       "comp_key INTEGER PRIMARY KEY,"
	       "iden_key INTEGER NOT NULL,  "
	       "dset_key INTEGER NOT NULL,"
	       "search_range FLOAT NOT NULL,"
	       "shift_cut FLOAT ,"
	       "rg_cut FLOAT ,"
	       "e_cut FLOAT ,"
	       "FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),"
	       "FOREIGN KEY(comp_key) REFERENCES comps(comp_key),"
	       "FOREIGN KEY(iden_key) REFERENCES iden(comp_key)"
	       ");");


  exec_wrapper(db_,
	       "CREATE TABLE msd_sweep ("
	       "comp_key     INTEGER  PRIMARY KEY ,"
	       "dset_key     INTEGER  NOT NULL	,"
	       "iden_key    INTEGER  NOT NULL    ,"
	       "track_key    INTEGER  NOT NULL    ,"
	       "trk_len_min  INTEGER  NOT NULL    ,"
	       "trk_len_step INTEGER  NOT NULL    ,"
	       "steps        INTEGER  NOT NULL    ,"
	       "FOREIGN KEY(comp_key)  REFERENCES comps(comp_key),"
	       "FOREIGN KEY(dset_key)  REFERENCES dsets(dset_key),"
	       "FOREIGN KEY(track_key) REFERENCES tracking(comp_key),"
	       "FOREIGN KEY(iden_key) REFERENCES iden(comp_key)"
	       ");");

  exec_wrapper(db_,
	       "CREATE TABLE msd ("
	       "comp_key INTEGER PRIMARY KEY,"
	       "iden_key INTEGER NOT NULL,"
	       "track_key INTEGER NOT NULL,"
	        "dset_key INTEGER NOT NULL,"
	        "msd_steps INTEGER NOT NULL,"
	       "min_track_length INTEGER NOT NULL,"
	       "fin TEXT NOT NULL,"
	       "fout TEXT NOT NULL,"
	       
	       "FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),"
	        "FOREIGN KEY(comp_key) REFERENCES comps(comp_key),"
	        "FOREIGN KEY(track_key) REFERENCES tracking(comp_key)"
	       ");");

  
  exec_wrapper(db_,
	       "insert into func_names (func_name,func_key) values  ('iden',1);"
	       "insert into func_names (func_name,func_key) values  ('iden_avg',2);"
	       "insert into func_names (func_name,func_key) values  ('gofr',3);"
	       "insert into func_names (func_name,func_key) values  ('gofr3d',4);"
	       "insert into func_names (func_name,func_key) values  ('gofr_by_plane',5);"
	       "insert into func_names (func_name,func_key) values  ('link3d',6);"
	       "insert into func_names (func_name,func_key) values  ('msd',7);"
	       "insert into func_names (func_name,func_key) values  ('phi6',8);"
	       "insert into func_names (func_name,func_key) values  ('track_stats',9);"
	       "insert into func_names (func_name,func_key) values  ('tracking',10);"
	       "insert into func_names (func_name,func_key) values  ('vanhove',11);"
	       "insert into func_names (func_name,func_key) values  ('msd_sweep',12);"
	       );
  

  exec_wrapper(db_,
	       "insert into dsets (fname,dtype,ddate,sname)"
	       "values ('filename_0','t','1900-01-01','test_sname');");
  exec_wrapper(db_,
	       "insert into dsets (fname,dtype,ddate,sname)"
	       "values ('filename_1','t','1900-01-01','test_sname');");
  exec_wrapper(db_,
	       "insert into dsets (fname,dtype,ddate,sname)"
	       "values ('filename_2','t','1900-01-01','test_sname');");
  
}


void SQL_handler::tracking_md_fun(const  Md_store & md_store)
{
  float tmp_float;
  int tmp_int;
  string tmp_str;
  
  int rc;
  sqlite3_stmt * stmt;

  // set up statement to be executed
  const char * base_stmt = "insert into tracking "
    "(comp_key,iden_key,dset_key,search_range,shift_cut,rg_cut,e_cut,fin,fout) "
    "values (?,?,?, ?,?,?,?, ?,?)";

  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to prepare statement",rc));
  
  
  

  // bind in the values
  int_bind  (stmt,1 ,md_store.get_value("comp_key"    ,tmp_int));
  int_bind  (stmt,2 ,md_store.get_value("iden_key"    ,tmp_int));
  int_bind  (stmt,3 ,md_store.get_value("dset_key"    ,tmp_int));
  float_bind(stmt,4 ,md_store.get_value("search_range",tmp_float));
  float_bind(stmt,5 ,md_store.get_value("shift_cut"   ,tmp_float));
  float_bind(stmt,6 ,md_store.get_value("rg_cut"      ,tmp_float));
  float_bind(stmt,7 ,md_store.get_value("e_cut"       ,tmp_float));
  text_bind (stmt,8 ,md_store.get_value("fin"         ,tmp_str));
  text_bind (stmt,9 ,md_store.get_value("fout"        ,tmp_str));
  
  
  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    rollback();
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
    throw runtime_error(err_format("failed to take apart",rc));

}

void SQL_handler::iden_md_fun(const  Md_store & md_store)
{
  float tmp_float;
  int tmp_int;
  string tmp_str;
  
  int rc;
  sqlite3_stmt * stmt;

  // set up statement to be executed
  const char * base_stmt = "insert into iden "
    "(comp_key,dset_key,threshold,hwhm,p_rad,d_rad,mask_rad,top_cut,frames_avged) "
    "values (?,?,?,?, ?,?,?,? ,?)";



  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to prepare statement",rc));

  
  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("dset_key"    ,tmp_int  ));
  float_bind(stmt,3,md_store.get_value("threshold"   ,tmp_float));
  float_bind(stmt,4,md_store.get_value("hwhm"        ,tmp_float));
  int_bind  (stmt,5,md_store.get_value("p_rad"       ,tmp_int  ));
  int_bind  (stmt,6,md_store.get_value("d_rad"       ,tmp_int  ));
  int_bind  (stmt,7,md_store.get_value("mask_rad"    ,tmp_int  ));
  float_bind(stmt,8,md_store.get_value("top_cut"     ,tmp_float));
  int_bind  (stmt,9,md_store.get_value("frames_avged",tmp_int  ));
  

  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
      rollback();
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
    throw runtime_error(err_format("failed to take apart",rc));

}

  
void SQL_handler::msd_md_fun(const  Md_store & md_store)
{
  // house keeping
  int tmp_int;
  string  tmp_str;
  int rc;
  sqlite3_stmt * stmt;



  // set up statement to be executed
  const char * base_stmt = "insert into msd "
    "(comp_key,iden_key,track_key,dset_key,msd_steps,min_track_length,fin,fout) "
    "values (?,?,?, ? ,?,?,?,?)";
    

  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    throw runtime_error(err_format("failed to prepare statement",rc));
  }
  

  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("iden_key"    ,tmp_int  ));  
  int_bind  (stmt,3,md_store.get_value("track_key"   ,tmp_int  ));
  int_bind  (stmt,4,md_store.get_value("dset_key"    ,tmp_int  ));
  
  int_bind  (stmt,5,md_store.get_value("msd_steps" ,tmp_int  ));
  int_bind  (stmt,6,md_store.get_value("min_track_length",tmp_int  ));
  
  text_bind  (stmt,7,md_store.get_value("fin",tmp_str  ));
  text_bind  (stmt,8,md_store.get_value("fout",tmp_str  ));

  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    cout<<err_format("binding or insertion did not go well error",rc)<<endl;
  
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
  {
    rollback();
    throw runtime_error(err_format("failed to take apart",rc));
  }
  
  
  return;
}

void SQL_handler::gofr_md_fun(const  Md_store & md_store)
{
  // house keeping
  int tmp_int;
  string  tmp_str;
  float tmp_float;
  
  int rc;
  sqlite3_stmt * stmt;



  // set up statement to be executed
  const char * base_stmt = "insert into gofr "
    "(comp_key,iden_key,dset_key,"
    "nbins,max_range,"
    "shift_cut,rg_cut,e_cut,"
    "fin,fout) "
    "values (?,?,?, ?,?, ?,?,?, ?,?)";
    

  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    throw runtime_error(err_format("failed to prepare statement",rc));
  }
  


  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("iden_key"    ,tmp_int  ));  
  int_bind  (stmt,3,md_store.get_value("dset_key"    ,tmp_int  ));

  int_bind  (stmt,4,md_store.get_value("nbins"   ,tmp_int  ));
  float_bind  (stmt,5,md_store.get_value("max_range" ,tmp_float  ));

  float_bind  (stmt,6,md_store.get_value("shift_cut" ,tmp_float  ));
  float_bind  (stmt,7,md_store.get_value("rg_cut" ,tmp_float  ));
  float_bind  (stmt,8,md_store.get_value("e_cut" ,tmp_float  ));
  
  
  text_bind  (stmt,9,md_store.get_value("fin",tmp_str  ));
  text_bind  (stmt,10,md_store.get_value("fout",tmp_str  ));

  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    cout<<err_format("binding or insertion did not go well error",rc)<<endl;
  
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
  {
    rollback();
    throw runtime_error(err_format("failed to take apart",rc));
  }
  
  
  return;
}

void SQL_handler::msd_sweep_md_fun(const  Md_store & md_store)
{

  int tmp_int;
  
  int rc;
  sqlite3_stmt * stmt;

  // set up statement to be executed
  const char * base_stmt = "insert into msd_sweep "
    "(comp_key,dset_key,iden_key,track_key,trk_len_min,trk_len_step,steps) "
    "values (?,?,?, ?,?,?,?)";



  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
    throw runtime_error(err_format("failed to prepare statement",rc));

  
  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("dset_key"    ,tmp_int  ));
  int_bind  (stmt,3,md_store.get_value("iden_key"    ,tmp_int  ));
  int_bind  (stmt,4,md_store.get_value("track_key"   ,tmp_int  ));
  int_bind  (stmt,5,md_store.get_value("trk_len_min" ,tmp_int  ));
  int_bind  (stmt,6,md_store.get_value("trk_len_step",tmp_int  ));
  int_bind  (stmt,7,md_store.get_value("steps"       ,tmp_int  ));
  

  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    cout<<err_format("binding or insertion did not go well error",rc)<<endl;
  
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
  {
    rollback();
    throw runtime_error(err_format("failed to take apart",rc));
  }
  
}


void SQL_handler::get_comp_mdata(int comp_key,Md_store & md_store, string table_name)
{
  int rc;
  

  // look up comp in comps and figure out which table to go to
  
  //string table_name = "iden";
  

  // look up in correct table
  // base of the statement
  const string base_stmt = "select * from " + table_name + " where comp_key = ?;";
  sqlite3_stmt * stmt;
  // prepare the statement
  rc = sqlite3_prepare_v2(db_,base_stmt.c_str(),-1,&stmt,NULL);
  if(rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    throw runtime_error(err_format("failed to prepare statement",rc));
  }
  
  // bind in the comp_key
  int_bind(stmt,1,comp_key);
  
  // step the statement, which should return a row
  rc = sqlite3_step(stmt);
  if(rc  != SQLITE_ROW)
    throw runtime_error(err_format("failed to find data",rc));
  
  // loop over the result columns and put in to the md_store object.
  int col_count = sqlite3_column_count(stmt);
  for(int j = 0;j<col_count;j++)
  {
    // look up the column name
    const char * cname = sqlite3_column_origin_name(stmt,j);
    // look up column type
    int ctype = sqlite3_column_type(stmt,j);
    // done with if elif cascade instead of switch to make scoping
    // issues go away
    if(ctype == SQLITE_INTEGER)
    {
      // handle ints
      int tmp_value = sqlite3_column_int(stmt,j);
      md_store.add_element(cname,tmp_value);
    }
    else if (ctype == SQLITE_FLOAT)
    {
      // handle float
      double tmp_value = sqlite3_column_double(stmt,j);
      md_store.add_element(cname,(float)tmp_value);
    }
    else if (ctype == SQLITE_BLOB)
    {
      cout<<"BLOB"<<endl;
      throw logic_error("code can't deal with blob type");
    }
    else if (ctype == SQLITE_NULL)
    {
      cout<<"NULL"<<endl;
      throw logic_error("code can't deal with null type");
    }
    else if (ctype == SQLITE3_TEXT)
    {
      // handle strings
      const unsigned char * tmp_value = sqlite3_column_text(stmt,j);

      md_store.add_element(cname,reinterpret_cast<const char *>(tmp_value));;
      
    }
    
    
  }
  
  rc = sqlite3_finalize(stmt);
  if(rc  != SQLITE_OK)
    throw runtime_error(err_format("failed to take apart",rc));

    
}


void SQL_handler::vanHove_md_fun(const  Md_store & md_store)
{
  // house keeping
  int tmp_int;
  string  tmp_str;
  float tmp_float;
  
  int rc;
  sqlite3_stmt * stmt;



  // set up statement to be executed
  const char * base_stmt = "insert into vanHove "
    "(comp_key,dset_key,track_key,"
    "min_track_length,max_step,max_range,nbins,"
    "fin,fout) "
    "values (?,?,?, ?,?,?,? ,?,?)";
    
  
  
  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    throw runtime_error(err_format("failed to prepare statement",rc));
  }
  


  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("dset_key"    ,tmp_int  ));  
  int_bind  (stmt,3,md_store.get_value("track_key"    ,tmp_int  ));

  int_bind  (stmt,4,md_store.get_value("min_track_length"   ,tmp_int  ));
  int_bind  (stmt,5,md_store.get_value("max_step"   ,tmp_int  ));
  int_bind  (stmt,6,md_store.get_value("max_range" ,tmp_float  ));
  int_bind  (stmt,7,md_store.get_value("nbins"   ,tmp_int  ));
  
  text_bind  (stmt,8,md_store.get_value("fin",tmp_str  ));
  text_bind  (stmt,9,md_store.get_value("fout",tmp_str  ));


  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    cout<<err_format("binding or insertion did not go well error",rc)<<endl;
  
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
  {
    rollback();
    throw runtime_error(err_format("failed to take apart",rc));
  }
  
  
  return;
}


void SQL_handler::gofr_by_plane_md_fun(const  Md_store & md_store)
{
  // house keeping
  int tmp_int;
  string  tmp_str;
  float tmp_float;
  
  int rc;
  sqlite3_stmt * stmt;



  // set up statement to be executed
  const char * base_stmt = "insert into gofr_by_plane "
    "(comp_key,iden_key,dset_key,"
    "nbins,max_range,comp_count,"
    "shift_cut,rg_cut,e_cut,"
    "fin,fout) "
    "values (?,?,?, ?,?,?, ?,?,?, ?,?)";
    

  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    throw runtime_error(err_format("failed to prepare statement",rc));
  }
  


  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("iden_key"    ,tmp_int  ));  
  int_bind  (stmt,3,md_store.get_value("dset_key"    ,tmp_int  ));

  int_bind  (stmt,4,md_store.get_value("nbins"   ,tmp_int  ));
  float_bind  (stmt,5,md_store.get_value("max_range" ,tmp_float  ));
  int_bind  (stmt,6,md_store.get_value("comp_count"   ,tmp_int  ));

  float_bind  (stmt,7,md_store.get_value("shift_cut" ,tmp_float  ));
  float_bind  (stmt,8,md_store.get_value("rg_cut" ,tmp_float  ));
  float_bind  (stmt,9,md_store.get_value("e_cut" ,tmp_float  ));
  
  
  text_bind  (stmt,10,md_store.get_value("fin",tmp_str  ));
  text_bind  (stmt,11,md_store.get_value("fout",tmp_str  ));

  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    cout<<err_format("binding or insertion did not go well error",rc)<<endl;
  
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
  {
    rollback();
    throw runtime_error(err_format("failed to take apart",rc));
  }
  
  
  return;
}


void SQL_handler::phi6_md_fun(const  Md_store & md_store)
{
  // house keeping
  int tmp_int;
  string  tmp_str;
  float tmp_float;
  
  int rc;
  sqlite3_stmt * stmt;



  // set up statement to be executed
  const char * base_stmt = "insert into phi6 "
    "(comp_key,iden_key,dset_key,"
    "neighbor_range,"
    "shift_cut,rg_cut,e_cut,"
    "fin,fout) "
    "values (?,?,?, ?, ?,?,?, ?,?)";
    

  // prepare the sql statement
  rc = sqlite3_prepare_v2(db_,base_stmt,-1,&stmt,NULL);
  if(rc != SQLITE_OK)
  {
    sqlite3_finalize(stmt);
    throw runtime_error(err_format("failed to prepare statement",rc));
  }
  


  
  // bind in the values
  int_bind  (stmt,1,md_store.get_value("comp_key"    ,tmp_int  ));
  int_bind  (stmt,2,md_store.get_value("iden_key"    ,tmp_int  ));  
  int_bind  (stmt,3,md_store.get_value("dset_key"    ,tmp_int  ));
  
  float_bind  (stmt,4,md_store.get_value("neighbor_range" ,tmp_float  ));
  
  float_bind  (stmt,5,md_store.get_value("shift_cut" ,tmp_float  ));
  float_bind  (stmt,6,md_store.get_value("rg_cut" ,tmp_float  ));
  float_bind  (stmt,7,md_store.get_value("e_cut" ,tmp_float  ));
    
  text_bind  (stmt,8,md_store.get_value("fin",tmp_str  ));
  text_bind  (stmt,9,md_store.get_value("fout",tmp_str  ));

  // try running the statement
  rc  =  sqlite3_step(stmt);
  // if not happy, roll back.  finalize will also return an error if 
  if(rc  != SQLITE_DONE)
    cout<<err_format("binding or insertion did not go well error",rc)<<endl;
  
  
  
  // clean up statement, this needs to be done before throwing so that the db will close
  rc = sqlite3_finalize(stmt);
  
  // if it does not return done, rollback the transaction and throw
  if(rc  != SQLITE_OK)
  {
    rollback();
    throw runtime_error(err_format("failed to take apart",rc));
  }
  
  
  return;
}


/*
  bodies of local functions
 */

void text_bind(sqlite3_stmt* stmt, int indx,const string &  txt)
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
    throw runtime_error(err_format("failed to bind sql_paramater",rc));

  
}

string err_format(string txt,int error)
{
  return txt + ": " + rc_formatter(error);
}

std::string utilities::ftype_to_str(F_TYPE f)
{
  switch(f)
  {
  case F_NOFUNCTION    :
	throw runtime_error("no function given");
  case F_IDEN:
    return "iden";
  case F_IDEN_AVG:
    return "iden_avg";
  case F_MSD:
    return "msd";
  case F_TRACKING:
    return "tracking";
  case F_MSD_SWEEP:
    return "msd_sweep";
  case F_GOFR:
    return "gofr";
  case F_VANHOVE:    
    return "vanHove";
  case F_GOFR3D:
  case F_GOFR_BY_PLANE:
  case F_LINK3D:  
  case F_PHI6:
  case F_TRACK_STATS:
    throw logic_error("not implemented");
    

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
    throw runtime_error(err_format("failed on: " + cmd,rc));
  }
}

std::string rc_formatter(int rc)
{
  switch (rc)
  {
  case 0:   /* Successful result */
    return "SQLITE_OK";;
  case 1:   /* SQL error or missing database */
    return "SQLITE_ERROR";
  case 2:   /* Internal logic error in SQLite */
    return "SQLITE_INTERNAL";
  case 3:   /* Access permission denied */
    return "SQLITE_PERM";
  case 4:   /* Callback routine requested an abort */
    return "SQLITE_ABORT";
  case 5:   /* The database file is locked */
    return "SQLITE_BUSY";
  case 6:   /* A table in the database is locked */
    return "SQLITE_LOCKED";
  case 7:   /* A malloc() failed */
    return "SQLITE_NOMEM";
  case 8:   /* Attempt to write a readonly database */
    return "SQLITE_READONLY";
  case 9:   /* Operation terminated by sqlite3_interrupt()*/
    return "SQLITE_INTERRUPT";
  case 10:   /* Some kind of disk I/O error occurred */
    return "SQLITE_IOERR";
  case 11:   /* The database disk image is malformed */
    return "SQLITE_CORRUPT";
  case 12:   /* NOT USED. Table or record not found */
    return "SQLITE_NOTFOUND";
  case 13:   /* Insertion failed because database is full */
    return "SQLITE_FULL";
  case 14:   /* Unable to open the database file */
    return "SQLITE_CANTOPEN";
  case 15:   /* Database lock protocol error */
    return "SQLITE_PROTOCOL";
  case 16:   /* Database is empty */
    return "SQLITE_EMPTY";
  case 17:   /* The database schema changed */
    return "SQLITE_SCHEMA";
  case 18:   /* String or BLOB exceeds size limit */
    return "SQLITE_TOOBIG";
  case 19:   /* Abort due to constraint violation */
    return "SQLITE_CONSTRAINT";
  case 20:   /* Data type mismatch */
    return "SQLITE_MISMATCH";
  case 21:   /* Library used incorrectly */
    return "SQLITE_MISUSE";
  case 22:   /* Uses OS features not supported on host */
    return "SQLITE_NOLFS";
  case 23:   /* Authorization denied */
    return "SQLITE_AUTH";
  case 24:   /* Auxiliary database format error */
    return "SQLITE_FORMAT";
  case 25:   /* 2nd parameter to sqlite3_bind out of range */
    return "SQLITE_RANGE";
  case 26:   /* File opened that is not a database file */
    return "SQLITE_NOTADB";
  case 100:  /* sqlite3_step() has another row ready */
    return "SQLITE_ROW";
  case 101:  /* sqlite3_step() has finished executing */
    return "SQLITE_DONE";
  default:
    return "unknown code";
  }
}
