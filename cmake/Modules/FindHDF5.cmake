# Thomas A Caswell, 2009
# Public domain
# - this module looks for HDF5
# Defines:
#  HDF5_LIBRARIES:   required libraries: libmex, etc




FIND_LIBRARY(HDF5_HL_LIBRARY
  hdf5_hl
  )

FIND_LIBRARY(HDF5_CPP_LIBRARY
  hdf5_cpp
  )

FIND_LIBRARY(HDF5_LIBRARY
  hdf5
  )


# This is common to UNIX and Win32:
SET(HDF5_LIBRARIES
  ${HDF5_LIBRARY}
  ${HDF5_CPP_LIBRARY}
  ${HDF5_HL_LIBRARY}
)
