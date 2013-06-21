#Copyright 2011-2012 Thomas A Caswell
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
from __future__ import division

import os.path
import datetime
import xml.dom.minidom
import PIL.Image
import numpy as np
import re


# needed for plotting
class Image_wrapper(object):
    '''Base class for wrapping image sequences'''

    def __getitem__(self, key):
        if type(key) == slice:
            return map(self.get_frame, range(self.image_count)[key])

        return self.get_frame(key)

    def get_frame(self, j):
        raise NotImplementedError

    def __str__(self):
        try:
            fn = self.format_name
        except:
            fn = ''
        return fn


class Stack_wrapper(Image_wrapper):
    def __init__(self, fname):
        '''fname is the full path '''
        self.format_name = fname
        self.im = PIL.Image.open(fname)

        self.im.seek(0)
        # get image dimensions from the meta data the order is flipped
        # due to row major v col major ordering in tiffs and numpy
        self.im_sz = [self.im.tag[0x101][0],
                      self.im.tag[0x100][0]]
        self.cur = self.im.tell()

        j = 0
        while True:
            try:
                self.im.seek(j)
                j += 1
            except EOFError:
                self.im.seek(0)
                break

        self._len = j

    def __len__(self):
        return self._len

    def get_frame(self, j):
        '''Extracts the jth frame from the image sequence.
        if the frame does not exist return None'''
        try:
            self.im.seek(j)
        except EOFError:
            return None

        self.cur = self.im.tell()
        return np.reshape(self.im.getdata(), self.im_sz).astype('uint16')

    def __iter__(self):
        self.im.seek(0)
        self.old = self.cur
        self.cur = self.im.tell()
        return self

    def next(self):
        try:
            self.im.seek(self.cur)
            self.cur = self.im.tell()+1
        except EOFError:
            self.im.seek(self.old)
            self.cur = self.im.tell()
            raise StopIteration
        return np.reshape(self.im.getdata(), self.im_sz)

    def get_meta(self, j):
        cur = self.im.tell()
        if cur != j:
            self.im.seek(j)
            xml_str = self.im.tag[270]
            self.im.seek(cur)
        else:
            xml_str = self.im.tag[270]

        return _parse_xml_string(xml_str)


class Series_wrapper(Image_wrapper):
    prog = re.compile(r'(.*?)([0-9]+)\.([a-zA-Z]+)')

    @classmethod
    def create_wrapper(cls, first_fname):
        '''
        :param first_fname: the full path to the first image in the series

        Uses regular expressions to guess the basename, offset, padding and extension. '''

        res = cls.prog.search(first_fname)
        if res is None:
            return None
        basename, num, ext = res.groups()
        padding = len(num)
        return cls(basename, ext, int(num), padding)

    def __init__(self, base_name, ext, img_num_off_set, padding):
        '''
        :param base_name: the  the full path up to the numbering
        :param ext: file extension
        :param padding: the number of digits the numbers are padded to
        :param img_num_off_set: the number of the first frame (to map to the first frame = 0)

        '''
        self.format_name = base_name
        self.base_name = base_name + '%0' + str(padding) + 'd.' + ext

        self._im_num_offset = img_num_off_set
        j = 0
        while os.path.isfile(self.base_name % (j+self._im_num_offset)):
            j += 1
        self._len = j

    def __len__(self):
        return self._len

    def get_frame(self, j):
        '''Extracts the jth (off set) frame from the image sequence.
        if the frame does not exist return None'''
        try:
            im = PIL.Image.open(self.base_name % (j+self._im_num_offset))
        except IOError:
            print "didn't find the file"
            print self.base_name % j
            return None
        img_sz = im.size[::-1]
        return np.reshape(im.getdata(), img_sz).astype('uint16')


def _parse_xml_string(xml_str):

    def _write(md_dict, name, val):
        if name == "acquisition-time-local" or name == "modification-time-local":
            tmp = int(val[18:])
            val = val[:18] + "%(#)03d" % {"#": tmp}
            val = datetime.datetime.strptime(val, '%Y%m%d %H:%M:%S.%f')
        md_dict[name] = val

    def _parse_attr(file_obj, dom_obj):
        if dom_obj.getAttribute("id") == "Description":
            _parse_des(file_obj, dom_obj)
        elif dom_obj.getAttribute("type") == "int":
            _write(file_obj, dom_obj.getAttribute("id"), int(dom_obj.getAttribute("value")))
        elif dom_obj.getAttribute("type") == "float":
            _write(file_obj, dom_obj.getAttribute("id"), float(dom_obj.getAttribute("value")))
        else:
            _write(file_obj, dom_obj.getAttribute("id"), dom_obj.getAttribute("value").encode('ascii'))

    def _parse_des(file_obj, des_obj):
        des_string = des_obj.getAttribute("value")
        des_split = des_string.split("&#13;&#10;")

        for x in des_split:
            tmp_split = x.split(":")
            if len(tmp_split) == 2:
                _write(file_obj, tmp_split[0], tmp_split[1].encode('ascii'))

    dom = xml.dom.minidom.parseString(xml_str)

    props = dom.getElementsByTagName("prop")
    f = dict()
    for p in props:
        _parse_attr(f, p)

    return f
