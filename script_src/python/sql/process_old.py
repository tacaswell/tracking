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
from datetime import date

def _eat_file_name(conn,dir_name,fnames):
    (sname,dtype) = _parse_dir_name(dir_name)


    for fname in fnames:
        full_name = (dir_name + '/' + fname).strip()

        if os.path.isfile(full_name):
            (tiff_name,temp) = _parsehdf(full_name)
            pdate = date.fromtimestamp(os.path.getmtime(full_name)).strftime('%Y-%m-%d')

            _adddset(tiff_name,temp,sname,dtype,pdate,conn)
            print tiff_name
            tiff_key = conn.execute("select key from dsets where fname=?;",tiff_name).fetchone()[0]

            _addcomp(tiff_name,full_name,pdate,'Iden',conn)


def _adddset(fname,temp,dname,dtype,date,conn):
    t = (fname,dname,temp,dtype,date)
    conn.execute('insert into dsets (fname,dname,temp,dtype,date) values (?,?,?,?,?)',t)
    conn.commit()

def _addcomp(fin_name,fout_name,date,comp,key,conn):
    t = (key,fin_name,fout_name,date,comp)
    conn.execute('insert into comp values (?,?,?,?,?)',t)

def _parsehdf(fname):
    print fname
    F = h5py.File(fname,'r')
    tiff_name = F.attrs.get('image_name',0);

    temp = F.attrs.get('probe_temp',0);
    F.close
    return (tiff_name,temp)


def _parse_dir_name(dir_name):
    ds = dir_name.split('/')
    return (ds[-3] + '-' + ds[-2], ds[-1])

con = sqlite3.connect('/home/tcaswell/sql_play/test.db')


base_proc_path = "/home/tcaswell/colloids/processed/polyNIPAM_batch_12"
os.path.walk(base_proc_path,_eat_file_name,con)


print "done"
