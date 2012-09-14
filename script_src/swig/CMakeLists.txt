INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})



SET(CMAKE_SWIG_FLAGS "-c++")

set(SWIG_STATIC cstatic2D.i)
set(SWIG_TRACKING ctracking2D.i)
set(SWIG_IDEN ciden2D.i)
set(SWIG_SOURCES ${SWIG_STATIC} ${SWIG_TRACKING} ${SWIG_IDEN})
set_source_files_properties(${SWIG_SOURCES} PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${SWIG_SOURCES} PROPERTIES SWIG_FLAGS '-includeall')



if(COMPILE_BASIC)
SWIG_ADD_MODULE(cstatic2 python
  ${SWIG_STATIC})
SWIG_LINK_LIBRARIES(cstatic2
  ${PYTHON_LIBRARIES}
  cstatic2D)



SWIG_ADD_MODULE(ctracking2 python
  ${SWIG_TRACKING})
SWIG_LINK_LIBRARIES(ctracking2
  ${PYTHON_LIBRARIES}
  ctracking2D)

endif(COMPILE_BASIC)

if(COMPILE_IDEN)
set(IDEN_INC_PATH ${PROJECT_SOURCE_DIR}/iden/include)

include_directories(${IPP_INCLUDE_DIRS})
include_directories(${FreeImage_INCLUDE_DIR})
include_directories(${IDEN_INC_PATH})

SWIG_ADD_MODULE(ciden2 python
  ${SWIG_IDEN})
SWIG_LINK_LIBRARIES(ciden2
  ${PYTHON_LIBRARIES}
  cstatic2D
  ciden2D)
endif(COMPILE_IDEN)


set(SWIG_WRAPPERS static2D.py tracking2D.py)
FOREACH( F ${SWIG_WRAPPERS})
  cm_export_file(${F} "./")
ENDFOREACH(F)