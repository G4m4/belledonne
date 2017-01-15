# @brief Belledonne implementation root cmake file

add_subdirectory(common)

if (${BUILD_VSIX} STREQUAL "ON")
  add_subdirectory(plugin)
endif (${BUILD_VSIX} STREQUAL "ON")

add_subdirectory(standalone)
