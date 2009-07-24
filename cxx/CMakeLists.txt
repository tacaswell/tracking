SET(COMMON_TRACK_SRC gofr_free.cpp hash_box.cpp hash_case.cpp
hash_shelf.cpp histogram.cpp particle_base.cpp
particle_track.cpp track_box.cpp track_list.cpp track_shelf.cpp
wrapper_base.cpp wrapper_i.cpp wrapper_i_file.cpp wrapper_i_ning.cpp
wrapper_i_ning_hd.cpp wrapper_o_file.cpp tuple.cpp array.cpp cell.cpp
coarse_grain_array.cpp counted_vector.cpp
)

SET(HDF_SRC wrapper_i_hdf5.cpp)

SET(MATLAB_UTIL_SRC wrapper_i_matlab.cpp wrapper_o_matlab.cpp matlab_utils.cpp
  generic_wrapper_matlab.cpp generic_parameters_matlab.cpp cell_matlab.cpp )


message( ${PROJECT_BINARY_DIR})
add_executable (tracking_cmd ${COMMON_TRACK_SRC} test.cpp ${HDF_SRC})
target_link_libraries(tracking_cmd ${HDF5_LIBRARIES})
set_target_properties(tracking_cmd PROPERTIES compile_flags "-D_BSD_SOURCE -D_POSIX_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE" 
  link_flags "-D_BSD_SOURCE -D_POSIX_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE")


 
add_executable(fast_gofr fast_gofr.cpp ${COMMON_TRACK_SRC}
)





add_library(test SHARED mex_test.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(test ${MATLAB_LIBRARIES})
set_target_properties(test PROPERTIES link_flags "-Wl,--version-script=/usr/local/matlab/extern/lib/glnxa64/mexFunction.map")
set_target_properties(test PROPERTIES compile_flags  "-DMATLAB_MEX_FILE")
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



add_library(quick_test SHARED quick_mex_test.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(quick_test ${MATLAB_LIBRARIES})
set_target_properties(quick_test PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)

SET(APPS basic_static basic_dynamic nn_vectors nnn_vectors corr_test)

foreach(app ${APPS})
  add_library(${app} SHARED apps/${app}.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
  target_link_libraries(${app} ${MATLAB_LIBRARIES})
  set_target_properties(${app} PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
    )
endforeach(app)

# add_library(basic_static SHARED apps/basic_static.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
# target_link_libraries(basic_static ${MATLAB_LIBRARIES})
# set_target_properties(basic_static PROPERTIES
#     PREFIX ""
#     SUFFIX  ".${MATLAB_MEXFILE_EXT}"
# )

# add_library(basic_dynamic SHARED apps/basic_dynamic.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
# target_link_libraries(basic_dynamic ${MATLAB_LIBRARIES})
# set_target_properties(basic_dynamic PROPERTIES
#     PREFIX ""
#     SUFFIX  ".${MATLAB_MEXFILE_EXT}"
# )


# add_library(nn_vectors SHARED apps/nn_vectors.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
# target_link_libraries(nn_vectors ${MATLAB_LIBRARIES})
# set_target_properties(nn_vectors PROPERTIES
#     PREFIX ""
#     SUFFIX  ".${MATLAB_MEXFILE_EXT}"
# )



# add_library(corr_test SHARED apps/corr_test.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
# target_link_libraries(corr_test ${MATLAB_LIBRARIES})
# set_target_properties(corr_test PROPERTIES
#     PREFIX ""
#     SUFFIX  ".${MATLAB_MEXFILE_EXT}"
# )
