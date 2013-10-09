#------------------------------------------------------------------------------
# Set up test options
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${MCHECKSUM_INCLUDES_BUILD_TIME}
)

#------------------------------------------------------------------------------
# Set up tests
#------------------------------------------------------------------------------
add_executable(simple_test simple_test.c)
target_link_libraries(simple_test mchecksum)
if(MCHECKSUM_ENABLE_COVERAGE)
  set_coverage_flags(simple_test)
endif()

set(TEST_HASH_TYPES crc16 crc64)
foreach(hash_type ${TEST_HASH_TYPES})
  add_test(NAME "${hash_type}"
    COMMAND $<TARGET_FILE:simple_test> "${hash_type}"
  )
endforeach()
