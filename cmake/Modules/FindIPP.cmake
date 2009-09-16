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



SET(IPP_PROCESS_INCLUDE "/home/tcaswell/intel/ipp/5.3.4.080/em64t/include/")
SET(IPP_PROCESS_INCLUDES  IPP_PROCESS_INCLUDE)

# Finally the library itself
find_library(IPPI_LIB 
  NAMES ippiem64t    
)		     
		     
find_library(IPPS_LIB
  NAMES ippsem64t    
)		     
find_library(IPPV_LIB
  NAMES ippvmem64t
)
find_library(IPPC_LIB
  NAMES ippcoreem64t
)


message(STATUS "${IPPI_LIB}")
message(STATUS "${IPPS_LIB}")
message(STATUS "${IPPV_LIB}")
message(STATUS "${IPPC_LIB}")
# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.

set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPI_LIB)
set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPS_LIB)
set(IPP_PROCESS_LIBS ${IPP_PROCESS_LIBS} IPPV_LIB)


libfind_process(IPP)
