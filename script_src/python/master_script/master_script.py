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

path_name = 'polyNIPAM_batch_12/20090820/2/t/'
base_data_path = "/home/tcaswell/colloids/data/"
base_proc_path = "/home/tcaswell/colloids/processed/"

contents = os.listdir(base_data_path +path_name)
to_process = []
for c in contents:
    tmp = c.split('.')
    if tmp[-1]=='pram':
        to_process = to_process+[c]

print to_process
if not os.path.exists(base_proc_path + path_name):
    os.makedirs(base_proc_path + path_name,0751)

for f in to_process:
    tmp = f.split('.')
    base_name = ''.join(tmp[:-1])
    print base_name
    parse1.make_h5(base_name,base_data_path +path_name,base_proc_path+path_name)
    subprocess.call(["Iden",path_name,base_name])
