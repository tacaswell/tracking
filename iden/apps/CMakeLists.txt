add_executable(Iden process.cpp 
  ${COMMON_IDEN_SRC} 
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${HDF_SRC} 
  ${COMMON_WRAPPER_SRC}
  ${COMMON_HASH_SRC}
  ${CONFIG_SRC}
  )
set_target_properties(Iden PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
target_link_libraries(Iden 
  ${IPP_LIBRARIES} 
  ${HDF5_LIBRARIES}
  ${FreeImage_LIBRARIES}
  ${XERCESC_LIBRARY}
  )


