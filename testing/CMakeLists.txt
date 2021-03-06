# testing


add_executable(track_test track_test.cpp 
  ${COMMON_SRC} 
  ${UTIL_SRC} 
   
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
   
  ${CONFIG_SRC}
  )
set_target_properties(hdf_add PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(hdf_add ${HDF5_LIBRARIES} ${XERCESC_LIBRARY})


add_executable(fake_hdf make_fake_hdf.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  ${HDF_SRC}
   
  ${CONFIG_SRC}
  )
set_target_properties(fake_hdf PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(fake_hdf ${HDF5_LIBRARIES} ${XERCESC_LIBRARY} )



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
   
  ${CONFIG_SRC}
  ${COMMON_TRACK_SRC}
  )
set_target_properties(read_write_add_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2;TESTING")
target_link_libraries(read_write_add_test ${HDF5_LIBRARIES}  ${XERCESC_LIBRARY} )

add_executable(template_test template_testing.cpp)
set_target_properties(template_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=3;TESTING")

add_executable(sofq_ND_test sofq_ND_test.cpp
  ${COMMON_SRC}
  ${CORR_SRC}
  
  ${COMMON_HASH_SRC}
  ${ACCUM_SCR}
  )
set_target_properties(sofq_ND_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")


add_executable(sofq_test sofq_test.cpp
  ${COMMON_SRC}
  ${CORR_SRC}
  
  ${COMMON_HASH_SRC}
  ${ACCUM_SCR}
  )
set_target_properties(sofq_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")

add_executable(sql_test sql_test.cpp 
  ${CONFIG_SRC}
  ${SQL_CODE}
)
set_target_properties(sql_test PROPERTIES COMPILE_DEFINITIONS  "TESTING")
target_link_libraries(sql_test ${Boost_LIBRARIES} ${dl_LIBRARY} ${XERCESC_LIBRARY} ${SQLITE3_LIBRARY} )



add_executable(corr_test corr_test.cpp
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${COMMON_HASH_SRC}
  ${CORR_SRC}
  )
set_target_properties(corr_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")

add_executable(md_store_hdf md_store_hdf
  ${COMMON_SRC_PATH}/md_store
  ${COMMON_SRC_PATH}/attr_list_hdf
  ${COMMON_SRC_PATH}/enum_utils
  ${COMMON_SRC_PATH}/md_store_attr_list
  )
set_target_properties(md_store_hdf PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(md_store_hdf ${HDF5_LIBRARIES} )



add_executable(rtt_hc rtt_hc.cpp 
  ${COMMON_SRC}
  ${UTIL_SRC}
  ${HDF_SRC}
  
  ${COMMON_TRACK_SRC}
  ${COMMON_HASH_SRC} 
  ${HASH_TRACK_SRC}
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
  )
target_link_libraries(rtt_hc
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(rtt_hc PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



add_executable(rtt_ts rtt_ts.cpp 
  ${COMMON_SRC}
  ${UTIL_SRC}
  ${HDF_SRC}
  
  ${COMMON_TRACK_SRC}
  ${COMMON_HASH_SRC} 
  ${HASH_TRACK_SRC}
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
  )
target_link_libraries(rtt_ts
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(rtt_ts PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



add_subdirectory(filt_fac_test)




add_executable(wrapper_i_generic_test wrapper_i_generic_test.cpp
  ${COMMON_WRAPPER_SRC}
  ${UTIL_SRC} 
  )
set_target_properties(wrapper_i_generic_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")


add_executable(jb_binary_test jb_binary_test.cpp
  ${COMMON_WRAPPER_SRC}
  ${COMMON_SRC_PATH}/wrapper_i_jb_binary.cpp
  ${UTIL_SRC} 
  )
set_target_properties(jb_binary_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")


add_executable(generic_wrapper_test generic_wrapper_test.cpp
  ${COMMON_SRC_PATH}/generic_wrapper_hdf.cpp 
  ${COMMON_SRC_PATH}/attr_list_hdf.cpp
  ${COMMON_SRC_PATH}/md_store_attr_list
  ${COMMON_SRC_PATH}/md_store.cpp
  ${COMMON_SRC_PATH}/enum_utils.cpp
  )
set_target_properties(generic_wrapper_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")
target_link_libraries(generic_wrapper_test ${HDF5_LIBRARIES} )




add_executable(md_store_test md_store_test
  ${COMMON_SRC_PATH}/md_store
  ${COMMON_SRC_PATH}/enum_utils
  )
set_target_properties(md_store_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2;TESTING")



add_executable(accum_case_test accum_case_test
  ${COMMON_SRC_PATH}/accum_chi4_self
  ${COMMON_SRC_PATH}/md_store
  ${COMMON_SRC_PATH}/enum_utils
  ${COMMON_SRC_PATH}/particle_track.cpp
  ${COMMON_SRC_PATH}/particle_base.cpp
  )
set_target_properties(accum_case_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2;TESTING")


