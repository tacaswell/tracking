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



def triple_objects(a,b,c):
    for i in range(len(a)):
        yield (a[i],b[i],c[i])
    

def _start_scene(pov_file):
    print >>pov_file, """

#include "colors.inc"
background {color White}
camera {
    location <100, 100, 100>
    look_at  <50, 50,  50>
    angle 30
    sky <0,0,1>
    }
light_source { <50, 50, 300> color White}
light_source { <50, 50, -300> color White}
light_source { <-100, 50, 50> color White}
light_source { <100, 50, 50> color White}
light_source { <50, -100, 50> color White}
light_source { <50, 100, 50> color White}
"""
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


def _set_posistion(pov_file, vec,r):

    print >> pov_file, "<"+ str(vec[0])+","+str(vec[1])+","+str(vec[2])+">," + str(r)


def main():
    comp_num = 55;
    fbase = "/home/tcaswell/colloids/processed/"
    fpath = "polyNIPAM_batch_12/20100119/a/z/"
    fname = "room_crystal_link.h5 "
    #old = "/home/tcaswell/colloids/processed/polyNIPAM_batch_12/20090730/jammed/z/27-7_link.h5"
    print fbase + fpath + fname

    f = h5py.File((fbase + fpath + fname).strip())
    #scale = 6.45/60
    scale = 1
    x = f["/frame000000/x_%(#)07d"%{"#":comp_num}][:]
    y = f["/frame000000/y_%(#)07d"%{"#":comp_num}][:]
    z = f["/frame000000/z_%(#)07d"%{"#":comp_num}][:]
    pov_file = open("crys_pov_file.pov",'w')


    _start_scene(pov_file)

    #for ind in range(0,20000):
    ztop = 70
    zbot = 30
    xtop = 70
    xbot = 30
    ytop = 70
    ybot = 30

    for (xi,yi,zi) in triple_objects(x,y,z):

        if zi<ztop and zi>zbot and xi <xtop and xi > xbot and yi<ytop and yi >ybot:

            _open_particle(pov_file)
            _set_posistion(pov_file,(xi,yi,zi),.5*scale)
            _close_particle(pov_file,(zi-zbot)/(ztop-zbot)/2)

    _end_scene(pov_file)

    f.close()
    pov_file.close()
