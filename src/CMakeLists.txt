# Setup ProtoBufs
include(FindProtobuf)
find_package(Protobuf REQUIRED)


protobuf_generate_cpp(
    PROTO_SRC 
    PROTO_HEADER 
    messages.proto
)

add_library(
    proto
    ${PROTO_SRC}
    ${PROTO_HEADER}
)

set(INTERNAL_LIBS
    proto
)

set(INTERNAL_SOURCE
    book.cc
    entity.cc
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
    book_main.cc
    ${INTERNAL_SOURCE}
    ${EXTERNAL_SOURCE}
)

target_link_libraries(
    main
    ${EXTERNAL_LIBS}
    ${INTERNAL_LIBS}
)


