#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "assimp::assimp" for configuration "Release"
set_property(TARGET assimp::assimp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(assimp::assimp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libassimp.5.4.1.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libassimp.5.dylib"
  )

list(APPEND _cmake_import_check_targets assimp::assimp )
list(APPEND _cmake_import_check_files_for_assimp::assimp "${_IMPORT_PREFIX}/lib/libassimp.5.4.1.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
