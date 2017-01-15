# Retrieve all implementation common source files

file(GLOB
     BELLEDONNE_IMPLEMENTATION_SRC
     *.cc
     *.cpp
)

# Expose variables to parent CMake files
set(BELLEDONNE_IMPLEMENTATION_SRC
    ${BELLEDONNE_IMPLEMENTATION_SRC}
    PARENT_SCOPE
)

file(GLOB
     BELLEDONNE_IMPLEMENTATION_HDR
     *.h
)

# Expose variables to parent CMake files
set(BELLEDONNE_IMPLEMENTATION_HDR
    ${BELLEDONNE_IMPLEMENTATION_HDR}
    PARENT_SCOPE
)
