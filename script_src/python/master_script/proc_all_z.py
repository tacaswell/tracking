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

import cpp_wrapper
import sqlite3

def main_prog():
    conn = sqlite3.connect('/home/tcaswell/colloids/processed/processed_data.db')
    
    # get list of z types files
    z_dsets = conn.execute("select key from dsets where dtype = 'z'").fetchall()

    for d in z_dsets:
        key = d[0]
        # check to see if Iden has been done and do it
        res = cpp_wrapper.check_comps_table(key,'Iden',conn)
        if len(res) ==0:
            cpp_wrapper.do_Iden(key,conn)
            
        
        # check to see if gofr is done and do it
        res = cpp_wrapper.check_comps_table(key,'gofr',conn)
        if len(res) ==0:
            cpp_wrapper.do_gofr(key,conn)
        
        # check to see if link is done, and do it
        res = cpp_wrapper.check_comps_table(key,'link3D',conn)
        if len(res) ==0:
            cpp_wrapper.do_link3D(key,conn)
        # check to see if gofr3D is done and do it
        res = cpp_wrapper.check_comps_table(key,'gofr3D',conn)
        if len(res) ==0:
            cpp_wrapper.do_gofr3D(key,conn)
        pass
    pass


if __name__ == "__main__":
    main_prog()


