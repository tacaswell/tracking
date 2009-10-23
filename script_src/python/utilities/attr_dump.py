#!/usr/bin/python
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




import h5py
import os.path
import sys

def _dump(fname,f_out):
    print fname
    f = h5py.File(fname,'r')
    f_out.write("top level\n")
    f_out.write("===============\n")
    for a in f.attrs:
        f_out.write(a +"\n")
    g = f['frame000000']
    f_out.write("\n")
    f_out.write("frame level\n")
    f_out.write("===============\n")
    for a in g.attrs:
        f_out.write(a +"\n")
    f.close()
    

    

path_name = 'polyNIPAM_batch_12/20090820/2/t/'
base_proc_path = "/home/tcaswell/colloids/processed/"


if len(sys.argv) <2:
    print "provide a file name"
    exit()
if len(sys.argv) >= 2:
    file_name = sys.argv[1]
if len(sys.argv) >= 3:
    i_path = sys.argv[2]
else:
    i_path = ""


_dump(file_name,sys.stdout)

