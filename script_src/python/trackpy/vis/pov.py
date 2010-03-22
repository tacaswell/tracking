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
import sqlite3
import math

def _triple_objects(a,b,c):
    for i in range(len(a)):
        yield (a[i],b[i],c[i])
def _quad_objects(a,b,c,d):
    """ generator for quads"""
    for i in range(len(a)):
        yield (a[i],b[i],c[i],d[i])
    

def _start_scene(pov_file,max_x,max_y,max_z):
    print >>pov_file, """
    #include \"colors.inc\"
    background {color White}
    camera {
"""
    print >>pov_file, 'location <' + str(max_x*3)+ ',' +str(max_y*3)+','+str(max_z*3)  + '>'
    print >>pov_file, 'look_at  <' + str(max_x/2)+ ',' +str(max_y/2)+','+str(max_z/2)  + '>'
    print >>pov_file, """
    angle 21
    sky <0,0,1>
    }
"""
    print >>pov_file," light_source { <"+ str(max_x/2)+ ',' +str(max_y/2)+','+str(max_z*3)  +"> color White}"
    print >>pov_file," light_source { <"+ str(max_x/2)+ ',' +str(max_y/2)+','+str(-max_z*2)  +"> color White}"
    print >>pov_file," light_source { <"+ str(max_x/2)+ ',' +str(max_y*3)+','+str(max_z/2)  +"> color White}"
    print >>pov_file," light_source { <"+ str(max_x/2)+ ',' +str(-max_y*2)+','+str(max_z/2)  +"> color White}"
    print >>pov_file," light_source { <"+ str(max_x*3)+ ',' +str(max_y/2)+','+str(max_z/2)  +"> color White}"
    print >>pov_file," light_source { <"+ str(-max_x*2)+ ',' +str(max_y/2)+','+str(max_z/2)  +"> color White}"

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


def make_range_pov(x,y,z,I,range_x,range_y,range_z,out_name):

    pov_file= open(out_name,'w')

    max_I = max(I)
    _start_scene(pov_file,max(x),max(y),max(z))
    draw_box(pov_file,(0,max(x)),(0,max(y)),(0,max(z)))
    scale = .95
    for (xi,yi,zi,Ii) in _quad_objects(x,y,z,I):

        if zi<range_z[1] and zi>range_z[0] and xi <range_x[1] and xi > range_x[0] and yi<range_y[1] and yi >range_x[0]:
            _open_particle(pov_file)
            _set_posistion(pov_file,(xi,yi,zi),.5*scale)
            _close_particle(pov_file,Ii/max_I)

    _end_scene(pov_file)


    pov_file.close()


    

def draw_box(pov_file,rangex,rangey,rangez):
    for z in rangez:
        for x in rangex:
            print >>pov_file," cylinder{"
            print >>pov_file,'<' +str(x)+','+str(rangey[0])+','+str(z)+'>,<'+str(x)+','+str(rangey[1])+','+str(z)+'>,.5'
            print >>pov_file,"""
            texture{pigment{color Yellow}}
            }"""
        for y in rangey:
            print >>pov_file," cylinder{"
            print >>pov_file,'<'+str(rangex[0])+','+str(y)+','+str(z)+'>,<'+str(rangex[1])+','+str(y)+','+str(z)+'>,.5'
            print >>pov_file,"""
            texture{pigment{color Blue}}
            }"""
    for x in rangex:
        for y in rangey:
            print >>pov_file," cylinder{"
            print >>pov_file,'<'+str(x)+','+str(y)+','+str(rangez[0])+'>,<'+str(x)+','+str(y)+','+str(rangez[1])+'>,.5'
            print >>pov_file,"""
            texture{pigment{color Green}}
            }"""

def make_z_slices(x,y,z,I,step_sz,base_name):


    

    # x = f["/frame000000/x_%(#)07d"%{"#":comp_num}][:]
    # y = f["/frame000000/y_%(#)07d"%{"#":comp_num}][:]
    # z = f["/frame000000/z_%(#)07d"%{"#":comp_num}][:]
    # # get max of ranges
    # max_z = 15;
    # # loop over z
    max_z = max(z)
    max_y = max(y)
    max_x = max(x)
    for zstp in range(0,math.ceil(max_z),step_sz):
        print zstp
        make_range_pov(x,y,z,I,(0,max_x),(0,max_y),(zstp,zstp+step_sz),base_name + '_%(#)04d'%{"#":(zstp/step_sz)} + '.pov')
        pass
    
