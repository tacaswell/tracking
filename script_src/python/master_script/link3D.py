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

import parse1
import os
import os.path
import subprocess
import sys
import getopt
import sqlite
from datetime import date


base_proc_path = "/home/tcaswell/colloids/processed/"

prog_path = "/home/tcaswell/misc_builds/basic/apps/"
prog_name = "link3D"


def _process_path(path_name):
    contents = os.listdir(base_data_path +path_name)
    to_process = []
    for c in contents:
        tmp = c.split('.')
        if tmp[-1]=='done':
            to_process = to_process+[c]
    print to_process
    _prcocess_list(to_process)


def _process_filename(file_name):
    f = open(file_name,'r')
    to_process = []
    for line in f:
        line =  line.strip().rsplit('.',1)[0] + '.h5'
        print line
        if os.path.isfile((base_proc_path + line).strip()):
            to_process = to_process + [line.strip()]

    print to_process
    _prcocess_list(to_process)

def _prcocess_list(to_process):
    con = sqlite3.connect('/home/tcaswell/sql_play/test.db')
    for fin in to_process:
        path_name = os.path.dirname(fin) + '/'
        f = os.path.basename(fin)
        
        tmp = f.split('.')
        base_name = ''.join(tmp[:-1])
        print base_proc_path + path_name + base_name
        subprocess.call(["time",prog_path + prog_name,path_name,base_name])
        _addcomp()
        conn.close()


def _addcomp(fin_name,fout_name,date,comp,key,conn):

    t = (key,fin_name,fout_name,date,comp)
    conn.execute('insert into comp values (?,?,?,?,?)',t)
    conn.commit()


def _usage():
    print "help!!!"
    print "-f [filename]"
    print "-p [path]"

def _main():
    try:
        opts,args = getopt.getopt(sys.argv[1:],"f:p:")
        print opts
    except:
        _usage()
        sys.exit(2)
    for opt ,arg in opts:
        if opt == '-f':
            _process_filename(arg)
        elif opt == '-p':
            _process_path(arg)
        else:
            _usage()


if __name__ == "__main__":
    _main()

