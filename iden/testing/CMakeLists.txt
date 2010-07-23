add_executable(fitest fi_test.cpp
  ${MD_PARSE_SRC}
  )
set_target_properties(fitest PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
target_link_libraries(fitest 
  ${FreeImage_LIBRARIES}
  ${XERCESC_LIBRARY}
  )


add_executable(mm_md_test mm_md_test
  ${MD_PARSE_SRC}
  )
set_target_properties(mm_md_test PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
target_link_libraries(mm_md_test 
  ${FreeImage_LIBRARIES}
  ${XERCESC_LIBRARY}
  )


