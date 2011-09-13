# - Find SQLITE3
# The following setings are defined
# SQLITE3_ROOT_DIR, the root of the include and lib directory
# SQLITE3_INCLUDE_DIR, the full path of the include dir (ADVANCED)
# SQLITE3_LIBRARIES, the name of the sqlite library (ADVANCED)
#####
# Find SQLITE3

# 2010-11-19 TAC
# a modified version of FindXerciesC.cmake
# taken from http://cartwheel.idyll.org/browser/trunk/FindXercesC.cmake

set(SQLITE3_FIND_QUIETLY FALSE)

# Look for a root installation
FIND_PATH(SQLITE3_ROOT_DIR include/sqlite3.h
  HINTS ENV  SQLITE3_ROOT
  /usr
  
  DOC "The root of an installed sqlite installation"
)

# try to find the header
FIND_PATH(SQLITE3_INCLUDE_DIR sqlite3.h
  HINTS ENV  SQLITE3_ROOT
  ${SQLITE3_ROOT_DIR}/include
  /usr/include 
  /usr/local/include
)

# Find the library
FIND_LIBRARY(SQLITE3_LIBRARY
   NAMES sqlite3
   HINTS ENV  SQLITE3_ROOT
   PATHS
     ${SQLITE3_ROOT_DIR}/lib
     /usr/lib 
     /usr/local/lib
   DOC "The name of the sqlite library"
)


IF (SQLITE3_ROOT_DIR)
  IF (SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
    SET (SQLITE3_FOUND TRUE)
    SET (SQLITE3_LIBRARIES "${SQLITE3_LIBRARY}")
    # FIXME: There should be a better way of handling this?
    # FIXME: How can we test to see if the lib dir isn't 
    # FIXME: one of the default dirs?
    LINK_DIRECTORIES(${SQLITE3_ROOT_DIR}/lib)
  ENDIF (SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
ENDIF (SQLITE3_ROOT_DIR)

IF (SQLITE3_FOUND)
  IF (NOT SQLITE3_FIND_QUIETLY)
    MESSAGE (STATUS "Found sqlite3: ${SQLITE3_LIBRARY}")
  ENDIF (NOT SQLITE3_FIND_QUIETLY)
ELSE (SQLITE3_FOUND)
  IF (SQLITE3_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find sqlite")
  ENDIF (SQLITE3_FIND_REQUIRED)
ENDIF (SQLITE3_FOUND)

MARK_AS_ADVANCED(
  SQLITE3_INCLUDE_DIR
  SQLITE3_LIBRARY
)
