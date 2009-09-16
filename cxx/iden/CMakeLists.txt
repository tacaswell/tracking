SET(COMMON_IDEN_SRC data_proc1.cpp image1.cpp kernel1.cpp params1.cpp)







find_package(IPP REQUIRED)
include_directories(${IPP_INCLUDE_DIRS})
set(LIBS ${LIBS} ${IPPI_LIB})
set(LIBS ${LIBS} ${IPPS_LIB})
set(LIBS ${LIBS} ${IPPV_LIB})
set(LIBS ${LIBS} ${IPPC_LIB})

include(FindFreeImage)


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


add_executable(Iden iden.cpp ${COMMON_IDEN_SRC} ${COMMON_TRACK_SRC} ${UTIL_SRC} ${HDF_SRC})
target_link_libraries(Iden ${IPP_LIBRARIES} )
target_link_libraries(Iden ${HDF_LIBRARIES} )
target_link_libraries(Iden ${Freeimage_LIBRARIES})