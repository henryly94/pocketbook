# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (yypocketbook)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)

include_directories(
    external/abseil-cpp/absl
    ${CMAKE_CURRENT_BINARY_DIR}/src
    ${PROTOBUF_INCLUDE_DIR}
)

# Setup ProtoBufs
include(FindProtobuf)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using Protobuf ${protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

add_subdirectory(src)

install(
    TARGETS main
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
