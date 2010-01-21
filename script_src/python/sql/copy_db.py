#Copyright 2009 Thomas A Caswell
#tcaswell@uchicago.edu
#http://jfi.uchicago.edu/~tcaswell
#
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 3 of the License, or (at
#your option) any later version.
#
#This program is distributed in the hope that it will be useful, but
#WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, see <http://www.gnu.org/licenses>.

import sqlite3
import h5py
import os.path
import os
import re

from datetime import date

if os.path.isfile('new.db'):
    os.remove('new.db')

con = sqlite3.connect('/home/tcaswell/sql_play/test.db')
con_new = sqlite3.connect('new.db')
con_new.execute('PRAGMA foreign_keys = ON;')
#make tables

con_new.execute('CREATE TABLE dsets (key INTEGER PRIMARY KEY ASC, fname TEXT, temp REAL,  dtype TEXT,date TEXT,sname TEXT);')
con_new.execute('CREATE TABLE comps (comp_key INTEGER PRIMARY KEY ASC, dset_key INTEGER,  fin TEXT, fout TEXT, date TEXT,function TEXT,FOREIGN KEY(dset_key) REFERENCES dsets(key));')


cur = con.cursor()
cur.execute('select * from dsets')

for row in cur:
    con_new.execute('insert into dsets values (?,?,?,?,?,?)',row)
    con_new.commit()

cur.execute('select * from comp')

for row in cur:
    con_new.execute('insert into comps (dset_key,fin,fout,date,function) values (?,?,?,?,?)',row)
    con_new.commit()
