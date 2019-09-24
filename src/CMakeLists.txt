protobuf_generate_cpp(
    PROTO_SRC 
    PROTO_HEADER 
    messages.proto
)

find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin)
get_filename_component(ABS_FILE service.proto ABSOLUTE)
get_filename_component(DIR_FILE ${ABS_FILE} PATH)

set(service_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.cc")
set(service_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.h")
set(service_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.cc")
set(service_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.h")

add_custom_command(
    OUTPUT  "${service_proto_srcs}"
	    "${service_proto_hdrs}"
	    "${service_grpc_srcs}"
	    "${service_grpc_hdrs}"
    COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
      	   --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
           --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
	   -I "${DIR_FILE}"
	   "${ABS_FILE}"
    DEPENDS ${ABS_FILE} ${PROTOBUF_PROTOC_EXECUTABLE}
)

# Messages_Proto
add_library(
    messages_proto
    ${PROTO_SRC}
)
target_link_libraries(
    messages_proto
    ${PROTOBUF_LIBRARY}
)

# Service_Proto
add_library(
    service_proto
    ${service_proto_srcs}
)
target_link_libraries(
    service_proto
    ${PROTOBUF_LIBRARY}
)

# Service
add_library(
    service
    ${service_grpc_srcs}
)
target_link_libraries(
    service
    service_proto
    messages_proto
    grpc++_unsecure
)

# Entity
add_library(
    entity
    entity.cc
)
target_link_libraries(
    entity
    absl::flat_hash_map
    absl::flat_hash_set
)

# Book
add_library(
    book
    book.cc
)
target_link_libraries(
    book
    entity
    absl::strings
    absl::time
    absl::flat_hash_map
)    
 

# Util
add_library(
    util
    util.cc
)
target_link_libraries(
    util
    book
    messages_proto
    absl::time
)

# Book Main
add_executable(
    main
    book_main.cc
)
target_link_libraries(
    main
    book
    entity
)

# Book Service
add_executable(
    book_service_main
    book_service.cc
)
target_link_libraries(
    book_service_main
    book
    service
    util
    grpc++_unsecure
)

# Book Client
add_executable(
    book_client_main
    book_client.cc
)
target_link_libraries(
    book_client_main
    book
    service
    grpc++_unsecure
)
