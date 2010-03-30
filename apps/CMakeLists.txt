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


add_executable(sofq sofq.cpp 
  ${COMMON_SRC}
  ${HDF_SRC}
  ${CORR_SRC}
  ${GNU_PLOT_SRC}
  ${COMMON_HASH_SRC} 
  ${COMMON_WRAPPER_SRC} 
  ${CONFIG_SRC}
)
target_link_libraries(sofq
  ${HDF5_LIBRARIES} 
  ${XERCESC_LIBRARY} 
)
set_target_properties(sofq PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")




# add_executable(tracking tracking.cpp ${COMMON_SRC}
#  ${UTIL_SRC}
#  ${HDF_SRC}
#  ${CORR_SRC}
#  ${GNU_PLOT_SRC}
#  ${COMMON_TRACK_SRC}
#  ${COMMON_HASH_SRC} 
#  ${HASH_TRACK_SRC}
#  ${COMMON_WRAPPER_SRC} 
#  ${CONFIG_SRC}
# )
# target_link_libraries(tracking
#   ${HDF5_LIBRARIES} 
#   ${XERCESC_LIBRARY} 
#   )
# set_target_properties(tracking PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



# add_executable(phi6 phi6.cpp ${COMMON_SRC}
#  ${UTIL_SRC}
#  ${HDF_SRC}
#  ${CORR_SRC}
#  ${GNU_PLOT_SRC}
#  ${COMMON_HASH_SRC} 
#  ${COMMON_WRAPPER_SRC} 
#  ${CONFIG_SRC}
# )
# target_link_libraries(phi6
#   ${HDF5_LIBRARIES} 
#   ${XERCESC_LIBRARY} 
# )
# set_target_properties(phi6 PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
