#  SERIAL_FOUND - system has Potrace
#  SERIAL_INCLUDE_DIR - the Potrace include directory
#  SERIAL_LIBRARIES - The libraries needed to use Potrace

FIND_PATH(SERIAL_INCLUDE_DIR SerialStream.h
    /usr/include
    /usr/local/include
    )

FIND_LIBRARY(SERIAL_LIBRARY NAMES serial libserial
    PATHS
    /usr/lib
    /usr/local/lib
    )

if (SERIAL_INCLUDE_DIR AND SERIAL_LIBRARY)
    set(SERIAL_FOUND TRUE)
    set(SERIAL_LIBRARIES ${SERIAL_LIBRARY})
else (SERIAL_INCLUDE_DIR AND SERIAL_LIBRARY)
    set(SERIAL_FOUND FALSE)
endif (SERIAL_INCLUDE_DIR AND SERIAL_LIBRARY)

if (SERIAL_FOUND)
    message(STATUS "Found LibSerial: ${SERIAL_LIBRARIES}")
else (SERIAL_FOUND)
    message(STATUS "*** Didn't find LibSerial")
endif (SERIAL_FOUND)

MARK_AS_ADVANCED(SERIAL_INCLUDE_DIR SERIAL_LIBRARIES SERIAL_LIBRARY)

