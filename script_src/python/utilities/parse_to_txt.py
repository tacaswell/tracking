#!/usr/bin/python

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


import xml.dom.minidom
import subprocess
import datetime
import os.path
import sys
import argparse

def _write(file,name,val):
    if name == "acquisition-time-local" or name == "modification-time-local":
        tmp = int(val[18:])
        val = val[:18] + "%(#)03d"%{"#":tmp}
    file.write(name + ": " + str(val) + "\n")


def _start_group(f,n):
    f.write("Frame " + str(n) + "\n")


def _end_group(f):
    f.write("\n\n")
    
    
def _parse_attr(file_obj,dom_obj):
    if dom_obj.getAttribute("id") =="Description":
        _parse_des(file_obj,dom_obj)
    elif dom_obj.getAttribute("type") =="int":
        _write(file_obj,dom_obj.getAttribute("id"),int(dom_obj.getAttribute("value")))
    elif  dom_obj.getAttribute("type") =="float":
        _write(file_obj,dom_obj.getAttribute("id"),float(dom_obj.getAttribute("value")))
    else: 
        _write(file_obj,dom_obj.getAttribute("id"), dom_obj.getAttribute("value").encode('ascii'))

def _parse_des(file_obj,des_obj):
    des_string = des_obj.getAttribute("value")
    des_split = des_string.split("&#13;&#10;")

    for x in des_split:
        tmp_split = x.split(":")
        if len(tmp_split) ==2:
            _write(file_obj,tmp_split[0],tmp_split[1].encode('ascii'))

def _parse_params_attr(file_obj,i_str):
    ssplit = i_str.strip().split(':')

    if len(ssplit)==3:
        print ssplit
    elif ssplit[0].strip()=='p_rad' or ssplit[0].strip()=='d_rad'or ssplit[0].strip()=='mask_rad':
        _write(file_obj,ssplit[0].strip(),int(float(ssplit[1].strip())))
        print ssplit
    elif ssplit[0].strip()=='threshold'or ssplit[0].strip()=='hwhm' or ssplit[0].strip()=='shift_cut' or ssplit[0].strip()=='rg_cut' or ssplit[0].strip()=='e_cut':
        _write(file_obj,ssplit[0].strip(),float(ssplit[1].strip()))
        print ssplit
    else:
        print ssplit[0].strip()


def _parse_params(file_obj,fname):
    f = open(fname)
    for line in f:
        _parse_params_attr(file_obj,line)
    f.close()
    n_fname  = fname[:(len(fname)-5)] + ".done"
    os.rename(fname,n_fname)

def _parse_temp(file_obj,fname):
    rexp = re.compile('\d\d-\d')
    ma = rexp.findall(fname)
    if(len(ma)>=1):
        _write(file_obj,'probe_temp',float('.'.join(ma[0].split('-'))))
    if(len(ma)==2):
        _write(file_obj,'lens_temp',float('.'.join(ma[0].split('-'))))

def parse_file(fname,f):




    

    # make sure the files exist
    if not (os.path.exists(fname) ):
        print "file does not exist"
        exit()
    




    #changed to deal with 2.5v2.6
    #g = f.create_group("frame{0:06d}".format(0))

    _start_group(f,0)

    a = subprocess.Popen(["tiffinfo","-0",fname] ,stdout=subprocess.PIPE)
    tiff_string = (a.stdout).readlines();
    tiff_string = "".join(tiff_string)

    xml_str = tiff_string[(tiff_string.find("<MetaData>")):(10 + tiff_string.rfind("MetaData>"))]
    dom = xml.dom.minidom.parseString(xml_str)

    props = dom.getElementsByTagName("prop")
    
    for p in props:
        if p.parentNode.nodeName == "PlaneInfo":
            _parse_attr(f,p)
            if p.getAttribute("id") == "acquisition-time-local":
                tmp = p.getAttribute("value")
                initial_t  = datetime.datetime.strptime(tmp[:17],"%Y%m%d %H:%M:%S") 
                initial_t.replace(microsecond=int(tmp[18:])*1000)


        elif p.parentNode.nodeName == "MetaData":
            _parse_attr(f,p)
        elif p.parentNode.nodeName == "SetInfo":
            _parse_attr(f,p)
            if p.getAttribute("id") == "number-of-planes":
                frame_count = int(p.getAttribute("value"))

    _write(f,"dtime",0.0)

    _end_group(f)
    
    

    

    for frame in range(1,frame_count):
        _start_group(f,frame)
        a = subprocess.Popen(["tiffinfo","-"+str(frame),fname] ,stdout=subprocess.PIPE)
        tiff_string = (a.stdout).readlines();
        tiff_string = "".join(tiff_string)
        xml_str = tiff_string[(tiff_string.find("<MetaData>")):(10 + tiff_string.rfind("MetaData>"))]
        dom = xml.dom.minidom.parseString(xml_str)
        props = dom.getElementsByTagName("prop")



        for p in props:
            if p.parentNode.nodeName == "PlaneInfo":
                _parse_attr(f,p)
                if p.getAttribute("id") == "acquisition-time-local":
                    tmp = p.getAttribute("value")
                    current_t = datetime.datetime.strptime(tmp[:17],"%Y%m%d %H:%M:%S") 
                    current_t = current_t.replace(microsecond=int(tmp[18:])*1000)
        dt = current_t - initial_t
        _write(f,"dtime",dt.seconds + dt.microseconds/(pow(10.,6)))
        initial_t = current_t
        _end_group(f)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("fname_in",help="The file name of the file to strip MD from")
    parser.add_argument("--fout",help="Name of file to write results to")
    args = parser.parse_args()
    if args.fout is None:
        f = sys.stdout
    else:
        f = open(args.fout,'w')




    parse_file(args.fname_in,f)

    if args.fout is not None:
        f.close()
        
if __name__ == "__main__":
    main()
