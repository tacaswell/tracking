# Thomas A Caswell, 2009
# Public domain
# - this module looks for HDF5
# Defines:
#  HDF5_LIBRARIES:   required libraries: libmex, etc



FIND_LIBRARY(HDF5_HL_LIBRARY
  hdf5_hl
  /usr/lib
  )

FIND_LIBRARY(HDF5_CPP_LIBRARY
  hdf5_cpp
  /usr/lib
  )

FIND_LIBRARY(HDF5_LIBRARY
  hdf5
  /usr/lib
  )


FIND_LIBRARY(Z_LIBRARY
  z
  /usr/lib
  )
FIND_LIBRARY(M_LIBRARY
  m
  /usr/lib
  )




# This is common to UNIX and Win32:
SET(HDF5_LIBRARIES
  ${HDF5_LIBRARY}
  ${HDF5_CPP_LIBRARY}
  ${HDF5_HL_LIBRARY}
  ${Z_LIBRARY}
  ${M_LIBRARY}
  )

#
#message(   ${HDF5_LIBRARY}      )
#message(    ${HDF5_CPP_LIBRARY}	)
#message(    ${HDF5_HL_LIBRARY}	)
#message(    ${Z_LIBRARY}        )
#message(    ${M_LIBRARY}        )
