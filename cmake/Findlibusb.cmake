# Based on module from bladeRF:
# https://github.com/Nuand/bladeRF/blob/e09200c38e1b273ebe43cd789839e66b0506aead/host/cmake/modules/FindLibUSB.cmake

# This is a modified version of the file written by Hedrik Sattler,
# from the OpenOBEX project (licensed GPLv2/LGPL).  (If this is not correct,
# please contact us so we can attribute the author appropriately.)
#
# https://github.com/zuckschwerdt/openobex/blob/master/CMakeModules/FindLibUSB.cmake
# http://dev.zuckschwerdt.org/openobex/
#
# Find libusb-1.0
#
# It will use PkgConfig if present and supported, otherwise this
# script searches for binary distribution in the path defined by
# the LIBUSB_PATH variable.
#
# The following standard variables get defined:
#  LIBUSB_FOUND:            true if LibUSB was found
#  LIBUSB_HEADER_FILE:      the location of the C header file
#  LIBUSB_INCLUDE_DIRS:     the directorys that contain headers
#  LIBUSB_LIBRARIES:        the library files
#  LIBUSB_VERSION           the detected libusb version
#  LIBUSB_HAVE_GET_VERSION  True if libusb has libusb_get_version()

include(CheckLibraryExists)
include(CheckIncludeFile)

# Find libusb
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_LIBUSB QUIET libusb-1.0 libusb)
endif(PKG_CONFIG_FOUND)
if(PKGCONFIG_LIBUSB_FOUND AND NOT LIBUSB_FOUND)
    set(LIBUSB_INCLUDE_DIRS ${PKGCONFIG_LIBUSB_INCLUDE_DIRS})
    foreach(i ${PKGCONFIG_LIBUSB_LIBRARIES})
        string(REGEX MATCH "[^-]*" ibase "${i}")
        find_library(${ibase}_LIBRARY
            NAMES ${i}
            PATHS ${PKGCONFIG_LIBUSB_LIBRARY_DIRS}
           )
        if(${ibase}_LIBRARY)
            list(APPEND LIBUSB_LIBRARIES ${${ibase}_LIBRARY})
        endif(${ibase}_LIBRARY)
        mark_as_advanced(${ibase}_LIBRARY)
    endforeach(i)

else(PKGCONFIG_LIBUSB_FOUND)
    find_file(LIBUSB_HEADER_FILE
        NAMES
        libusb.h
        PATHS
        ${LIBUSB_PATH}
        ${LIBUSB_EXTRA_PATHS}
        PATH_SUFFIXES
        include include/libusbx-1.0 include/libusb-1.0
       )
    mark_as_advanced(LIBUSB_HEADER_FILE)
    get_filename_component(LIBUSB_INCLUDE_DIRS "${LIBUSB_HEADER_FILE}" PATH)

    find_library(usb_LIBRARY
        NAMES
        libusb-1.0 usb-1.0
        PATHS
        ${LIBUSB_PATH}
        ${LIBUSB_EXTRA_PATHS}
        PATH_SUFFIXES
        ${LIBUSB_LIBRARY_PATH_SUFFIX}
       )
    mark_as_advanced(usb_LIBRARY)
    if(usb_LIBRARY)
        set(LIBUSB_LIBRARIES ${usb_LIBRARY})
    endif(usb_LIBRARY)

endif(PKGCONFIG_LIBUSB_FOUND AND NOT LIBUSB_FOUND)

if(LIBUSB_INCLUDE_DIRS AND LIBUSB_LIBRARIES)
    set(LIBUSB_FOUND TRUE)
endif(LIBUSB_INCLUDE_DIRS AND LIBUSB_LIBRARIES)
