# Thomas A Caswell, 2009
# Public domain
# - this module looks for HDF5
# Defines:
#  HDF5_LIBRARIES:   required libraries: libmex, etc



FIND_LIBRARY(HDF5_CPP_LIBRARY
  hdf5_cpp
  HINTS
#  /home/tcaswell/other_source/hdf/hdf5-1.6.5/hdf5/lib
#  /usr/lib
 /home/tcaswell/other_source/hdf/hdf5-1.8.3/hdf5/lib
  )

FIND_LIBRARY(HDF5_LIBRARY
  hdf5
  HINTS
#  /home/tcaswell/other_source/hdf/hdf5-1.6.5/hdf5/lib
  /home/tcaswell/other_source/hdf/hdf5-1.8.3/hdf5/lib
#  /usr/lib
  )


FIND_LIBRARY(Z_LIBRARY
  z
  /usr/lib
  )
FIND_LIBRARY(M_LIBRARY
  m
  /usr/lib
  )
FIND_PATH(HDF_INCLUDE_DIR
  "H5Cpp.h"
  HINTS
  "/home/tcaswell/other_source/hdf/hdf5-1.6.5/hdf5/include"
  )



# This is common to UNIX and Win32:
SET(HDF5_LIBRARIES
  ${HDF5_LIBRARY}
  ${HDF5_CPP_LIBRARY}
  ${Z_LIBRARY}
  ${M_LIBRARY}
  )

#
#message(   ${HDF5_LIBRARY}      )
#message(    ${HDF5_CPP_LIBRARY}	)
#message(    ${HDF5_HL_LIBRARY}	)
#message(    ${Z_LIBRARY}        )
#message(    ${M_LIBRARY}        )
