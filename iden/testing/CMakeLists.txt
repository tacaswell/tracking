add_executable(fitest fi_test.cpp
  
  )
set_target_properties(fitest PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
target_link_libraries(fitest 
  ${FreeImage_LIBRARIES}
  ${XERCESC_LIBRARY}
  )


