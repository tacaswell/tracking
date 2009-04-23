SET(COMMON_TRACK_SRC gofr_free.cpp hash_box.cpp hash_case.cpp
hash_shelf.cpp histogram.cpp master_box.cpp particle_base.cpp
particle_track.cpp track_box.cpp track_list.cpp track_shelf.cpp
wrapper_base.cpp wrapper_i.cpp wrapper_i_file.cpp wrapper_i_ning.cpp
wrapper_i_ning_hd.cpp wrapper_o_file.cpp tuple.cpp array.cpp cell.cpp
coarse_grain_array.cpp
)

SET(MATLAB_UTIL_SRC wrapper_i_matlab.cpp wrapper_o_matlab.cpp matlab_utils.cpp
  generic_wrapper_matlab.cpp generic_parameters_matlab.cpp cell_matlab.cpp )



# add_executable (tracking_cmd ${COMMON_TRACK_SRC} test.cpp)

add_executable(fast_gofr fast_gofr.cpp ${COMMON_TRACK_SRC})

add_library(test SHARED mex_test.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(test ${MATLAB_LIBRARIES})
set_target_properties(test PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)

add_library(test2 SHARED mex_test2.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(test2 ${MATLAB_LIBRARIES})
set_target_properties(test2 PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)


add_library(basic_static SHARED basic_static.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(basic_static ${MATLAB_LIBRARIES})
set_target_properties(basic_static PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)

add_library(basic_dynamic SHARED basic_dynamic.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(basic_dynamic ${MATLAB_LIBRARIES})
set_target_properties(basic_dynamic PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)
