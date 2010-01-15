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
import xml.dom.minidom
import tempfile



import subprocess
import sys
import getopt
import sqlite3
from datetime import date


class _xml_data:
    def __init__(self):
        self.doc = xml.dom.minidom.getDOMImplementation(None).createDocument(None,"root",None)

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

        return tf[1]
    def disp(self):
        print self.doc.toprettyxml()

        
        
def _addcomp(fin_name,fout_name,date,comp,key,conn):
    """Adds an entry to the database connection handed in"""
    t = (key,fin_name,fout_name,date,comp)
    conn.execute('insert into comp values (?,?,?,?,?)',t)
    conn.commit()




def _call_program(prog_name,iname,oname,cname,prog_path):
    """Calls a function/program with the given arguements"""
    subprocess.call(["time",prog_path + prog_name,'-i',iname,'-o',oname,'-c',cname])
    #_addcomp(iname,oname,date,program,

def _main():
    
    config = _xml_data()
    config.add_elm("link3D",[("box_side_len","4"),("search_range","3.5"),("min_trk_len","3")])

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
    
    os.remove(fname)


if __name__ == "__main__":
    _main()

