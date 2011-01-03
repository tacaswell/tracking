# apps

SET(GNU_PLOT_SRC
  ${PROJECT_SOURCE_DIR}/external_src/gnu_wrapper/gnuplot_i.cpp
)
include_directories(${TRACKING_SOURCE_DIR}/external_src/gnu_wrapper)


#message(status "common source is ${COMMON_SRC}")

add_executable(gofr gofr_hdf.cpp 
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${HDF_SRC} 
  ${CORR_SRC}
  ${GNU_PLOT_SRC}
  ${COMMON_HASH_SRC} 
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
  )
#message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(gofr 
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(gofr PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")



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


add_executable(link3D link3D.cpp ${COMMON_SRC}
 ${UTIL_SRC}
 ${HDF_SRC}
 ${CORR_SRC}
 ${GNU_PLOT_SRC}
 ${COMMON_TRACK_SRC}
 ${COMMON_HASH_SRC} 
 ${HASH_TRACK_SRC}
 ${COMMON_WRAPPER_SRC} 
 ${CONFIG_SRC}
)
target_link_libraries(link3D 
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
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



add_executable(phi6 phi6.cpp ${COMMON_SRC}
 ${UTIL_SRC}
 ${HDF_SRC}
 ${CORR_SRC}
 ${GNU_PLOT_SRC}
 ${COMMON_HASH_SRC} 
 ${COMMON_WRAPPER_SRC} 
 ${CONFIG_SRC}
)
target_link_libraries(phi6
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
)
set_target_properties(phi6 PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")



add_executable(tracking tracking.cpp ${COMMON_SRC}
 ${UTIL_SRC}
 ${HDF_SRC}
 ${CORR_SRC}
 ${GNU_PLOT_SRC}
 ${COMMON_TRACK_SRC}
 ${COMMON_HASH_SRC} 
 ${HASH_TRACK_SRC}
 ${COMMON_WRAPPER_SRC} 
 ${CONFIG_SRC}
 ${SQL_CODE}
)
target_link_libraries(tracking
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  ${SQLITE3_LIBRARY}
  )
set_target_properties(tracking PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



add_executable(gofr_by_plane gofr_by_plane.cpp 
  ${COMMON_SRC} 
  ${UTIL_SRC} 
  ${HDF_SRC} 
  ${CORR_SRC}
  ${GNU_PLOT_SRC}
  ${COMMON_HASH_SRC} 
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
  )
#message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(gofr_by_plane
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(gofr_by_plane PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")


add_executable(msd msd ${COMMON_SRC}
 ${UTIL_SRC}
 ${HDF_SRC}
 ${CORR_SRC}
 ${GNU_PLOT_SRC}
 ${COMMON_TRACK_SRC}
 ${COMMON_HASH_SRC} 
 ${HASH_TRACK_SRC}
 ${COMMON_WRAPPER_SRC} 
 ${CONFIG_SRC}
)
target_link_libraries(msd
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(msd PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")


add_executable(track_stats track_stats
  ${COMMON_SRC}
  ${UTIL_SRC}
  ${HDF_SRC}
  ${CORR_SRC}
  ${GNU_PLOT_SRC}
  ${COMMON_TRACK_SRC}
  ${COMMON_HASH_SRC} 
  ${HASH_TRACK_SRC}
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
  ${COMMON_SRC_PATH}/histogram
  )
target_link_libraries(track_stats
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(track_stats PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")


add_executable(vanHove vanHove
  ${COMMON_SRC}
  ${UTIL_SRC}
  ${HDF_SRC}
  ${COMMON_HASH_SRC} 
  ${COMMON_TRACK_SRC}
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
  ${COMMON_SRC_PATH}/histogram
  ${TRK_ACCUM_SRC}
  )
target_link_libraries(vanHove
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  )
set_target_properties(vanHove PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



add_executable(msd_sweep msd_sweep ${COMMON_SRC}
 ${UTIL_SRC}
 ${HDF_SRC}
 ${CORR_SRC}
 ${GNU_PLOT_SRC}
 ${COMMON_TRACK_SRC}
 ${COMMON_HASH_SRC} 
 ${HASH_TRACK_SRC}
 ${COMMON_WRAPPER_SRC} 
 ${CONFIG_SRC}
 ${SQL_CODE}
 ${COMMON_SRC_PATH}/ta_msd
)
target_link_libraries(msd_sweep
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
  ${SQLITE3_LIBRARY}
  )
set_target_properties(msd_sweep PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")
