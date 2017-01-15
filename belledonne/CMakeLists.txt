# @brief Belledonne root cmake file

add_subdirectory(src)
# The implementation depends on the lib, so it's included after
add_subdirectory(implementation)

if (BUILD_TEST)
  add_subdirectory(tests)
endif (BUILD_TEST)
