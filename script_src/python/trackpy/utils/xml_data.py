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
    def write_to_disk(self,fname):
        f = open(fname,'w')
        self.doc.writexml(f,addindent='   ',newl='\n')
        f.close()
    def write_to_tmp(self):
        if self.fname == None:
            tf = tempfile.mkstemp()
            os.close(tf[0])
            self.fname = tf[1]
        self.write_to_disk(self.fname)
        return self.fname

    
    def disp(self):
        """Pretty prints the xml"""
        print self.doc.toprettyxml()

    def merge_File(self,fname,stanza):
        """Merges the given stanza into from fname into the current object"""
        tmp_doc = xml.dom.minidom.parse(fname)
        tmp_stanza = tmp_doc.getElementsByTagName(stanza).item(0)
        cur_stanza = self.stanza
        
    
        if self.doc.getElementsByTagName(stanza).length == 0:
            self.add_stanza(stanza)
        else:
            self.stanza = self.doc.getElementsByTagName(stanza).item(0)
            
        prams = tmp_stanza.getElementsByTagName("param")

        

        for p in prams:
            self.add_pram(p.getAttribute('key'),p.getAttribute('type'),p.getAttribute('value'))
            
        
        self.stanza = cur_stanza
    def __del__(self):
        if self.fname:
            if os.path.isfile(self.fname):
                os.remove(self.fname)


def parse_xml_to_dic(fname):
    '''This is a hacky function to convert the xml files produced by
    matlab into the dictionaries that the python code expects to be
    handed in (which will be turned back in to xml files to be handed
    in to the c++)'''
    tmp_doc = xml.dom.minidom.parse(fname)
    tmp_stanza = tmp_doc.getElementsByTagName('iden').item(0)
    prams = tmp_stanza.getElementsByTagName("param")
    iden_prams  = {}

    for p in prams:
        key = p.getAttribute('key')
        dtype = p.getAttribute('type')
        val = p.getAttribute('value')
        if dtype == 'int':
            iden_prams[key] = int(val)
        elif dtype == 'float':
            iden_prams[key] = float(val)
        else:
            raise Expecption('un handled type')
        
    return iden_prams

def _test_fun():
    '''
    <?xml version="1.0" ?>
<root>
        <iden>
                <pram key="a" type="int" value="1"/>
                <pram key="b" type="float" value="3.145"/>
                <pram key="c" type="string" value="gahh"/>
        </iden>
        <test>
                <pram key="d" type="int" value="2"/>
                <pram key="e" type="int" value="3"/>
        </test>
</root>

<?xml version="1.0" ?>
<root>
        <iden>
                <pram key="test" type="int" value="5"/>
                <pram key="a" type="int" value="1"/>
                <pram key="b" type="float" value="3.145"/>
                <pram key="c" type="string" value="gahh"/>
                <pram key="f" type="int" value="4"/>
        </iden>
</root>
'''
    test = xml_data()
    test.add_stanza("iden")
    test.add_pram('a','int',1)
    test.add_pram('b','float',3.145)
    test.add_pram('c','string','gahh')

    test.add_stanza("test")
    test.add_pram('d','int',2)
    test.add_pram('e','int',3)
    test.disp()
    test.add_pram('f','int',4,"iden")
    test.add_pram('g','int',5,"test")

    test.write_to_disk('test.xml')

    test2 = xml_data()
    test2.add_stanza("iden")
    test2.add_pram('test','int',5)
    test2.merge_File('test.xml','iden')

    test2.disp()


if __name__ == "__main__":
    _test_fun()


