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


from numpy import *
import matplotlib.pyplot as plt
import random
import itertools
import h5py


def plot_file_frame(f,fr_num):
    x = f["/frame%(#)06d"%{"#":fr_num}+"/x"]
    y = f["/frame%(#)06d"%{"#":fr_num}+"/y"]
    plt.plot(x,y,'ro')
    plt.show()

f = h5py.File("../utilities/test.h5",'r')

plot_file_frame(f,0)
f.close()
