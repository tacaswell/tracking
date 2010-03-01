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
        self.stanza = None
    def add_stanza(self,name):
        self.stanza = self.doc.createElement(name)
        self.doc.documentElement.appendChild(self.stanza)
    def add_pram(self,key,type,value,stanza=None):
        if not stanza == None:
            self.stanza = self.doc.getElementsByTagName(stanza).item(0)
            
        tmp_elm = self.doc.createElement("pram")
        tmp_elm.setAttribute('key',key)
        tmp_elm.setAttribute('type',type)
        tmp_elm.setAttribute('value',str(value))
        self.stanza.appendChild(tmp_elm)
        
        pass
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
        

def __main__():
    test = xml_data()
    test.add_stanza("iden")
    test.add_pram('a','int',1)
    test.add_pram('b','float',3.145)
    test.add_pram('c','string','gahh')
    test.disp()
    test.add_stanza("test")
    test.add_pram('d','int',2)
    test.add_pram('e','int',3)
    test.disp()
    test.add_pram('f','int',4,"iden")
    test.add_pram('g','int',5,"test")
    test.disp()

__main__()
