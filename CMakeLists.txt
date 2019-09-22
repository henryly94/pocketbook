# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (yypocketbook)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)

include_directories(
    external/abseil-cpp
)

set(INTERNAL_SOURCE
)

set(EXTERNAL_SOURCE
)

set(EXTERNAL_LIBS
    absl::strings
    absl::container
)

add_executable(
    main
    src/book_main.cc
    ${EXTERNAL_SOURCE}
)

target_link_libraries(
    main
    ${EXTERNAL_LIBS}
)