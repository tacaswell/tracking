# testing

SET(GNU_PLOT_SRC
  ${PROJECT_SOURCE_DIR}/external_src/gnu_wrapper/gnuplot_i.cpp
)
include_directories(${TRACKING_SOURCE_DIR}/external_src/gnu_wrapper)


add_executable(track_test track_test.cpp 
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${GNU_PLOT_SRC} 
  ${COMMON_TRACK_SRC}
  )
set_target_properties(track_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2;TESTING")


add_executable(hash_test2 hash_test.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  )
set_target_properties(hash_test2 PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")


add_executable(hash_test3 hash_test.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  )
set_target_properties(hash_test3 PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=3;TESTING")


add_executable(xml_test read_config_test.cpp
  ${CONFIG_SRC}
  )
target_link_libraries(xml_test ${XERCESC_LIBRARY} )
set_target_properties(xml_test PROPERTIES COMPILE_DEFINITIONS "MAIN_TEST")


add_executable(hdf_add add_to_hdf_test.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  ${HDF_SRC}
  ${GNU_PLOT_SRC} 
  )
set_target_properties(hdf_add PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(hdf_add ${HDF5_LIBRARIES} )


add_executable(fake_hdf make_fake_hdf.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  ${HDF_SRC}
  ${GNU_PLOT_SRC} 
  )
set_target_properties(fake_hdf PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(fake_hdf ${HDF5_LIBRARIES} )



add_executable(hdf_attr attr_test.cpp
  ${COMMON_SRC} 
  ${COMMON_SRC_PATH}/attr_list_hdf.cpp
  )
set_target_properties(hdf_attr PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(hdf_attr ${HDF5_LIBRARIES} )



add_executable(read_write_add_test read_write_add_test.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  ${HDF_SRC}
  ${GNU_PLOT_SRC} 
  )
set_target_properties(read_write_add_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(read_write_add_test ${HDF5_LIBRARIES} )
