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

import os
import os.path
import subprocess
import h5py

path_name = 'polyNIPAM_batch_12/20090820/2/t/'
base_data_path = "/home/tcaswell/colloids/data/"
base_proc_path = "/home/tcaswell/colloids/processed/"

prog_path = "/home/tcaswell/misc_builds/proc3D/apps/"
prog_name = "goftheta_hdf"


contents = os.listdir(base_data_path +path_name)
to_process = []
for c in contents:
    tmp = c.split('.')
    if tmp[-1]=='done':
        to_process = to_process+[c]

print to_process


# make sure there is a  file
f = h5py.File(base_proc_path + path_name +prog_name + ".h5",'w')
f.close()

for f in to_process:
    tmp = f.split('.')
    base_name = ''.join(tmp[:-1])
    print base_name
    subprocess.call(["time",prog_path + prog_name ,path_name,base_name])
