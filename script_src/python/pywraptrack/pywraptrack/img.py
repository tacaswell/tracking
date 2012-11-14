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

import PIL.Image
import numpy as np


# needed for plotting
class Image_wrapper(object):
    '''Base class for wrapping image sequences'''
        
    def __getitem__(self, key):
        if type(key) == slice:
            return map(self.get_frame, range(self.image_count)[key])

        return self.get_frame(key)

    def get_frame(self,j):
        raise NotImplementedError
    
class Stack_wrapper(Image_wrapper):
    def __init__(self,fname):
        '''fname is the full path '''
        self.im  = PIL.Image.open(fname)

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
                j +=1
            except EOFError:
                self.im.seek(0)
                break

        self._len = j

    def __len__(self):
        return self._len
    
    def get_frame(self,j):
        '''Extracts the jth frame from the image sequence.
        if the frame does not exist return None'''
        try:
            self.im.seek(j)
        except EOFError:
            return None
        
        self.cur = self.im.tell()
        return np.reshape(self.im.getdata(),self.im_sz).astype('uint16')
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
        return np.reshape(self.im.getdata(),self.im_sz)

    
    
    def get_meta(self,j):
        cur = self.im.tell()
        if cur != j:
            self.im.seek(j)
            xml_str = im_wrap.im.tag[270]
            self.im.seek(cur)
        else: 
            xml_str = im_wrap.im.tag[270]
        return parse_xml_string(xml_str)

       
class Series_wrapper(Image_wrapper):
    def __init__(self,base_name,ext,padding = None):
        '''base name includes the full path up to the numbering
        assumes that the numbering is left padded with 0 to padding places
        the extension does not include the .'''
        if padding is None:
            im_guess = len(os.listdir(os.path.dirname(base_name)))
            padding = int(np.log10(im_guess))+1
            
        self.base_name = base_name + '%0' + str(padding) + 'd.' + ext
        # added extra +1 to cope with mm starting naming at 1, not the god-given 0
        self._im_num_offset = 1
        j = 0
        while os.path.isfile(self.base_name%(j+self._im_num_offset)):
            j +=1
        self._len = j


    def __len__(self):
        return self._len
    def get_frame(self,j):
        '''Extracts the jth frame from the image sequence.
        if the frame does not exist return None'''
        try:
            im = PIL.Image.open(self.base_name%(j+self._im_num_offset))
        except IOError:
            print "didn't find the file"
            print self.base_name%j
            return None
        img_sz = im.size[::-1]
        return np.reshape(im.getdata(),img_sz).astype('uint16')
