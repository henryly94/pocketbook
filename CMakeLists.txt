# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (yypocketbook)

add_subdirectory(external EXCLUDE_FROM_ALL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup ProtoBufs
include(FindProtobuf)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using Protobuf ${Protobuf_VERSION}")

include_directories(
    external/abseil-cpp
    external/grpc/include
    external/googletest/include
    ${CMAKE_CURRENT_BINARY_DIR}/src
    ${PROTOBUF_INCLUDE_DIR}
)
enable_testing()
add_subdirectory(src)

install(
    TARGETS main book_service_main book_client_main
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
