# @brief Belledonne root cmake file

add_subdirectory(implementation)
add_subdirectory(src)

if (BUILD_TEST)
  add_subdirectory(tests)
endif (BUILD_TEST)
