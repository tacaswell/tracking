SET(COMMON_IDEN_SRC
  data_proc1.cpp
  image1.cpp
  kernel1.cpp
  params1.cpp
  iden.cpp
  wrapper_i_plu.cpp)









find_package(IPP REQUIRED)
find_package(FreeImage REQUIRED)
find_package(HDF5 REQUIRED)

include_directories(${IPP_INCLUDE_DIRS})
include_directories(${FreeImage_INCLUDE_DIR})
include_directories("/home/tcaswell/other_source/gnuplot-cpp-read-only/")
#set(LIBS ${LIBS} ${IPPI_LIB})
#set(LIBS ${LIBS} ${IPPS_LIB})
#set(LIBS ${LIBS} ${IPPV_LIB})
#set(LIBS ${LIBS} ${IPPC_LIB})
#





add_library(iden SHARED tac_bpass_cntfnd.cpp ${COMMON_IDEN_SRC}
  ${MATLAB_UTIL_SRC}) 
target_link_libraries(iden ${MATLAB_LIBRARIES} ${LIBS})
set_target_properties(iden PROPERTIES 
  PREFIX "" 
  SUFFIX ".${MATLAB_MEXFILE_EXT}" )
#
#ADD_CUSTOM_COMMAND(
#  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/iden.m
#  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tac_bpass_cntfnd.m ${CMAKE_CURRENT_BINARY_DIR}/iden.m
#  #  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tac_bpass_cntfnd.m
#  )
#
#ADD_CUSTOM_TARGET(driver ALL DEPENDS iden.m)



add_executable(Iden process.cpp ${COMMON_IDEN_SRC} ${COMMON_TRACK_SRC} ${UTIL_SRC} ${HDF_SRC})
message(STATUS "Found IPP: ${IPP_LIBRARIES}")
target_link_libraries(Iden ${IPP_LIBRARIES} )
message(STATUS "Found HDF: ${HDF5_LIBRARIES}")
target_link_libraries(Iden ${HDF5_LIBRARIES} )
message(STATUS "Found FreeImage: ${FreeImage_LIBRARIES}")
target_link_libraries(Iden ${FreeImage_LIBRARIES})