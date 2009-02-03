objects = wrapper_i_file.o  hash_box.o hash_shelf.o		\
	 gofr_free.o wrapper_o_file.o particle_track.o track_box.o	\
	 hash_case.o track_list.o track_shelf.o particle_base.o		\
	 wrapper_i_ning.o hash_shelf3d.o wrapper_i_ning_hd.o		\
	 wrapper_base.o wrapper_i.o histogram.o 

c_objects = test.o


mex_code= mex_test.cpp
mex_objects = wrapper_i_matlab.o wrapper_o_matlab.o matlab_utils.o

INCLUDE    = 
MINCLUDE   = -I$(MATLABHOME)/extern/include	

MEXSUFFIX  = mexa64
MATLABHOME = /usr/local/matlab
MEX        = mex
CXX        = g++
CXXFLAGS = -O3 -fno-inline -Wall -ansi -D_GNU_SOURCE -fPIC	\
-fno-omit-frame-pointer -pthread   $(INCLUDE) 
#-DMX_COMPAT_32
LIBS       = -lm

MEXFLAGS   = -cxx CC='$(CXX)' CXX='$(CXX)' LD='$(CXX)'
CXXLIBS = -Wl,-rpath-link,/usr/local/matlab/bin/glnxa64	\
-L/usr/local/matlab/bin/glnxa64 -lmx -lmex -lmat -lm

MCXX = 	$(CXX) $(CXXFLAGS) $(MINCLUDE) -c -o $@ $<


#testing execuatables
m_prog: $(objects) $(mex_objects) $(mex_code)
	  $(MEX) -o  $(MEXFLAGS)  $(LIBS) -output testing.$(MEXSUFFIX) $^

prog.out: $(objects) $(c_objects)
	$(CXX) $(CXXFLAGS) -o prog $(objects) $(c_objects)


track_2d: $(objects) $(mex_objects) track_2d.cpp
	  $(MEX) -o  $(MEXFLAGS)  $(LIBS) -output track_2D.$(MEXSUFFIX) $^


test_e: test2.o histogram.o
	$(CXX) $(CXXFLAGS) -o test_prog $^





#random computation
gofr_free.o: hash_box.h hash_shelf.h

#hash objects
hash_box.o: hash_box.h particle.h
hash_case.o :hash_shelf.h particle.h master_box_t.h wrapper.h	\
	track_list.h track_shelf.h hash_case.h
hash_shelf.o: hash_shelf.h hash_box.h particle.h master_box.h \
	master_box_t.h
hash_shelf3d.o: hash_shelf.h hash_box.h particle.h master_box.h \
	master_box_t.h hash_shelf3d.h


#wrappers
wrapper_base.o: wrapper.h
wrapper_i.o: wrapper_i.h params.h wrapper.h

wrapper_i_file.o: wrapper_i_file.h params_file.h wrapper.h wrapper_i.h params.h
wrapper_i_ning.o: wrapper_i.h params.h wrapper.h wrapper_i_ning.h params_ning.h
wrapper_i_ning_hd.o: wrapper_i.h params.h wrapper.h wrapper_i_ning.h	\
	 wrapper_i_ning_hd.h params_ning.h params_ning_hd.h

wrapper_o_file.o: wrapper_o.h params.h wrapper.h


#matlab objects
wrapper_i_matlab.o: wrapper_i_matlab.cpp params_matlab.h wrapper_i_matlab.h wrapper_i.h wrapper.h params.h
	$(MCXX)
wrapper_o_matlab.o: wrapper_o_matlab.cpp params_matlab.h wrapper_o_matlab.h wrapper_o.h wrapper.h params.h
	$(MCXX)
matlab_utils.o: matlab_utils.cpp matlab_utils.h
	$(MCXX)



#particle objects
particle_base.o: particle.h
particle_track.o: particle.h wrapper.h wrapper_i.h wrapper_o.h track_box.h

#track_objects
track_box.o: track_box.h particle.h
track_list.o: particle.h track_list.h 
track_shelf.o : particle.h track_box.h track_shelf.h histogram.h exception.h

#helper objects
histogram.o: histogram.h
exception.o: exception.h

#testing objects
test.o: wrapper_i.h particle.h master_box.h hash_box.h hash_shelf.h	\
	params.h wrapper_o.h wrapper.h track_box.h hash_case.h		\
	track_list.h track_shelf.h hash_shelf3d.h hash_case3d.h		\
	wrapper_i_ning_hd.h params_ning_hd.h histogram.h


test2.o: histogram.h

doc: *.h
	doxygen Doxyfile;	
	update_docs.sh;

.PHONY : clean
clean :
	-rm $(objects) $(mex_objects)
