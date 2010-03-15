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


import os
import os.path

import parse1

import h5py

import subprocess
import sqlite3
from xml_data import xml_data
from datetime import date


def _check_comps_table(key,func,conn):
    # figure out name of file to write to
    res = conn.execute("select fout,comp_key from comps where dset_key=? and function=?;",(key,func)).fetchall()
    return res


def _make_sure_h5_exists(fname):
    if not os.path.isfile(fname):
        f = h5py.File(fname)
        f.close()


def do_link3D(key,conn):
    prog_path = "/home/tcaswell/misc_builds/basic_dbg/apps/"
    prog_name = "link3D"
    # figure out name of file to write to
    res = _check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        # _do_Iden(key,conn)
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    (fname,read_comp) = res[0]

    
    
    # see if there is already a linked file
    res = conn.execute("select fout from comps where dset_key=? and function='link3D';",
                       (key,)).fetchall()
    if len(res)>0:
        print "Already linked"
        return

    
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1
    
    
    config = xml_data()
    config.add_stanza("link3D")
    config.add_pram("box_side_len","float","4");
    config.add_pram("search_range","float","3.5")
    config.add_pram("min_trk_len","int","3")
    config.add_stanza("comps")
    config.add_pram("read_comp","int",str(read_comp))
    config.add_pram("write_comp","int",str(comp_num))
    config.disp()
    cname = config.write_to_tmp()

    fout = fname.replace(".h5","_link.h5")

    # look
    
    print fname
    print fout


    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fname,'-o',fout, '-c',cname ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (dset_key,date,fin,fout,function) values (?,?,?,?,?);",
                     (key,date.today().isoformat(),fname,fout,prog_name))
        conn.commit()



def do_Iden(key,conn):
    # see if the file has already been processed
    prog_path = '/home/tcaswell/misc_builds/iden_dbg/iden/apps/'
    prog_name = "Iden"
    res = _check_comps_table(key,"Iden",conn)
    if len(res) >0:
        print "already processed, flipping out"
        return

    fin = conn.execute("select fname from dsets where key = ?;",(key,)).fetchone()[0]
    fout = '.'.join(fin.replace("data","processed").split('.')[:-1]) + '.h5'
    fpram = fin.replace("tif","xml")
    if not os.path.isfile(fpram):
        print "can not find parameter file, exiting"
        return

    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1


    prams = xml_data()
    prams.add_stanza("comp")
    prams.add_pram("number","int",comp_num)
    prams.merge_File(fpram,"iden")
    
    fxml = prams.write_to_tmp()

    prams.disp()
    prams.write_to_disk('/home/tcaswell/misc_builds/iden_dbg/iden/apps/pram2.xml')
    # ask for confirmation
    print "will processess"
    print fin
    print "from parameters in"
    print fpram
    print "will write to"
    print fout
    # resp = raw_input("is this ok?: ")
    # if resp[0]=='n':
    #     print "you have chosen not to continue"
    #     return

    

    # format name
    # funny string stuff to match parse1 calls
    base_name = '.'.join(os.path.basename(fin).split('.')[:-1])
    data_path = os.path.dirname(fin)  + '/'
    proc_path = os.path.dirname(fout)  + '/'
    if not os.path.exists(proc_path):
        os.makedirs(proc_path,0751)

    parse1.make_h5(base_name,data_path,proc_path)

    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout,'-c',fxml ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (dset_key,date,fin,fout,function) values (?,?,?,?,?);",
                     (key,date.today().isoformat(),fin,fout,prog_name))
        conn.commit()


def do_gofr3D(key,conn):
    prog_path = '/home/tcaswell/misc_builds/basic/apps/'
    prog_name = "gofr3D"

    # see if the file has already been processed
    res = _check_comps_table(key,"link3D",conn)
    if len(res) ==0:
        print "no entry"
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    fin = res[0][0]

    fout = os.path.dirname(fin) + '/gofr3D.h5'
    _make_sure_h5_exists(fout)
    
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    config = xml_data()
    config.add_stanza("gofr3D")
    config.add_pram("max_range","float","9.5")
    config.add_pram("nbins","int","2000")
    config.add_pram("grp_name","string",str(comp_num))
    config.disp()
    cfile = config.write_to_tmp()
    
    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout, '-c',cfile ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (comp_key,dset_key,date,fin,fout,function)"
                     +"values (?,?,?,?,?,?);",
                     (comp_num,key,date.today().isoformat(),fin,fout,prog_name))
        conn.commit()

def do_gofr(key,conn):

    prog_path = '/home/tcaswell/misc_builds/basic_dbg/apps/'
    prog_name = "gofr"

    # see if the file has already been processed
    res = _check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    (fin,read_comp) = res[0]
    

    fout = os.path.dirname(fin) + '/gofr.h5'
    _make_sure_h5_exists(fout)
    
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    config = xml_data()
    config.add_stanza("gofr")
    config.add_pram("max_range","float","100")
    config.add_pram("nbins","int","2000")
    config.add_pram("grp_name","string",prog_name + "_%(#)07d"%{"#":comp_num})
    config.add_stanza("comps")
    config.add_pram("read_comp","int",str(read_comp))
    config.add_pram("write_comp","int",str(comp_num))
    config.disp()
    cfile = config.write_to_tmp()
    print fin
    print fout

    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout, '-c',cfile ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (comp_key,dset_key,date,fin,fout,function)"
                     +"values (?,?,?,?,?,?);",
                     (comp_num,key,date.today().isoformat(),fin,fout,prog_name))
        conn.commit()
    else:
        print "ERROR!!!"
    

def do_tracking(key,conn):
    prog_path = '/home/tcaswell/misc_builds/basic_git/apps/'
    prog_name = "tracking"

    # figure out name of file to write to
    res = _check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        # _do_Iden(key,conn)
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    fname = res[0][0]
    

    srange = raw_input("enter search range: ")
    
    config = xml_data()
    config.add_stanza("tracking")
    config.add_pram("search_range","float",srange)
    config.add_pram("box_side_len","float",srange)
    config.add_pram("min_trk_len","int","10")
    config.disp()
    cname = config.write_to_tmp()

        
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    fout = fname.replace(".h5","_tracks_" + str(comp_num) + ".h5")




    # look
    
    print fname
    print fout


    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fname,'-o',fout, '-c',cname ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (dset_key,date,fin,fout,function) values (?,?,?,?,?);",
                     (key,date.today().isoformat(),fname,fout,prog_name))
        conn.commit()

    
    pass




def do_phi6(key,conn):
    prog_path = '/home/tcaswell/misc_builds/basic/apps/'
    prog_name = "phi6"

    # figure out name of file to write to
    res = _check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        # _do_Iden(key,conn)
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    (fname,read_comp) = res[0]
    

    srange = raw_input("enter search range: ")
    
    config = xml_data()
    config.add_stanza("phi6")
    config.add_pram("neighbor_range","float",srange)
    config.add_pram("grp_name","string","frame")
    config.disp()
    cname = config.write_to_tmp()

        
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    fout = fname
    # look
    
    print fname
    print fout


    
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fname,'-o',fout, '-c',cname ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        print "entering into database"
        conn.execute("insert into comps (dset_key,date,fin,fout,function) values (?,?,?,?,?);",
                     (key,date.today().isoformat(),fname,fout,prog_name))
        conn.commit()

    
    pass

