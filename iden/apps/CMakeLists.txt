add_executable(Iden process.cpp 
  )
set_target_properties(Iden PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
target_link_libraries(Iden 
  cstatic2D
  ciden2D

  )


add_executable(Iden_avg process_avg 
  )
set_target_properties(Iden_avg PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
target_link_libraries(Iden_avg 
  cstatic2D
  ciden2D
  )


