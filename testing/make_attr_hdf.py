import h5py
import numpy


f = h5py.File("attr_test.h5",'w')
f.attrs.create('test_attr',1,None,'int32')
f.attrs.create('test_attr2',[1,2],None,'int32')
f.attrs.create('test_attrf',123.456,None,'float32')
f.attrs.create('test_str',"test")
f.attrs.create('pair_test',[1.2,3.4],None,'float32')
f.attrs.create('triple_test',[1.2,3.4,5.6],None,'float32')
f.close()
