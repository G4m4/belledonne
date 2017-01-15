Belledonne
==================================

Belledonne is a code navigation library.
Relying on libclang to parse the code, it provides various helpers to help browse through symbols once stored in a database.

Organisation
------------

The whole code lives in belledonne_lib; other targets are here to wrap it in various frameworks, including the test target belledonne_tests based on GTest.

The minimally required external dependencies are:
- libclang, referenced through a custom cmake file; it requires environment variables LIBCLANG_INCLUDE_PATH and LIBCLANG_LIBRARY_PATH to be set
- lmdb, directly integrated into the whole project
- lmdb++, which is header-only anyway
- optionally gtest

The last three are referenced as submodules of the git depot, it just works but you need to update them manually:

    git submodule init
    git submodule update

Building Belledonne
-------------------

Belledonne build system is Cmake.

Its most basic use would be, from the root directory belledonne/ :

    mkdir build && cd build
    cmake ..
    cmake --build .

Tests projects can be built by adding the proper option:

    cmake -DBUILD_TEST=ON ../

License
==================================
Belledonne is under GPLv3.

See the COPYING file for the full license text.
