
def check_comps_table(key,func,conn):
    # figure out name of file to write to
    res = conn.execute("select fout,comp_key from comps where dset_key=? and function=?;",(key,func)).fetchall()
    return res

def do_gofr3D(key,conn):
    prog_name = "gofr3D"

    # see if the file has already been processed
    res = check_comps_table(key,"link3D",conn)
    if len(res) ==0:
        print "no entry"
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    (fin,read_comp) = res[0]

    

    fout = os.path.dirname(fin) + '/gofr3D.h5'
    _make_sure_h5_exists(fout)
    
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    config = xml_data()
    config.add_stanza("gofr3D")
    config.add_pram("max_range","float","5")
    config.add_pram("nbins","int","2000")
    config.add_pram("grp_name","string",prog_name + "_%(#)07d"%{"#":comp_num})


def do_phi6(key,conn):
    prog_path = '/home/tcaswell/misc_builds/basic_dbg/apps/'
    prog_name = "phi6"

    # figure out name of file to write to
    res = check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        # _do_Iden(key,conn)
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    (fname,read_comp) = res[0]
    
    
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1
    
    
    #srange = raw_input("enter search range: ")
    srange = 13
    
    config = xml_data()
    config.add_stanza("phi6")
    config.add_pram("neighbor_range","float",srange)


    

def do_sofq(key,conn,rel = True):

    prog_name = "sofq"
    
    # see if the file has already been processed
    res = check_comps_table(key,"Iden",conn)
    if len(res) ==0:
        print "no entry"
        return
    if len(res) >1:
        print "more than one entry, can't cope, quiting"
        return
    (fin,read_comp) = res[0]
    

    fout = os.path.dirname(fin) + '/sofq.h5'
    _make_sure_h5_exists(fout)
    
    comp_num = conn.execute("select max(comp_key) from comps;").fetchone()[0] + 1

    config = xml_data()
    config.add_stanza("sofq")
    config.add_pram("nbins","int","200")
    
