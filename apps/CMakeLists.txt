# apps

SET(GNU_PLOT_SRC
  ${PROJECT_SOURCE_DIR}/external_src/gnu_wrapper/gnuplot_i.cpp
)
include_directories(${TRACKING_SOURCE_DIR}/external_src/gnu_wrapper)


message(status "common source is ${COMMON_SRC}")

add_executable(gofr_hdf gofr_hdf.cpp ${COMMON_SRC} ${UTIL_SRC} ${HDF_SRC} ${CORR_SRC} ${GNU_PLOT_SRC})
message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(gofr_hdf ${HDF5_LIBRARIES} )



add_executable(goftheta_hdf goftheta_hdf.cpp ${COMMON_SRC} ${UTIL_SRC} ${HDF_SRC} ${CORR_SRC} ${GNU_PLOT_SRC})
message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(goftheta_hdf ${HDF5_LIBRARIES} )
