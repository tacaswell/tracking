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




# Include dir
find_path(IPP_PROCESS_INCLUDE
  NAMES ipp.h
  HINTS $ENV{IPP_ROOT}/include
)

SET(IPP_PROCESS_INCLUDES  IPP_PROCESS_INCLUDE)

# Finally the library itself
find_library(IPPI_LIB 
  NAMES ippi    
  NAMES ippiem64t
  HINTS $ENV{IPP_ROOT}/sharedlib
  HINTS $ENV{IPP_ROOT}/lib/intel64
)		     
find_library(IPPS_LIB
  NAMES ipps  
  NAMES ippsem64t  
  HINTS $ENV{IPP_ROOT}/sharedlib
  HINTS $ENV{IPP_ROOT}/lib/intel64
)		     
find_library(IPPV_LIB
  NAMES ippvm
  NAMES ippvmem64t
  HINTS $ENV{IPP_ROOT}/sharedlib
  HINTS $ENV{IPP_ROOT}/lib/intel64
)
find_library(IPPC_LIB
  NAMES ippcore
  NAMES ippcoreem64t
  HINTS $ENV{IPP_ROOT}/sharedlib
  HINTS $ENV{IPP_ROOT}/lib/intel64
)
find_library(IPPCV_LIB
  NAMES ippcv
  NAMES ippcvem64t
  HINTS $ENV{IPP_ROOT}/sharedlib
  HINTS $ENV{IPP_ROOT}/lib/intel64

)

find_library(IPPOMP_LIB
   NAMES iomp5
   HINTS /opt/intel/composer_xe_2013.1.117/compiler/lib/intel64
)
IF ( IPPOMP_FOUND)
  set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPOMP_LIB)
ENDIF( IPPOMP_FOUND)

message(STATUS "${IPPI_LIB}")
message(STATUS "${IPPS_LIB}")
message(STATUS "${IPPV_LIB}")
message(STATUS "${IPPC_LIB}")
message(STATUS "${IPPCV_LIB}")
message(STATUS "${IPPOMP_LIB}")
# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.

set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPI_LIB)
set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPS_LIB)
set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPV_LIB)
set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPC_LIB)
set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPCV_LIB)


libfind_process(IPP)
