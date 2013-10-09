cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
project(MCHECKSUM C)

#------------------------------------------------------------------------------
# Version information 
#------------------------------------------------------------------------------
set(MCHECKSUM_VERSION_MAJOR "0")
set(MCHECKSUM_VERSION_MINOR "1")
set(MCHECKSUM_VERSION_PATCH "0")
set(MCHECKSUM_PACKAGE "mchecksum")
set(MCHECKSUM_PACKAGE_NAME "MCHECKSUM")
set(MCHECKSUM_PACKAGE_VERSION "${MCHECKSUM_VERSION_MAJOR}.${MCHECKSUM_VERSION_MINOR}.${MCHECKSUM_VERSION_PATCH}")
set(MCHECKSUM_PACKAGE_VERSION_MAJOR "${MCHECKSUM_VERSION_MAJOR}.${MCHECKSUM_VERSION_MINOR}")
set(MCHECKSUM_PACKAGE_VERSION_MINOR "${MCHECKSUM_VERSION_PATCH}")
set(MCHECKSUM_PACKAGE_STRING "${MCHECKSUM_PACKAGE_NAME} ${MCHECKSUM_PACKAGE_VERSION}")
set(MCHECKSUM_PACKAGE_TARNAME "${MCHECKSUM_PACKAGE}")

#------------------------------------------------------------------------------
# Setup install and output Directories
#------------------------------------------------------------------------------
if(NOT MCHECKSUM_INSTALL_BIN_DIR)
  set(MCHECKSUM_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
endif()
if(NOT MCHECKSUM_INSTALL_LIB_DIR)
  set(MCHECKSUM_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
endif()
if(NOT MCHECKSUM_INSTALL_INCLUDE_DIR)
  set(MCHECKSUM_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif()
if(NOT MCHECKSUM_INSTALL_DATA_DIR)
  set(MCHECKSUM_INSTALL_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share)
endif()

#------------------------------------------------------------------------------
# Setup CMake Environment
#------------------------------------------------------------------------------
# Setup cmake policies.
foreach(policy CMP0012 CMP0013 CMP0014)
  if(POLICY ${policy})
    cmake_policy(SET ${policy} NEW)
  endif()
endforeach()

foreach(policy CMP0017)
  if(POLICY ${policy})
    cmake_policy(SET ${policy} OLD)
  endif()
endforeach()

#------------------------------------------------------------------------------
if(NOT MCHECKSUM_EXTERNALLY_CONFIGURED)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
endif()
set(MCHECKSUM_CMAKE_DIR "${MCHECKSUM_SOURCE_DIR}/CMake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MCHECKSUM_CMAKE_DIR})

#------------------------------------------------------------------------------
# Disallow in-source build
#------------------------------------------------------------------------------
if("${MCHECKSUM_SOURCE_DIR}" STREQUAL "${MCHECKSUM_BINARY_DIR}")
  message(FATAL_ERROR
    "MChecksum requires an out of source Build. "
    "Please create a separate binary directory and run CMake there.")
endif()

#------------------------------------------------------------------------------
# Set a default build type if none was specified
#------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects.
#-----------------------------------------------------------------------------
if(NOT MCHECKSUM_EXPORTED_TARGETS)
  set(MCHECKSUM_EXPORTED_TARGETS "mchecksum-targets")
endif()

#------------------------------------------------------------------------------
# Choose static or shared libraries.
#------------------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build with shared libraries." OFF)

#------------------------------------------------------------------------------
# Enable verbose error.
#------------------------------------------------------------------------------
option(MCHECKSUM_ENABLE_VERBOSE_ERROR "Print error messages to stderr." ON)

#-------------------------------------------------------------------------------
function(set_lib_options libtarget libname libtype)
  if(${libtype} MATCHES "SHARED")
    if(WIN32 AND NOT MINGW)
      set(LIB_RELEASE_NAME "${libname}")
      set(LIB_DEBUG_NAME "${libname}_D")
      set(LIB_VERSION ${MCHECKSUM_PACKAGE_VERSION_MAJOR})
    else()
      set(LIB_RELEASE_NAME "${libname}")
      set(LIB_DEBUG_NAME "${libname}_debug")
      set(LIB_VERSION ${MCHECKSUM_PACKAGE_VERSION})
    endif()
  else()
    if(WIN32 AND NOT MINGW)
      set(LIB_RELEASE_NAME "lib${libname}")
      set(LIB_DEBUG_NAME "lib${libname}_D")
    else()
      # if the generator supports configuration types or if the CMAKE_BUILD_TYPE has a value
      if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        set(LIB_RELEASE_NAME "${libname}")
        set(LIB_DEBUG_NAME "${libname}_debug")
      else()
        set(LIB_RELEASE_NAME "lib${libname}")
        set(LIB_DEBUG_NAME "lib${libname}_debug")
      endif()
    endif()
  endif()

  set_target_properties(${libtarget}
      PROPERTIES
      DEBUG_OUTPUT_NAME          ${LIB_DEBUG_NAME}
      RELEASE_OUTPUT_NAME        ${LIB_RELEASE_NAME}
      MINSIZEREL_OUTPUT_NAME     ${LIB_RELEASE_NAME}
      RELWITHDEBINFO_OUTPUT_NAME ${LIB_RELEASE_NAME}
      VERSION                    ${LIB_VERSION}
      SOVERSION                  ${LIB_VERSION}
  )

  #----- Use MSVC Naming conventions for Shared Libraries
  if(MINGW AND ${libtype} MATCHES "SHARED")
    set_target_properties(${libtarget}
        PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
    )
  endif()
endfunction()

#-----------------------------------------------------------------------------
# Coverage
#-----------------------------------------------------------------------------
option(MCHECKSUM_ENABLE_COVERAGE "Enable coverage." OFF)
if(MCHECKSUM_ENABLE_COVERAGE)
  set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags to the coverage program that CTest uses to perform coverage inspection"
  )
  mark_as_advanced(COVERAGE_FLAGS)
endif()

macro(set_coverage_flags target)
  set_target_properties(${target}
    PROPERTIES
      COMPILE_FLAGS ${COVERAGE_FLAGS}
      LINK_FLAGS ${COVERAGE_FLAGS}
  )
endmacro()

#-----------------------------------------------------------------------------
# Source
#-----------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

#-----------------------------------------------------------------------------
# Build doxygen documentation.
#-----------------------------------------------------------------------------
#option(BUILD_DOCUMENTATION "Build documentation." OFF)
#if(BUILD_DOCUMENTATION)
#  add_subdirectory(Documentation/Doxygen)
#endif()

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------
option(BUILD_TESTING "Build testing." OFF)
if(NOT MCHECKSUM_EXTERNALLY_CONFIGURED AND BUILD_TESTING)
  enable_testing()
#  include(CTest)
  add_subdirectory(Testing)
endif()

#-----------------------------------------------------------------------------
# Configure the config.cmake file for the build directory
#-----------------------------------------------------------------------------
configure_file(
  ${MCHECKSUM_SOURCE_DIR}/CMake/mchecksum-config.cmake.build.in
  ${MCHECKSUM_BINARY_DIR}/mchecksum-config.cmake @ONLY
)

#-----------------------------------------------------------------------------
# Configure the config.cmake file for the install directory
#-----------------------------------------------------------------------------
configure_file(
  ${MCHECKSUM_SOURCE_DIR}/CMake/mchecksum-config.cmake.install.in
  ${MCHECKSUM_BINARY_DIR}/CMakeFiles/mchecksum-config.cmake @ONLY
)

install(
  FILES
    ${MCHECKSUM_BINARY_DIR}/CMakeFiles/mchecksum-config.cmake
  DESTINATION
    ${MCHECKSUM_INSTALL_DATA_DIR}/cmake/mchecksum
)

#-----------------------------------------------------------------------------
# Configure the hdf5-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
configure_file(
  ${MCHECKSUM_SOURCE_DIR}/CMake/mchecksum-config-version.cmake.in
  ${MCHECKSUM_BINARY_DIR}/CMakeFiles/mchecksum-config-version.cmake @ONLY
)

install(
  FILES
    ${MCHECKSUM_BINARY_DIR}/CMakeFiles/mchecksum-config-version.cmake
  DESTINATION
    ${MCHECKSUM_INSTALL_DATA_DIR}/cmake/mchecksum
)

#-----------------------------------------------------------------------------
# For automake compatibility, also a provide a pkgconfig file
#-----------------------------------------------------------------------------
if(NOT WIN32 AND NOT APPLE)
  configure_file(
    ${MCHECKSUM_SOURCE_DIR}/CMake/mchecksum.pc.in
    ${MCHECKSUM_BINARY_DIR}/CMakeFiles/mchecksum.pc @ONLY
  )

  install(
    FILES
      ${MCHECKSUM_BINARY_DIR}/CMakeFiles/mchecksum.pc
    DESTINATION
      ${MCHECKSUM_INSTALL_LIB_DIR}/pkgconfig
  )
endif()

#-----------------------------------------------------------------------------
# CPack
#-----------------------------------------------------------------------------
set(CPACK_PACKAGE_DESCRIPTION_FILE ${MCHECKSUM_SOURCE_DIR}/README)
set(CPACK_RESOURCE_FILE_LICENSE ${MCHECKSUM_SOURCE_DIR}/COPYING)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Generic Checksum Library")
set(CPACK_PACKAGE_NAME "MChecksum")
set(CPACK_PACKAGE_VENDOR "Argonne National Laboratories / The HDF Group")
set(CPACK_PACKAGE_VERSION_MAJOR ${MCHECKSUM_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MCHECKSUM_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${MCHECKSUM_VERSION_PATCH})
set(CPACK_GENERATOR "TBZ2")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${MCHECKSUM_PACKAGE_TARNAME}-${MCHECKSUM_PACKAGE_VERSION})
set(CPACK_SOURCE_IGNORE_FILES ".git*;/GitSetup/;/.git/;.swp$;.#;/#;.*~")
set(CPACK_SOURCE_STRIP_FILES "")
include(CPack)
