INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})



SET(CMAKE_SWIG_FLAGS "-c++")

set(SWIG_STATIC cstatic2D.i)
set(SWIG_TRACKING ctracking2D.i)
set(SWIG_SOURCES ${SWIG_STATIC} ${SWIG_TRACKING})
set_source_files_properties(${SWIG_SOURCES} PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${SWIG_SOURCES} PROPERTIES SWIG_FLAGS '-includeall')


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


set(SWIG_WRAPPERS static2D.py tracking2D.py)
FOREACH( F ${SWIG_WRAPPERS})
  cm_export_file(${F} "./")
ENDFOREACH(F)