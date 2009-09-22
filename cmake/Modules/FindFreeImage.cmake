# - Try to find FreeImage
# Once done, this will define
#
#  FreeImage_FOUND - system has FreeImage
#  FreeImage_INCLUDE_DIRS - the FreeImage include directories
#  FreeImage_LIBRARIES - link these to use FreeImage

include(LibFindMacros)

# Dependencies


# Use pkg-config to get hints about paths
#libfind_pkg_check_modules(FreeImage_PKGCONF)

# Include dir
find_path(FreeImage_INCLUDE_DIR
  NAMES FreeImage.h
  HINTS  /home/tcaswell/other_source/FreeImage/Dist
)
# finally the library itself
find_library(FreeImage_LIBRARY
  NAMES freeimageplus
  HINTS  /home/tcaswell/other_source/FreeImage/Dist
  )

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.

if(FreeImage_INCLUDE_DIR)
 message(STATUS "Found FreeImage: ${FreeImage_INCLUDE_DIR}")
 message(STATUS "Found FreeImage: ${FreeImage_LIBRARY}")
endif(FreeImage_INCLUDE_DIR)

set(FreeImage_PROCESS_INCLUDES FreeImage_INCLUDE_DIR )
set(FreeImage_PROCESS_LIBS FreeImage_LIBRARY )
libfind_process(FreeImage)


 message(STATUS "Found FreeImage: ${FreeImage_LIBRARIES}")