# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (yypocketbook)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)

include_directories(
    external/abseil-cpp/absl
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROTOBUF_INCLUDE_DIR}
)

# Setup ProtoBufs
include(FindProtobuf)
find_package(Protobuf REQUIRED)

protobuf_generate_cpp(
    PROTO_SRC 
    PROTO_HEADER 
    src/message.proto)

add_library(
    proto
    ${PROTO_SRC}
    ${PROTO_HEADER}
)

set(INTERNAL_LIBS
    proto
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
    ${PROTOBUF_LIBRARY}
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
    ${INTERNAL_LIBS}
)
