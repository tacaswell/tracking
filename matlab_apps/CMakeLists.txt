SET(COMMON_SRC
 ${PROJECT_SOURCE_DIR}/cxx/gofr_free.cpp
 ${PROJECT_SOURCE_DIR}/cxx/hash_box.cpp
 ${PROJECT_SOURCE_DIR}/cxx/hash_case.cpp
 ${PROJECT_SOURCE_DIR}/cxx/hash_shelf.cpp
 ${PROJECT_SOURCE_DIR}/cxx/particle_base.cpp
 ${PROJECT_SOURCE_DIR}/cxx/function_cascades.cpp
 ${PROJECT_SOURCE_DIR}/cxx/tuple.cpp
 ${PROJECT_SOURCE_DIR}/cxx/data_map.cpp
 ${PROJECT_SOURCE_DIR}/cxx/enum_utils.cpp
 ${PROJECT_SOURCE_DIR}/cxx/filter_basic.cpp
 )

SET(COMMON_TRACK_SRC
  ${PROJECT_SOURCE_DIR}/cxx/particle_track.cpp
  ${PROJECT_SOURCE_DIR}/cxx/track_box.cpp
  ${PROJECT_SOURCE_DIR}/cxx/track_list.cpp
  ${PROJECT_SOURCE_DIR}/cxx/track_shelf.cpp 
  ${PROJECT_SOURCE_DIR}/cxx/hash_track_funs.cpp 
  )

SET(UTIL_SRC
  ${PROJECT_SOURCE_DIR}/cxx/array.cpp
  ${PROJECT_SOURCE_DIR}/cxx/cell.cpp
  ${PROJECT_SOURCE_DIR}/cxx/coarse_grain_array.cpp
  ${PROJECT_SOURCE_DIR}/cxx/counted_vector.cpp
  ${PROJECT_SOURCE_DIR}/cxx/histogram2d.cpp
  ${PROJECT_SOURCE_DIR}/cxx/histogram.cpp
)

SET(CORR_SRC
  ${PROJECT_SOURCE_DIR}/cxx/corr_gofr.cpp
)
SET(MATLAB_WRAPPER_SRC

)

SET(OTHER_WRAPPER_SRC
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_i_file.cpp
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_i_ning.cpp
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_i_ning_hd.cpp
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_o_file.cpp
  )

SET(HDF_SRC
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_i_hdf.cpp
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_o_hdf.cpp
  ${PROJECT_SOURCE_DIR}/cxx/generic_wrapper_hdf.cpp
  )

SET(MATLAB_UTIL_SRC
  ${PROJECT_SOURCE_DIR}/cxx/matlab_utils.cpp
  ${PROJECT_SOURCE_DIR}/cxx/generic_wrapper_matlab.cpp
  ${PROJECT_SOURCE_DIR}/cxx/generic_parameters_matlab.cpp
  ${PROJECT_SOURCE_DIR}/cxx/cell_matlab.cpp 
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_i_matlab.cpp
  ${PROJECT_SOURCE_DIR}/cxx/wrapper_o_matlab.cpp
  )

SET(GNU_PLOT_SRC
  /home/tcaswell/other_source/gnuplot-cpp-read-only/gun_plot_i.cpp
)


message( ${PROJECT_BINARY_DIR})
add_executable (tracking_cmd ${COMMON_TRACK_SRC} test.cpp ${HDF_SRC})
target_link_libraries(tracking_cmd ${HDF5_LIBRARIES})
set_target_properties(tracking_cmd PROPERTIES compile_flags "-D_BSD_SOURCE -D_POSIX_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE" 
  link_flags "-D_BSD_SOURCE -D_POSIX_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE")


 
add_library(test SHARED mex_test.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
target_link_libraries(test ${MATLAB_LIBRARIES})
set_target_properties(test PROPERTIES link_flags "-Wl,--version-script=/usr/local/matlab/extern/lib/glnxa64/mexFunction.map")
set_target_properties(test PROPERTIES compile_flags  "-DMATLAB_MEX_FILE")
set_target_properties(test PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)





add_library(test3 SHARED mex_test3.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC}
  ${UTIL_SRC} ${MATLAB_WRAPPER_SRC} wrapper_o_hdf.cpp)
target_link_libraries(test3 ${MATLAB_LIBRARIES})
target_link_libraries(test3 ${HDF5_LIBRARIES})
set_target_properties(test3 PROPERTIES link_flags "-Wl,--version-script=/usr/local/matlab/extern/lib/glnxa64/mexFunction.map")
set_target_properties(test3 PROPERTIES compile_flags  "-DMATLAB_MEX_FILE")
set_target_properties(test3 PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
)
message( ${HDF_INCLUDE_DIR})

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

SET(APPS basic_static basic_dynamic nn_vectors nnn_vectors corr_test phi6)

foreach(app ${APPS})
  add_library(${app} SHARED apps/${app}.cpp ${COMMON_TRACK_SRC} ${MATLAB_UTIL_SRC})
  target_link_libraries(${app} ${MATLAB_LIBRARIES})
  set_target_properties(${app} PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
    )
endforeach(app)

include_directories(/home/tcaswell/other_source/gnuplot-cpp-read-only)

add_subdirectory(iden)
add_subdirectory(apps)