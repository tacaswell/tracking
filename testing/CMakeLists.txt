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





