project(tactile-proto CXX)

set(TACTILE_PROTO tactile-proto)

file(GLOB_RECURSE TACTILE_PROTO_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/*.proto"
     )

protobuf_generate_cpp(TACTILE_PROTO_GEN_SOURCE_FILES TACTILE_PROTO_GEN_HEADER_FILES "${TACTILE_PROTO_FILES}")

add_library(${TACTILE_PROTO} STATIC)
add_library(tactile::proto ALIAS ${TACTILE_PROTO})

target_sources(${TACTILE_PROTO} PRIVATE "${TACTILE_PROTO_GEN_HEADER_FILES}" "${TACTILE_PROTO_GEN_SOURCE_FILES}")

tactile_prepare_target(${TACTILE_PROTO})

set_target_properties(${TACTILE_PROTO} PROPERTIES UNITY_BUILD "OFF")

target_include_directories(${TACTILE_PROTO} SYSTEM PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(${TACTILE_PROTO} PRIVATE protobuf::libprotobuf)
