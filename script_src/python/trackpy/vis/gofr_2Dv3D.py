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
import h5py


def main_prog():
    conn = sqlite3.connect('/home/tcaswell/colloids/processed/processed_data.db')
    
    # ask for key?

def make_plot(key,conn):
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
    dset_names = ['bin_count', 'bin_edges']
    plt.figure()
    plt.hold(True)
    plt.plot(group[dset_names[1]][:]*6.45/60,group[dset_names[0]])
    plt.plot(group3D[dset_names[1]],group3D[dset_names[0]])

    plt.title(sample_name + " temp: " + str(temp))
    plt.legend(['2D','3D'])
    plt.grid(True)
    # save plot
    
    pass

    

 

if __name__ =='__main__':
    main_prog()
