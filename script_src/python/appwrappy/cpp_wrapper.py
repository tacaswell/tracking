#Copyright 2010-2011 Thomas A Caswell
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

import h5py

import subprocess
import sqlite3
from .utils.xml_data import xml_data
from datetime import date
import temp_log_parse as tlp

_rel_path = "/home/tcaswell/misc_builds/basic_rel/apps/"
_dbg_path = "/home/tcaswell/misc_builds/basic_dbg/apps/"


def add_gofr_plane_mdata(comp_pram,i_pram,f_pram,s_pram,conn):
    """Writes the meta data for g(r) by plane to the table """
    prams = (comp_pram['dset'],comp_pram['write_comp'],i_pram['nbins'],
             f_pram['max_range'],i_pram['comp_count'],comp_pram['read_comp'])
    conn.execute("insert into gofr_by_plane_prams" +
                 " (dset_key,comp_key,'nbins','max_range','comp_count',iden_key) " +
                 "values (?,?,?,?,?,?);",prams)
    conn.commit()


def add_msd_mdata(comp_pram,i_pram,f_pram,s_pram,conn):
    params = (comp_pram['dset'],
              comp_pram['write_comp'],
              i_pram['msd_steps'],
              f_pram['search_range'],
              i_pram['min_track_length'],
              comp_pram['read_comp'])
    conn.execute("insert into msd_prams " +
                 "(dset_key,comp_key,msd_steps, search_range,min_track_length,iden_key) "+
                 "values (?,?,?,?,?,?)",params)
    conn.commit()

    
def add_trk_stat_mdata(comp_pram,i_pram,f_pram,s_pram,conn):
    params = (comp_pram['dset'],
              comp_pram['write_comp'],
              comp_pram['read_comp'],
              f_pram['search_range'],
              i_pram['steps'],
              i_pram['hist_bins'],
              f_pram['hist_range'],)
    conn.execute("insert into trk_stat_prams " +
                 "(dset_key,comp_key,iden_key,search_range,steps,hist_bins,hist_range) "+
                 "values (?,?,?,?,?,?,?)",params)
    conn.commit()


def add_tracking_mdata(comp_pram,i_pram,f_pram,s_pram,conn):
    params = (comp_pram['dset'],
              comp_pram['write_comp'],
              f_pram['search_range'],
              comp_pram['read_comp'])
    conn.execute("insert into tracking_prams " +
                 "(dset_key,comp_key,search_range,iden_key) "+
                 "values (?,?,?,?)",params)
    conn.commit()

def add_vanHove_mdata(comp_pram,i_pram,f_pram,s_pram,conn):
    params = (comp_pram['dset'],comp_pram['write_comp'],
              comp_pram['iden_read_comp'],comp_pram['track_read_comp'],
              i_pram['min_track_length'],i_pram['max_step'],
              f_pram['max_range'],i_pram['nbins'])
    conn.execute("insert into vanHove_prams " +
                 "(dset_key,comp_key,iden_key,track_key," +
                 "min_track_length,max_step,max_range,nbins) "+
                 "values (?,?,?,?,?,?,?,?)",params)
    conn.commit()


    
_prog_to_fun_lookup = {'gofr_by_plane':add_gofr_plane_mdata,'msd':add_msd_mdata,'track_stats':add_trk_stat_mdata,'tracking':add_tracking_mdata,'vanHove':add_vanHove_mdata}


def _make_sure_h5_exists(fname):
    proc_path = '/'.join(fname.split('/')[:-1])
    if not os.path.exists(proc_path):
        os.makedirs(proc_path,0751)
    if not os.path.isfile(fname):
        f = h5py.File(fname)
        f.close()


def do_link3D(comp_key,conn,pram, fout_name):
    prog_name = "link3D"
    req_f_pram = ["box_side_len", "search_range"    ]
    req_i_pram = ["min_trk_len"]
    opt_pram_f = ['xy_scale','z_spacing']

    
    
    res = conn.execute("select fout,dset_key from iden where comp_key=? ;",
                       (comp_key,)).fetchone()
    read_comp = comp_key
    
    
    print res
    print comp_key
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,key) = res
    
    fout = os.path.dirname(fin) +'/'+ fout_name
    
    
    comp_prams = {'iden_key':read_comp,'dset_key':key,'link_key':0}
        
    
    pram_s = {'grp_name':prog_name}



    
    
    try:
        _call_fun_no_sql( prog_name,fin,fout,
                          comp_prams,
                          req_i_pram,pram,
                          req_f_pram,pram,
                          opt_f_pram = opt_pram_f)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'
    
    

    


def do_Iden(key,conn,iden_params,TL):
    # see if the file has already been processed
    prog_name = "Iden"
    prog_path = "/home/tcaswell/misc_builds/iden_rel/iden/apps/"
    res = conn.execute("select fout,comp_key from iden where dset_key=?;",(key,)).fetchall()
    (fin,ftype,mtype) = conn.execute("select fname,ftype,mtype from dsets where dset_key = ?;",(key,)).fetchone()

    
    if os.path.isfile(fin.replace('.tif','-file002.tif')):
        print "multi-part tiff, can't cope yet"
        return
    
    
    if ftype == 1:
        format_string = 'file_name'
        fout = '.'.join(fin.replace("data","processed").split('.')[:-1]) + '.h5'
    elif ftype == 2:
        format_string = 'base_name'
        fout = fin.replace("data","processed") + '.h5'
    else:
        print "unknown format type"
        return

    
    
    if len(res) >0:
        fout = fout.replace(".h5","-" + str(len(res)) + ".h5")


    ## fpram = fin.replace(".tif",".xml")
    ## if not os.path.isfile(fpram):
    ##     print "can not find parameter file, exiting"
    ##     print fin
    ##     print fpram
    ##     return

    
    comp_key = conn.execute("select max(comp_key) from comps;").fetchone()[0]
    if comp_key is None:
        comp_key = 0
    comp_key+=1
    
    prams = xml_data()
    prams.add_stanza("comp")
    prams.add_pram("number","int",comp_key)
    prams.add_pram("dset_key","int",key)
    prams.add_pram("md_format","int",mtype)
    prams.add_pram(format_string,"string",fin)
    #prams.merge_File(fpram,"iden")
    
    
    int_params = ["p_rad",'d_rad','mask_rad']
    float_params = ['threshold','hwhm','shift_cut','rg_cut','e_cut','top_cut']

    prams.add_stanza("iden")
    for ip in int_params:
        prams.add_pram(ip,'int',str(iden_params[ip]))
    for fp in float_params:
        prams.add_pram(fp,'float',str(iden_params[fp]))
    
    
    
    fxml = prams.write_to_tmp()

    prams.disp()
    # prams.write_to_disk('/home/tcaswell/misc_builds/iden_dbg/iden/apps/pram2.xml')
    # ask for confirmation
    print "will processess"
    print fin

    print "will write to"
    print fout
    # resp = raw_input("is this ok?: ")
    # if resp[0]=='n':
    #     print "you have chosen not to continue"
    #     return

    

    proc_path = os.path.dirname(fout)  + '/'
    if not os.path.exists(proc_path):
        os.makedirs(proc_path,0751)



    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout,'-c',fxml ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        
        print "entering into database"
        conn.execute("insert into comps (dset_key,func_key) values (?,?);",(key,1))
        
        p = (key,
             comp_key,
             iden_params["threshold"],
             iden_params["top_cut"],
             iden_params["p_rad"],
             iden_params["hwhm"],
             iden_params["d_rad"],
             iden_params["mask_rad"],
             1,
             fout
             )
        
        conn.execute("insert into iden "+
                     "(dset_key,comp_key,threshold,top_cut,p_rad,hwhm,d_rad,mask_rad,frames_avged,fout) "+
                     "values (?,?,?,?,?,?,?,?,?,?);"
                     ,p)
        conn.commit()
        if(TL is not None):
            tlp.set_plane_temp(comp_key,conn,TL)


def do_Iden_avg(key,conn,frames,iden_params,TL):
    # see if the file has already been processed
    prog_name = "Iden_avg"
    prog_path = "/home/tcaswell/misc_builds/iden_rel/iden/apps/"
    
    res = conn.execute("select fout,comp_key from iden where dset_key=?;",(key,)).fetchall()
    (fin,ftype) = conn.execute("select fname,ftype from dsets where dset_key = ?;",(key,)).fetchone()

    
    if os.path.isfile(fin.replace('.tif','-file002.tif')):
        print "multi-part tiff, can't cope yet"
        return
    

    if ftype == 1:
        format_string = 'file_name'
        fout = '.'.join(fin.replace("data","processed").split('.')[:-1]) + '.h5'
    elif ftype == 2:
        format_string = 'base_name'
        fout = fin.replace("data","processed") + '.h5'
    else:
        print "unknown format type"
        return
    
    
    if len(res) >0:
        fout = fout.replace(".h5","-" + str(len(res)) + ".h5")


    ## fpram = fin.replace(".tif",".xml")
    ## if not os.path.isfile(fpram):
    ##     print "can not find parameter file, exiting"
    ##     print fin
    ##     print fpram
    ##     return

    comp_key = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    prams = xml_data()
    prams.add_stanza("comp")
    prams.add_pram("number","int",comp_key)
    prams.add_pram("dset_key","int",key)
    prams.add_pram(format_string,"string",fin)
    #prams.merge_File(fpram,"iden")
    
    int_params = ["p_rad",'d_rad','mask_rad']
    float_params = ['threshold','hwhm','shift_cut','rg_cut','e_cut','top_cut']

    prams.add_stanza("iden")
    for ip in int_params:
        prams.add_pram(ip,'int',str(iden_params[ip]))
    for fp in float_params:
        prams.add_pram(fp,'float',str(iden_params[fp]))
           
    prams.add_pram("avg_count","int",str(frames))
    
    fxml = prams.write_to_tmp()

    prams.disp()
    # prams.write_to_disk('/home/tcaswell/misc_builds/iden_dbg/iden/apps/pram2.xml')
    # ask for confirmation
    print "will processess"
    print fin
    ## print "from parameters in"
    ## print fpram
    print "will write to"
    print fout
    # resp = raw_input("is this ok?: ")
    # if resp[0]=='n':
    #     print "you have chosen not to continue"
    #     return

    

    proc_path = os.path.dirname(fout)  + '/'
    if not os.path.exists(proc_path):
        os.makedirs(proc_path,0751)

    ## parse1.make_h5(fin,fout)

    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout,'-c',fxml ])
    print rc

    # if it works, then returns zero
    if rc == 0:
        
        print "entering into database"
        conn.execute("insert into comps (dset_key,func_key) values (?,?);",(key,1))
        
        p = (key,
             comp_key,
             iden_params["threshold"],
             iden_params["top_cut"],
             iden_params["p_rad"],
             iden_params["hwhm"],
             iden_params["d_rad"],
             iden_params["mask_rad"],
             frames,
             fout,
             date.today().isoformat()
             )
        
        conn.execute("insert into iden "+
                     "(dset_key,comp_key,threshold,top_cut,p_rad,hwhm,d_rad,mask_rad,frames_avged,fout,date) "+
                     "values (?,?,?,?,?,?,?,?,?,?,?);"
                     ,p)
        conn.commit()
        tlp.set_plane_temp(comp_key,conn,TL)
    
    

def do_gofr(comp_key,conn,pram_i, pram_f, pram_s = None, rel = True,):
    """
    Computes gofr 

    """
    prog_name = "gofr"
    required_pram_i = ['nbins']
    required_pram_f = ['max_range']
    required_pram_s = ['grp_name']
    opt_pram_f = ['e_cut','rg_cut','shift_cut']
    
    
    res = conn.execute("select fout,dset_key from iden where comp_key=? ;",
                       (comp_key,)).fetchone()
    read_comp = comp_key
    
    print res
    print comp_key
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,key) = res
    
    fout = os.path.dirname(fin) + '/gofr.h5'
    _make_sure_h5_exists(fout)
    
    comp_prams = {'iden_key':read_comp,'dset_key':key}
        
    if pram_s is None:
        pram_s = {'grp_name':prog_name}



    
    try:
        _call_fun_no_sql(prog_name,fin,fout,
                         comp_prams,
                         required_pram_i,pram_i,
                         required_pram_f,pram_f,
                         required_pram_s,pram_s,
                         opt_f_pram = opt_pram_f)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'


def do_phi6(comp_key,conn,pram, rel = True,):
    """
    Computes gofr 

    """
    prog_name = "phi6"
    required_pram_i = []
    required_pram_f = ['neighbor_range']
    required_pram_s = ['grp_name']
    opt_pram_f = ['e_cut','rg_cut','shift_cut']
    
    
    res = conn.execute("select fout,dset_key from iden where comp_key=? ;",
                       (comp_key,)).fetchone()
    read_comp = comp_key
    
    print res
    print comp_key
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,key) = res
    
    fout = fin
    _make_sure_h5_exists(fout)
    
    comp_prams = {'iden_key':read_comp,'dset_key':key}
        
    
    pram_s = {'grp_name':prog_name}



    
    try:
        _call_fun_no_sql(prog_name,fin,fout,
                         comp_prams,
                         required_pram_i,pram,
                         required_pram_f,pram,
                         required_pram_s,pram_s,
                         opt_f_pram = opt_pram_f)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'

    
def do_msd(comp_key,conn,pram_i, pram_f, pram_s = None, rel = True,):
    """
    Computes gofr 

    """
    prog_name = "msd"
    required_pram_i = ['msd_steps','min_track_length']
    required_pram_f = ['search_range','box_side_len']
    required_pram_s = None

    if pram_i['msd_steps'] > pram_i['min_track_length']:
        raise Exception("The max step must be less than the minimum track"+
                    " length due to population selection issues")


    # see if the file has already been processed
    res = conn.execute("select fout,dset_key from comps where comp_key=? ;",
                       (comp_key,)).fetchone()
    read_comp = comp_key
    
    print res
    print comp_key
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,key) = res

    fout = os.path.dirname(fin) + '/msd.h5'
    
    
    comp_prams = {'read_comp':read_comp,'dset':key}
    comp_prams['write_comp'] =conn.execute("select max(comp_key) from comps;"
                                           ).fetchone()[0] + 1

    try:
        _call_fun(conn,
                  prog_name,fin,fout,
                  comp_prams,
                  required_pram_i,pram_i,
                  required_pram_f,pram_f,
                  required_pram_s,pram_s)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'


def do_msd_sweep(track_key,conn,pram_i,pram_f=None,pram_s =None,rel = True):
    """
    computes msd for a range of minimum track lengths
    """
    prog_name = "msd_sweep"
    required_pram_i = ['trk_len_min','trk_len_step','steps']
    required_pram_f = None
    required_pram_s = None
    opt_pram_f = ['n_range']
    opt_pram_i = ['n_max','n_min']
    
    # see if the file has already been processed
    res = conn.execute("select fout,iden_key,dset_key from tracking where comp_key=? ;",
                       (track_key,)).fetchone()
    
    
    print res
    
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,iden_key,dset_key) = res
    fout = os.path.dirname(fin) + '/msd_sweep.h5'
    comp_prams = {'iden_key':iden_key,'dset_key':dset_key,'track_key':track_key}
    
    
    
    
    
    try:
        _call_fun_no_sql( prog_name,fin,fout,
                          comp_prams,
                          required_pram_i,pram_i,
                          required_pram_f,pram_f,
                          required_pram_s,pram_s,
                          opt_f_pram = opt_pram_f,opt_i_pram = opt_pram_i)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'


def do_vanHove_sweep(track_key,conn,pram_i,pram_f,pram_s =None,rel = True):
    """
    computes msd for a range of minimum track lengths
    """
    prog_name = "vanHove_sweep"
    required_pram_i = ['trk_len_min','trk_len_step','steps','nbins']
    required_pram_f = ['max_range']
    required_pram_s = None
    
    # see if the file has already been processed
    res = conn.execute("select fout,iden_key,dset_key from tracking where comp_key=? ;",
                       (track_key,)).fetchone()
    
    
    print res
    
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,iden_key,dset_key) = res
    fout = os.path.dirname(fin) + '/vanHove.h5'
    comp_prams = {'iden_key':iden_key,'dset_key':dset_key,'track_key':track_key}
    
    
    
    
    
    try:
        _call_fun_no_sql( prog_name,fin,fout,
                  comp_prams,
                  required_pram_i,pram_i,
                  required_pram_f,pram_f,
                  required_pram_s,pram_s)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'


    

def do_gofr_by_plane(comp_key,conn,pram_i, pram_f, pram_s = None, rel = True,):
    prog_name = "gofr_by_plane"
    required_pram_i = ['nbins']
    required_pram_f = ['max_range']
    required_pram_s = ['grp_name']
    opt_pram_f = ['e_cut','rg_cut','shift_cut']
    opt_pram_i = ['comp_count','frames_per_comp']
    # see if the file has already been processed
    
    
    res = conn.execute("select fout,dset_key from iden where comp_key=? ;",
                       (comp_key,)).fetchone()
    read_comp = comp_key
    
    print res
    print comp_key
    if res is None:
        raise utils.dbase_error('no entry found')
    
    (fin,key) = res
    
    fout = os.path.dirname(fin) + '/gofr_by_planes.h5'
    _make_sure_h5_exists(fout)
    

    comp_prams = {'iden_key':comp_key,'dset_key':key}
    
    
    if pram_s is None:
        pram_s = {'grp_name':prog_name}


    try:
        _call_fun_no_sql(
            prog_name,fin,fout,
            comp_prams,
            required_pram_i,pram_i,
            required_pram_f,pram_f,
            required_pram_s,pram_s,
            opt_f_pram = opt_pram_f,opt_i_pram = opt_pram_i)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'
    


def do_track_stat(comp_key,conn,pram_i, pram_f, pram_s = None, rel = True,):
    prog_name = "track_stats"
    required_pram_i = ['steps','hist_bins']
    required_pram_f = ['search_range','box_side_len','hist_range']
    required_pram_s = None

    (fin,dset_key) = _get_fin(comp_key,conn)
    
    fout = os.path.dirname(fin) + '/track_stats.h5'
    
    
    comp_prams = {'read_comp':comp_key,'dset':dset_key}
    comp_prams['write_comp'] =conn.execute("select max(comp_key) from comps;"
                                           ).fetchone()[0] + 1

    try:
        _call_fun(conn,
                  prog_name,fin,fout,
                  comp_prams,
                  required_pram_i,pram_i,
                  required_pram_f,pram_f,
                  required_pram_s,pram_s)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'



def do_tracking(comp_key,conn,pram_i, pram_f, pram_s = None, rel = True,db_path = None):
    prog_name = "tracking"
    required_pram_i = []
    required_pram_f = ['search_range']
    required_pram_s = None
    opt_pram_f = ['e_cut','rg_cut','shift_cut','I_min_cut']
    (fin,dset_key) = conn.execute("select fout,dset_key from iden where comp_key = ?",
                                  (comp_key,)).fetchone()
    
    fout = fin
    
    
    comp_prams = {'iden_key':comp_key,'dset_key':dset_key}
    comp_prams['write_comp'] =conn.execute("select max(comp_key) from comps;"
                                           ).fetchone()[0] + 1
    
    
    try:
        _call_fun_no_sql(prog_name,
                         fin,fout,
                         comp_prams,
                         required_pram_i,pram_i,
                         required_pram_f,pram_f,
                         required_pram_s,pram_s,
                         opt_f_pram=opt_pram_f,
                         db_path = db_path)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'


def do_vanHove(track_key,conn,pram_i, pram_f, pram_s = None, rel = True,):
    prog_name = "vanHove"
    required_pram_i = ['nbins','max_step','min_track_length']
    required_pram_f = ['max_range']
    required_pram_s = None
    
    if pram_i['max_step'] > pram_i['min_track_length']:
        raise Exception("The max step must be less than the minimum track"+
                    " length due to population selection issues")
    
    
    # see if the file has already been processed
    res = conn.execute("select fout,iden_key,dset_key from tracking where comp_key=? ;",
                       (track_key,)).fetchone()
    
    
    print res
    
    if res is None:
        raise utils.dbase_error('no entry found')

    (fin,iden_key,dset_key) = res
    
    fout = os.path.dirname(fin) + '/vanHove.h5'
    comp_prams = {'iden_key':iden_key,'dset_key':dset_key,'track_key':track_key}    
    
    

    try:
        _call_fun_no_sql(prog_name,
                         fin,fout,
                         comp_prams,
                         required_pram_i,pram_i,
                         required_pram_f,pram_f,
                         required_pram_s,pram_s,
                         opt_f_pram=opt_pram_f,
                         db_path = db_path)
    except KeyError, ke:
        print "Parameter: " ,ke,' not found'




    
def _call_fun(conn,
                prog_name,fin,fout,
                comp_pram,
                req_i_pram,
                i_pram,
                req_f_pram,
                f_pram,
                req_s_pram = None,
                s_pram = None,
                rel = True):
    """
    prog_name : the name of the analysis program to be called
    
    fin : the hdf file to be read into
    
    fout : the hdf file to write the results to
    
    comp_pram : a dictionary of the computation/logistic parameters
    
    req_[ifs]_pram : list of required [integer,float,string]
                     parameters to be passed to the program, the
                     string list is optional
    
    [ifs]_pram : dictionary of [integer,float,string] parameters to be
                 passed to the program, must include the parameters in
                 the req_[ifs]_pram list.  The string dictionary is
                 optional.
             
    rel : bool that sets if debugging or release version of the program 

    """
    
    
    
    config = xml_data()
    
    config.add_stanza(prog_name)
    if req_i_pram is not None :
        for p in req_i_pram:
            config.add_pram(p,'int',i_pram[p])
    if req_f_pram is not None :
        for p in req_f_pram:
            config.add_pram(p,'float',f_pram[p])
    if req_s_pram is not None :
        for p in req_s_pram:
            config.add_pram(p,'string',s_pram[p])

    
    config.add_stanza("comps")
    
    for p in comp_pram:
            config.add_pram(p,'int',comp_pram[p])

    config.disp()
    cfile = config.write_to_tmp()

    print fin
    print fout

    if rel: prog_path = _rel_path
    else: prog_path = _dbg_path
    rc = subprocess.call(["time",prog_path + prog_name,'-i',fin,'-o',fout, '-c',cfile ])
    print rc

    # if it works, then returns zero
    #if False:
    if rc == 0:
        # look up function key
        (func_key,) = conn.execute("select func_key from func_names where func_name = ?",(prog_name,))
        print "entering into database"
        conn.execute("insert into comps (comp_key,dset_key,func_key) "
                     + "values (?,?,?);",
                     (comp_pram['write_comp'],comp_pram['dset'],func_key)
                     )
        
        if prog_name in _prog_to_fun_lookup:
            _prog_to_fun_lookup[prog_name](comp_pram,i_pram,f_pram,s_pram,conn)
        
        conn.commit()
    else:
        print "ERROR!!!"

def _call_fun_no_sql(prog_name,fin,fout,
                     comp_pram,
                     req_i_pram,
                     i_pram,
                     req_f_pram,
                     f_pram,
                     req_s_pram = None,
                     s_pram = None,
                     db_path = None,
                     rel = True,
                     opt_f_pram = None,
                     opt_i_pram = None
                     ):
    """
    prog_name : the name of the analysis program to be called
    
    fin : the hdf file to be read into
    
    fout : the hdf file to write the results to
    
    comp_pram : a dictionary of the computation/logistic parameters
    
    req_[ifs]_pram : list of required [integer,float,string]
                     parameters to be passed to the program, the
                     string list is optional
    
    [ifs]_pram : dictionary of [integer,float,string] parameters to be
                 passed to the program, must include the parameters in
                 the req_[ifs]_pram list.  The string dictionary is
                 optional.
             
    rel : bool that sets if debugging or release version of the program 

    """
    
    if db_path is None:
        db_path = '/home/tcaswell/colloids/proc_db.db'
    
    config = xml_data()
    
    config.add_stanza(prog_name)
    if req_i_pram is not None :
        print i_pram
        for p in req_i_pram:
            config.add_pram(p,'int',i_pram[p])
    if req_f_pram is not None :
        for p in req_f_pram:
            config.add_pram(p,'float',f_pram[p])
    if req_s_pram is not None :
        for p in req_s_pram:
            config.add_pram(p,'string',s_pram[p])
    if opt_f_pram is not None :
        for p in opt_f_pram:
            if p in f_pram:
                config.add_pram(p,'float',f_pram[p])
    if opt_i_pram is not None :
        for p in opt_i_pram:
            if p in i_pram:
                config.add_pram(p,'int',f_pram[p])
    
    config.add_stanza("comps")
    
    for p in comp_pram:
            config.add_pram(p,'int',comp_pram[p])


    config.add_stanza("files")
    config.add_pram('db_path','string',db_path)
    config.add_pram('fin','string',fin)
    config.add_pram('fout','string',fout)
    config.disp()
    cfile = config.write_to_tmp()

    print fin
    print fout

    if rel: prog_path = _rel_path
    else: prog_path = _dbg_path
    rc = subprocess.call(["time",prog_path + prog_name,'-c',cfile ])
    print rc
    




    
def _get_fin(comp_key,conn):
    
    res = conn.execute("select fout,dset_key from comps where comp_key=? ;",
                       (comp_key,)).fetchone()
    print res
    print comp_key
    if res is None:
        raise utils.dbase_error('no entry found')
    
    return res
