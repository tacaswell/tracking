# apps

SET(GNU_PLOT_SRC
  ${PROJECT_SOURCE_DIR}/external_src/gnu_wrapper/gnuplot_i.cpp
)
include_directories(${TRACKING_SOURCE_DIR}/external_src/gnu_wrapper)


message(status "common source is ${COMMON_SRC}")
message(status "util source is ${UTIL_SRC}")
message(status "common track source is ${COMMON_TRACK_SRC}")

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





