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
import xml.dom.minidom
import tempfile
import os
import os.path

class xml_data:
    def __init__(self):
        self.doc = xml.dom.minidom.getDOMImplementation(None).createDocument(None,"root",None)
        self.fname = None
    def add_elm(self,elm,params):
        tmpelm = self.doc.createElement(elm)
        for a,b in params:
            tmpelm.setAttribute(a,b)
        self.doc.documentElement.appendChild(tmpelm)
    def write_to_file(self):
        tf = tempfile.mkstemp()
        os.close(tf[0])
        f = open(tf[1],'w')
        self.doc.writexml(f,addindent='   ',newl='\n')
        f.close()
        self.fname = tf[1]
        return self.fname
    def disp(self):
        print self.doc.toprettyxml()

    def __del__(self):
        if self.fname:
            if os.path.isfile(self.fname):
                os.remove(self.fname)
        

