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




import sys
import getopt
import sqlite3
import readline
from datetime import date

import cpp_wrapper

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

def _list(conn):
    print "This will access the database"

    return False

def _compute(conn):

    menu = {'L':cpp_wrapper.do_link3D,
            'I':cpp_wrapper.do_Iden,
            'g':cpp_wrapper.do_gofr3D,
            'T':cpp_wrapper.do_tracking
            }
    stop = False
    while not stop:
        cmd = raw_input("enter function: ")
        if cmd =='q':
            stop = True
            break
        if menu.has_key(cmd) :
            key = raw_input("enter key: ")
            stop = menu[cmd](key,conn)
        else:
            print "unknown command"

    return False


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
            


if __name__ == "__main__":
    _main_loop()
    #_quick_test()
    
