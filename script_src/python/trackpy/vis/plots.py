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


import sqlite3
import trackpy.cpp_wrapper as cpp_wrapper
import matplotlib
import matplotlib.pyplot as plt
import random
import itertools
import h5py

# change to take 
def _plot_file_frame_phi6(f,comp_number,fr_num):
    '''
    Takes an open hdf_file handle, f, 
    '''
    x = f["/frame%(#)06d"%{"#":fr_num}+"/x_%(#)07d"%{"#":comp_number}]
    y = f["/frame%(#)06d"%{"#":fr_num}+"/y_%(#)07d"%{"#":comp_number}]
    phi = f["/frame%(#)06d"%{"#":fr_num}+"/scaler_order_parameter_%(#)07d"%{"#":comp_number}]
    phir = zeros(phi.shape[0])
    phii = zeros(phi.shape[0])
    for j in range(0,phi.shape[0]):
        phir[j] = phi[j][0]
        phii[j] = phi[j][1]
        

    plt.quiver(x,y,phir,phii)
    plt.plot(x,y,'ro')
    plt.show()


def _plot_file_frame_pos(f,comp_number, fr_num):
    x = f["/frame%(#)06d"%{"#":fr_num}+"/x_%(#)07d"%{"#":comp_number}]
    y = f["/frame%(#)06d"%{"#":fr_num}+"/y_%(#)07d"%{"#":comp_number}]
    plt.plot(x,y,'ro')
    plt.show()



def make_2dv3d_plot(key,conn):
    # add error handling on all of these calls
    
    # get comp_number of gofr
    res = conn.execute("select comp_key,fout from comps where dset_key == ? and function == 'gofr'",(key,)).fetchall()
    (g_ck,g_fname) = res[0]
        
        
    # get comp_number of 3D gofr
    (g3D_ck,g3D_fname) = conn.execute("select comp_key,fout from comps where dset_key == ? and function == 'gofr3D'",(key,)).fetchone()


    # get dset name
    (sample_name, temp) = conn.execute("select sname,temp from dsets where key == ? ",(key,)).fetchone()

    print "g(r) key: " + str(g_ck)
    print "g(r)3D key: " + str(g3D_ck)
    print sample_name + " " + str(temp)
    print g_fname
    group = h5py.File(g_fname,'r')["gofr_%(#)07d"%{"#":g_ck}]
    print group.keys()


    group3D = h5py.File(g3D_fname,'r')["gofr3D_%(#)07d"%{"#":g3D_ck}]


    # make plot
    istatus = plt.isinteractive();
    if istatus:plt.ion()
    dset_names = ['bin_count', 'bin_edges']
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.hold(True)
    ax.plot(group[dset_names[1]][:]*6.45/60,group[dset_names[0]])
    ax.plot(group3D[dset_names[1]],group3D[dset_names[0]])
    ax.set_title(sample_name + " temp: " + str(temp))
    ax.legend(['2D','3D'])
    ax.grid(True)

    if istatus:
        plt.draw()
        plt.ion()


    pass
    # save figure
    


