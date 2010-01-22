#!/usr/bin/python
#Copyright 2010 Thomas A Caswell
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

import os
import os.path
import xml.dom.minidom
import tempfile

import parse1

import h5py

import subprocess
import sys
import getopt
import sqlite3
import readline
from datetime import date


class _xml_data:
    def __init__(self):
        self.doc = xml.dom.minidom.getDOMImplementation(None).createDocument(None,"root",None)
        self.fname = None
    def add_elm(self,elm,params):
        tmpelm = self.doc.createElement(elm)
        for a,b in params:
            tmpelm.setAttribute(a,b)
        self.doc.documentElement.appendChild(tmpelm)
    def write_to_file(self):
        tf = tempfile.mkstemp()
        os.close(tf[0])
        f = open(tf[1],'w')
        self.doc.writexml(f,addindent='   ',newl='\n')
        f.close()
        self.fname = tf[1]
        return self.fname
    def disp(self):
        print self.doc.toprettyxml()

    def __del__(self):
        if self.fname:
            if os.path.isfile(self.fname):
                os.remove(self.fname)
        


class Computations:
    def __init__(self,database):
        self.conn = None
    def do_computation(self,func):
        pass
        

def _quick_test():
    pass

def _addcomp(fin_name,fout_name,date,comp,key,conn):
    """Adds an entry to the database connection handed in"""
    t = (key,fin_name,fout_name,date,comp)
    conn.execute('insert into comps (dset_key,fin,fout,out,function) values (?,?,?,?,?)',t)
    conn.commit()




def _call_program(prog_name,iname,oname,cname,prog_path):
    """Calls a function/program with the given arguements"""
    subprocess.call(["time",prog_path + prog_name,'-i',iname,'-o',oname,'-c',cname])
#    _addcomp(iname,oname,date,program,

def _main_test():
    
    config = _xml_data()
    config.add_elm("link3D",[("box_side_len","4"),
                             ("search_range","3.5"),
                             ("min_trk_len","3")])

    config.add_elm("gofr3D",[("max_range","9.5"),
                               ("nbins","2000")])

    config.disp()

    fname = config.write_to_file()

    config2 = _xml_data()
    config2.add_elm("gofr",[("max_range","9.5"),
                               ("nbins","2000")])
    config2.disp()
    
    
    prog_path = "/home/tcaswell/misc_builds/basic/apps/"
    prog_name = "gofr3D"
    #    _call_program(prog_name,"/path/to/in","/path/to/out",fname,prog_path)


def _check_comps_table(key,func,conn):
    # figure out name of file to write to
    res = conn.execute("select fout from comps where dset_key=? and function=?;",(key,func)).fetchall()
    return res



def _do_link3D(key,conn):
    prog_path = "/home/tcaswell/misc_builds/basic/apps/"
    prog_name = "link3D"
    # figure out name of file to write to
    res = _check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        # _do_Iden(key,conn)
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    fname = res[0][0]


    # see if there is already a linked file
    res = conn.execute("select fout from comps where dset_key=? and function='link3D';",(key,)).fetchall()
    if len(res)>0:
        print "Already linked"
        return
    
    config = _xml_data()
    config.add_elm("link3D",[("box_side_len","4"),
                             ("search_range","3.5"),
                             ("min_trk_len","3")])
    cname = config.write_to_file()

    fout = fname.replace(".h5","_link.h5")

    # look
    
    print fname
    print fout


    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fname,'-o',fout, '-c',cname ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (dset_key,date,fin,fout,function) values (?,?,?,?,?);",
                     (key,date.today().isoformat(),fname,fout,prog_name))
        conn.commit()

    

def _add_to_dsets(conn):
    fname = raw_input("enter name: ")
    while not os.path.isfile(fname):
        print "file doest not exist"
        fname = raw_input("enter name: ")
    
    dt = raw_input("date :")
    sname = raw_input("sample name: ")
    dtype = raw_input("enter type: ")
    # add checks
    # add temperature
    conn.execute("insert into dsets (fname,dtype,date,sname) values (?,?,?,?);",(fname,dtype,dt,sname))
    conn.commit()

def _do_Iden(key,conn):
    # see if the file has already been processed
    prog_path = '/home/tcaswell/misc_builds/iden_rel/iden/apps/'
    prog_name = "Iden"
    res = _check_comps_table(key,"Iden",conn)
    if len(res) >0:
        print "already processed, flipping out"
        return

    fin = conn.execute("select fname from dsets where key = ?;",(key,)).fetchone()[0]
    fout = '.'.join(fin.replace("data","processed").split('.')[:-1]) + '.h5'
    fpram = fin.replace("tif","pram")
    if not os.path.isfile(fpram):
        print "can not find parameter file, exiting"
        return

    # ask for confirmation
    print "will processess"
    print fin
    print "from parameters in"
    print fpram
    print "will write to"
    print fout
    resp = raw_input("is this ok?: ")
    if resp[0]=='n':
        print "you have chosen not to continue"
        return

    # format name
    # funny string stuff to match parse1 calls
    base_name = '.'.join(os.path.basename(fin).split('.')[:-1])
    data_path = os.path.dirname(fin)  + '/'
    proc_path = os.path.dirname(fout)  + '/'
    if not os.path.exists(proc_path):
        os.makedirs(proc_path,0751)

    parse1.make_h5(base_name,data_path,proc_path)


    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (dset_key,date,fin,fout,function) values (?,?,?,?,?);",
                     (key,date.today().isoformat(),fin,fout,prog_name))

    




    
    # call process

    
    pass

def _do_gofr3D(key,conn):
    prog_path = '/home/tcaswell/misc_builds/basic/apps/'
    prog_name = "gofr3D"

    # see if the file has already been processed
    res = _check_comps_table(key,"link3D",conn)
    if len(res) ==0:
        print "no entry"
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    fin = res[0][0]

    fout = os.path.dirname(fin) + '/gofr3D.h5'
    if not os.path.isfile(fout):
        f = h5py.File(fout)
        f.close()
        del f

    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    config = _xml_data()
    config.add_elm("gofr3D",[("max_range","9.5"),
                             ("nbins","2000"),
                             ("grp_name",str(comp_num))])
    config.disp()
    cfile = config.write_to_file()
    
    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout, '-c',cfile ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (comp_key,dset_key,date,fin,fout,function) values (?,?,?,?,?,?);",
                     (comp_num,key,date.today().isoformat(),fin,fout,prog_name))


    

    
    pass


def _do_gofr2D(key,conn):
    # see if the file has already been processed
    res = _check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    fname = res[0][0]

    # format name

    # call process
    pass


    
def _main_loop():
    stop = False;
    conn = sqlite3.connect('/home/tcaswell/colloids/processed/processed_data.db')
    menu = {'l': _list,
            'c': _compute,
            'a': _add_to_dsets,          
            'q': lambda x :  True}

    while not stop:
        cmd = raw_input("enter command: ")
        cmd = cmd[0]
        if menu.has_key(cmd):
            stop = menu[cmd](conn)
        else:
            print "unknown command"
            

def _list(conn):
    print "This will access the database"

    return False

def _compute(conn):

    menu = {'L':_do_link3D,
            'I':_do_Iden,
            'g':_do_gofr3D,
            'q': lambda x,y :True
            }
    stop = False
    while not stop:
        cmd = raw_input("enter function: ")
        if menu.has_key(cmd):
            key = raw_input("enter key: ")
            stop = menu[cmd](key,conn)
        else:
            print "unknown command"
        

            
    return False

if __name__ == "__main__":
    _main_loop()
    #_quick_test()
    
