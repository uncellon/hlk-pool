#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Hlk::Pool" for configuration "Debug"
set_property(TARGET Hlk::Pool APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Hlk::Pool PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libHlkPool.so"
  IMPORTED_SONAME_DEBUG "libHlkPool.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS Hlk::Pool )
list(APPEND _IMPORT_CHECK_FILES_FOR_Hlk::Pool "${_IMPORT_PREFIX}/lib/libHlkPool.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
