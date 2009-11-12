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


import subprocess
import h5py
import os.path
import re
import sys
import colorsys

    

def _start_scene(pov_file):
    print >> pov_file, "#include \"colors.inc\""
    print >> pov_file, "background {color White}"
    print >> pov_file, "  camera {"
    print >> pov_file, "location <-125, 500, 250>"
    print >> pov_file, "look_at  <50, 75,  0>"
    print >> pov_file, "angle 25"
    print >> pov_file, "sky <0,0,1>"
    print >> pov_file,"}"
    print >> pov_file, "light_source { <50, 100, 300> color White}"

def _end_scene(pov_file):
    pass


def _open_particle(pov_file):
    print >> pov_file, "sphere {"


def _close_particle(pov_file,h):
   print >> pov_file, " texture {"
   tmp = colorsys.hsv_to_rgb(h,1,1)
   print >> pov_file, "pigment {color rgb <" +str(tmp[0]) +"," +str(tmp[1]) +"," +str(tmp[2]) +">}"
   print >> pov_file, "}"
   print >> pov_file, "}"


def _set_posistion(pov_file, vec):
    r = .45
    print >> pov_file, "<"+ str(vec[0])+","+str(vec[1])+","+str(vec[2])+">," + str(r)



f = h5py.File("/home/tcaswell/colloids/processed/polyNIPAM_batch_12/20090730/jammed/z/27-7_link_full_3.h5")
scale = .1707576
x = f["/frame000000/x"]
y = f["/frame000000/y"]
z = f["/frame000000/z"]
pov_file = open("pov_file.pov",'w')


_start_scene(pov_file)
#for ind in range(0,20000):
for ind in range(0,x.shape[0]):
    top = 25
    bot = 15
    if z[ind]<top and z[ind]>bot:
        _open_particle(pov_file)
        _set_posistion(pov_file,(x[ind]*scale,y[ind]*scale,z[ind]))
        _close_particle(pov_file,(z[ind]-bot)/(top-bot)/2)

_end_scene(pov_file)
    
f.close()
pov_file.close()
