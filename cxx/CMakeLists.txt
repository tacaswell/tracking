SET(COMMON_TRACK_SRC gofr_free.cpp hash_box.cpp hash_case.cpp hash_shelf.cpp histogram.cpp master_box.cpp particle_base.cpp particle_track.cpp  track_box.cpp track_list.cpp track_shelf.cpp wrapper_base.cpp wrapper_i.cpp wrapper_i_file.cpp  wrapper_i_ning.cpp wrapper_i_ning_hd.cpp wrapper_o_file.cpp)

SET(MATLAB_UTIL_SRC wrapper_i_matlab.cpp wrapper_o_matlab.cpp matlab_utils.cpp)



#add_executable (tracking_cmd ${COMMON_TRACK_SRC} test.cpp)



add_library(test SHARED mex_test.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(test ${MATLAB_LIBRARIES})
set_target_properties(test PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)
