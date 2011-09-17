# app
SET(GNU_PLOT_SRC
  ${PROJECT_SOURCE_DIR}/external_src/gnu_wrapper/gnuplot_i.cpp
)
include_directories(${TRACKING_SOURCE_DIR}/external_src/gnu_wrapper)


#message(status "common source is ${COMMON_SRC}")

add_executable(gofr gofr_hdf
  )
#message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(gofr 
  cstatic2D
  )
set_target_properties(gofr PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")




add_executable(link3D link3D
  )
target_link_libraries(link3D 
  ctracking2D
  )
set_target_properties(link3D PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")


add_executable(gofr3D gofr3D_hdf.cpp ${COMMON_SRC}
 ${UTIL_SRC}
 ${HDF_SRC}
 ${CORR_SRC}
 ${GNU_PLOT_SRC}
 ${COMMON_HASH_SRC} 
 ${COMMON_WRAPPER_SRC} 
 ${CONFIG_SRC}
)
target_link_libraries(gofr3D 
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
)
set_target_properties(gofr3D PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=3")




add_executable(phi6 phi6
  )
target_link_libraries(phi6
  cstatic2D
)
set_target_properties(phi6 PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")



add_executable(tracking tracking.cpp 
  )
target_link_libraries(tracking
  ctracking2D
  )
set_target_properties(tracking PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



add_executable(gofr_by_plane gofr_by_plane
  )
#message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(gofr_by_plane
  cstatic2D
  )
set_target_properties(gofr_by_plane PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")




add_executable(vanHove vanHove
  )
target_link_libraries(vanHove
  ctracking2D
  )
set_target_properties(vanHove PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



add_executable(msd_sweep msd_sweep 
  ${GNU_PLOT_SRC}
  )
target_link_libraries(msd_sweep
  ctracking2D
  )
set_target_properties(msd_sweep PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")


add_executable(vanHove_sweep vanHove_sweep 
)
target_link_libraries(vanHove_sweep
  ctracking2D
  )
set_target_properties(vanHove_sweep PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



# add_executable(goftheta_hdf goftheta_hdf.cpp ${COMMON_SRC}
#  ${UTIL_SRC}
#  ${HDF_SRC}
#  ${CORR_SRC}
#  ${GNU_PLOT_SRC}
#  ${COMMON_HASH_SRC} 
#  ${COMMON_WRAPPER_SRC} 
# )
# #message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
# target_link_libraries(goftheta_hdf ${HDF5_LIBRARIES} )
# set_target_properties(goftheta_hdf PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")


# add_executable(track_stats track_stats
#   )
# target_link_libraries(track_stats
#   ctracking2D
#   )
# set_target_properties(track_stats PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")


# add_executable(sofq sofq.cpp 
#   ${COMMON_SRC}
#   ${HDF_SRC}
#   ${CORR_SRC}
#   ${GNU_PLOT_SRC}
#   ${COMMON_HASH_SRC} 
#   ${COMMON_WRAPPER_SRC} 
#   ${CONFIG_SRC}
#   ${ACCUM_SCR}
# )
# target_link_libraries(sofq
#   ${HDF5_LIBRARIES} 
#   ${XERCESC_LIBRARY} 
# )
# set_target_properties(sofq PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
