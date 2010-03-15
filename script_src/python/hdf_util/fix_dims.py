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
import math
import re

def _fix_z(cnum,dir_name,fnames):
    for f in fnames:
#        if(os.path.isfile((dir_name +'/'+ f).strip()) and f[-9:] == "link3D.h5"  ):
        if(os.path.isfile((dir_name +'/'+ f).strip()) and f[-7:] == "link.h5"  ):
            print dir_name + f
            F = h5py.File((dir_name +'/'+ f),'r+')
            tmp = F.attrs.get('dims',0);
            print tmp
            tmp[2] = math.ceil(F["/frame000000/z_%(#)07d"%{"#":cnum}][:].max())
            tmp[0] = math.ceil(tmp[0])
            tmp[1] = math.ceil(tmp[1])
            print tmp
            F.attrs.modify('dims',tmp)
            F.close()


def _fix_all_dims(cnum,dir_name,fnames):
    for f in fnames:
#        if(os.path.isfile((dir_name +'/'+ f).strip()) and f[-9:] == "link3D.h5"  ):
        if(os.path.isfile((dir_name +'/'+ f).strip()) and f[-7:] == "link.h5"  ):
            print dir_name + f
            F = h5py.File((dir_name +'/'+ f),'r+')
            tmp = F.attrs.get('dims',0);
            print tmp
            tmp[2] = math.ceil(F["/frame000000/z_%(#)07d"%{"#":cnum}][:].max())
            tmp[0] = math.ceil(F["/frame000000/x_%(#)07d"%{"#":cnum}][:].max())
            tmp[1] = math.ceil(F["/frame000000/y_%(#)07d"%{"#":cnum}][:].max())
            print tmp
            F.attrs.modify('dims',tmp)
            F.close()



def _rescale(cnum,dir_name,fnames):
    scale = 6.45/60
    
    for f in fnames:
        if(os.path.isfile((dir_name +'/'+ f).strip()) and f[-7:] == "link.h5"  ):
            print dir_name + f
            F = h5py.File((dir_name +'/'+ f),'r+')
            tmp = F.attrs.get('dims',0);
            print tmp
            F["/frame000000/y_%(#)07d"%{"#":cnum}][:] = F["/frame000000/y_%(#)07d"%{"#":cnum}][:] * scale
            F["/frame000000/x_%(#)07d"%{"#":cnum}][:] = F["/frame000000/x_%(#)07d"%{"#":cnum}][:] * scale
            
            tmp[2] = math.ceil(F["/frame000000/z_%(#)07d"%{"#":cnum}][:].max())
            tmp[0] = math.ceil(F["/frame000000/x_%(#)07d"%{"#":cnum}][:].max())
            tmp[1] = math.ceil(F["/frame000000/y_%(#)07d"%{"#":cnum}][:].max())
            print tmp
                                  
            F.attrs.modify('dims',tmp)
            
            F.close()

base_proc_path = "/home/tcaswell/colloids/processed/polyNIPAM_batch_12/20100119/a/z/"
os.path.walk(base_proc_path,_rescale,55)



