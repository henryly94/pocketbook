# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (yypocketbook)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)

include_directories(
    external/abseil-cpp/absl
)

set(INTERNAL_SOURCE
    src/book.cc
    src/entity.cc
)

set(EXTERNAL_SOURCE
)

set(EXTERNAL_LIBS
    absl::strings
    absl::flat_hash_map
    absl::time
)

add_executable(
    main
    src/book_main.cc
    ${INTERNAL_SOURCE}
    ${EXTERNAL_SOURCE}
)

target_link_libraries(
    main
    ${EXTERNAL_LIBS}
)