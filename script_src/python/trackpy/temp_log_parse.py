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

from datetime import datetime
from datetime import timedelta
from datetime import time
from datetime import date

from utils.SortedCollection import SortedCollection
import os

import h5py

class Temp_log:
    
    def __init__(self):
        """Takes in the file name of a file created by the NI temperature
        logger program and generates a Temp_log object."""

        

        self.units = None
        
        self.temp_collection = SortedCollection([],lambda x:x[0])
        

    def parse_line(self,strin):
        """Parse a single line of the log file """
        (tm,temp) = strin.split('\t')
        tm = datetime.strptime(tm.strip(),'%I:%M:%S %p').time()
        if tm < self.start_time:
             self.start_time = tm
             self.cur_date = self.cur_date + timedelta(days=1)
        temp = float(temp.strip())
        return (datetime.combine(self.cur_date,tm),temp)

    def _parseFile(self,fname):
        # open file to deal with it
        f = open(fname,'r')
        # get the start time and date
        tmp_ln = f.readline()
        init_time = datetime.strptime((":".join(tmp_ln.split(":")[1:])).strip(),'%m/%d/%Y %I:%M:%S %p')
        

        self.cur_date = init_time.date()
        self.start_time = init_time.time()

        # get units of temperature
        units = f.readline().split(":")[-1].strip()
        

        # jump over the other header stuff
        for j in range(0,6):
            f.readline()

        tmp  = [self.parse_line(ln) for ln in f]
        # clean up file
        f.close()
        
        return tmp,units
    
    def addFile(self,fname):
        """Adds the data from a file to this log """
        (tmp,units) = self._parseFile(fname)
        if (self.units is not None) and (units != self.units):
            print units
            print self.units
            raise Exception('units must match')

        tmp.extend(self.temp_collection._items)
        
        self.temp_collection = SortedCollection(tmp,self.temp_collection._getkey())
        
    def get_temp(self,in_time):
        """returns the temperature at the time asked"""

        try:
            temp =  self.temp_collection.find(in_time.replace(microsecond=0))[1]
        except ValueError,v:
            temp_m1 = self.temp_collection.find(in_time.replace(microsecond=0) - timedelta(seconds=1))[1]
            temp_p1 = self.temp_collection.find(in_time.replace(microsecond=0) + timedelta(seconds=1))[1]
            temp = (temp_p1 + temp_m1)/2
        return temp

    def addDir(self,path):
        """Adds all the log files in a directory to this object"""
        for root,dirs,files in os.walk(path):
            for f in files:
                if f[-5:] == '].log':
                    self.addFile(root+f)
        



def set_plane_temp(iden_key,conn,temp_log):
    """Takes an Iden_key, a database connection and a Temp_log object
    and sets the plane temperatures for the iden file"""

    (fname,) = conn.execute("select fout from iden where comp_key =?",(iden_key,)).fetchone()
    F = h5py.File(fname,'r+')
    
    for g in F:
        if g == 'parameters':
            break
        grp = F[g]
        acq_time = datetime.strptime('.'.join(grp.attrs['acquisition-time-local'].split('.')[:-1])
                                     ,'%Y-%m-%d %H:%M:%S')
        if 'temperature' in grp.attrs.keys():
            pass
        # F.close()
        #     del F
        #     raise Exception("This file already has temperatures for the planes")
        else:
            try:
                grp.attrs['temperature'] = temp_log.get_temp(acq_time)
            except Exception , inst:
                print inst.args
                print inst
                F.close()
                del F
    F.close()
    del F
    
