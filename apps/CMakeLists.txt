# apps

SET(GNU_PLOT_SRC
  ${PROJECT_SOURCE_DIR}/external_src/gnu_wrapper/gnuplot_i.cpp
)
include_directories(${TRACKING_SOURCE_DIR}/external_src/gnu_wrapper)


message(status "common source is ${COMMON_SRC}")

add_executable(gofr_hdf gofr_hdf.cpp ${COMMON_SRC} ${UTIL_SRC} ${HDF_SRC} ${CORR_SRC} ${GNU_PLOT_SRC})
message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(gofr_hdf ${HDF5_LIBRARIES} )
set_target_properties(gofr_hdf PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")


add_executable(goftheta_hdf goftheta_hdf.cpp ${COMMON_SRC} ${UTIL_SRC} ${HDF_SRC} ${CORR_SRC} ${GNU_PLOT_SRC})
message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(goftheta_hdf ${HDF5_LIBRARIES} )
set_target_properties(goftheta_hdf PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")


add_executable(link3D link3D.cpp ${COMMON_SRC} ${UTIL_SRC} ${HDF_SRC} ${CORR_SRC} ${GNU_PLOT_SRC} ${COMMON_TRACK_SRC})
target_link_libraries(link3D ${HDF5_LIBRARIES} )
set_target_properties(link3D PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")



