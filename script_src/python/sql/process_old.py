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
import re

from datetime import date

def _eat_file_name(conn,dir_name,fnames):
    (sname,dtype) = _parse_dir_name(dir_name)

    if dtype == 'unknown':
        check_type = True
    else:
        check_type = False

    for fname in fnames:
        full_name = (dir_name + '/' + fname).strip()

        if os.path.isfile(full_name):
            if re.search("gof",full_name):
                pass

            elif re.search("link3D",full_name):
                #_process_link(fname,full_name,dir_name,conn)
                pass
            elif re.search("link",full_name):
                pass
            elif re.search("track",full_name):
                #_process_track(fname,full_name,dir_name,conn)
                pass
            elif re.search("split",full_name):
                pass

            else:
                #_process_iden_fnames(fname,check_type,sname,dtype,conn,full_name)
                pass
            
def _process_track(fname,full_name,dir_name,conn):
    base_name = full_name.replace("_tracks",'')
    base_name = base_name.replace("_part",'')
    print base_name
    key = conn.execute("select key from comp where fout=?;",(base_name,)).fetchone()[0]
    print (key,base_name)
    
def _process_link(fname,full_name,dir_name,conn):
    base_name = full_name.replace("_link3D",'')

    key = conn.execute("select key from comp where fout=?;",(base_name,)).fetchone()[0]

    pdate = date.fromtimestamp(os.path.getmtime(full_name)).strftime('%Y-%m-%d')
    print (key,pdate)
    _addcomp(base_name,full_name,pdate,'link3D',key,conn)
    
def _process_iden_fnames(fname,check_type,sname,dtype,conn,full_name):
    print "found a iden file called " + fname
    (tiff_name,temp) = _parsehdf(full_name)
    pdate = date.fromtimestamp(os.path.getmtime(full_name)).strftime('%Y-%m-%d')

    if tiff_name == 0:
        tiff_name = full_name.replace("processed",'data').replace(".h5",".tif").strip()

    if check_type:
        if re.match('[zZ]_?[sS]eries',fname):
            dtype = 'z'
    print tiff_name
    if temp == -1:
        temp = 'NULL'
    print (sname,temp,dtype)
    _adddset(tiff_name,temp,sname,dtype,pdate,conn)

    tiff_key = conn.execute("select key from dsets where fname=?;",(tiff_name,)).fetchone()[0]

    _addcomp(tiff_name,full_name,pdate,'Iden',tiff_key,conn)
                

def _adddset(fname,temp,dname,dtype,date,conn):
    t = (fname,dname,temp,dtype,date)
    conn.execute('insert into dsets (fname,sname,temp,dtype,date) values (?,?,?,?,?)',t)
    conn.commit()

def _addcomp(fin_name,fout_name,date,comp,key,conn):
    t = (key,fin_name,fout_name,date,comp)
    conn.execute('insert into comp values (?,?,?,?,?)',t)
    conn.commit()
    
def _parsehdf(fname):
    print fname
    F = h5py.File(fname,'r')
    tiff_name = F.attrs.get('image_name',0);

    temp = F.attrs.get('probe_temp',-1);
    F.close
    return (tiff_name,temp)


def _parse_dir_name(dir_name):
    ds = dir_name.split('/')
    flag = False
    count = 0
    sname = ''
    tflag = False
    for part in ds:
        if re.match('[0-9]{8}',part):
            flag = True
        if flag:
            if count == 0:
                sname = sname + part
                count = count +1
            elif re.match('[zt]$',part) and not tflag:
                tflag = True
                dtype = part
                break
            elif part == 'ztl' and not tflag:
                tflag = True
                dtype = part
                break
            elif count<2:
                sname = sname + '-' + part
                count = count + 1
            
    if not tflag:
        dtype = 'unknown'
    return (sname,dtype)

con = sqlite3.connect('/home/tcaswell/sql_play/test.db')


base_proc_path = "/home/tcaswell/colloids/processed/polyNIPAM_batch_12"
os.path.walk(base_proc_path,_eat_file_name,con)


print "done"
