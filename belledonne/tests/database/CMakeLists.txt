file(GLOB
     BELLEDONNE_TESTS_DATABASE_SRC
     *.cc
)

# Expose variables to parent CMake files
set(BELLEDONNE_TESTS_DATABASE_SRC
    ${BELLEDONNE_TESTS_DATABASE_SRC}
    PARENT_SCOPE
)
