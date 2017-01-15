# Retrieve all dummygroup tests source files

file(GLOB
     BELLEDONNE_TESTS_TOOLS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(BELLEDONNE_TESTS_TOOLS_SRC
    ${BELLEDONNE_TESTS_TOOLS_SRC}
    PARENT_SCOPE
)
