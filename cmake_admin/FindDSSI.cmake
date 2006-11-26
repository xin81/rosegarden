# - Try to find DSSI 0.9.0
# Once done this will define:
#
#  DSSI_FOUND - system has DSSI
#  DSSI_CFLAGS - Compiler switches required for using DSSI
#  DSSI_INC_DIR - Include diretory
#  DSSI_VERSION - DSSI version found

SET(CMAKE_INCLUDE_PATH ".")
INCLUDE(PkgConfigV)

PKGCONFIGV(dssi 0.9.0 _DSSIVersion _DSSIIncDir _DSSILinkDir _DSSILinkFlags _DSSICflags)

SET(DSSI_CFLAGS ${_DSSICflags})
SET(DSSI_INC_DIR ${_DSSIIncDir})
SET(DSSI_VERSION ${_DSSIVersion})

IF(DSSI_VERSION)
    SET(DSSI_FOUND TRUE)
ENDIF(DSSI_VERSION)

IF(NOT DSSI_FOUND)
    IF(DSSI_FIND_REQUIRED)
	MESSAGE(FATAL_ERROR "Could not find DSSI")
    ENDIF(DSSI_FIND_REQUIRED)
ENDIF(NOT DSSI_FOUND)

MARK_AS_ADVANCED(DSSI_CFLAGS)