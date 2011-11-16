
add_executable(filter_factory_test filter_factory_test.cpp
  ${CONFIG_SRC}
  ${HDF_SRC}
  ${COMMON_SRC}
  )
target_link_libraries(filter_factory_test ${XERCESC_LIBRARY} ${HDF5_LIBRARIES} )
set_target_properties(filter_factory_test PROPERTIES COMPILE_DEFINITIONS "MAIN_TEST;PTYPE=0;DIM_COUNT=2")

cm_export_file("test.xml" "./")