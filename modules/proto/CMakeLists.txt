project(tactile-proto CXX)

set(TACTILE_PROTO_TARGET tactile-proto)

file(GLOB_RECURSE TACTILE_PROTO_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/*.proto"
     )

protobuf_generate_cpp(TACTILE_PROTO_GEN_SOURCE_FILES TACTILE_PROTO_GEN_HEADER_FILES "${TACTILE_PROTO_FILES}")

add_library(${TACTILE_PROTO_TARGET} STATIC "${TACTILE_PROTO_GEN_HEADER_FILES}" "${TACTILE_PROTO_GEN_SOURCE_FILES}")
add_library(tactile::proto ALIAS ${TACTILE_PROTO_TARGET})

set_target_properties(${TACTILE_PROTO_TARGET}
                      PROPERTIES
                      CXX_STANDARD "${TACTILE_CXX_STANDARD}"
                      CXX_EXTENSIONS "OFF"
                      CXX_STANDARD_REQUIRED "ON"
                      POSITION_INDEPENDENT_CODE "ON"
                      UNITY_BUILD "OFF"
                      RUNTIME_OUTPUT_DIRECTORY "${TACTILE_BUILD_DIR}"
                      ARCHIVE_OUTPUT_DIRECTORY "${TACTILE_BUILD_DIR}"
                      LIBRARY_OUTPUT_DIRECTORY "${TACTILE_BUILD_DIR}"
                      )

target_include_directories(${TACTILE_PROTO_TARGET} SYSTEM PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(${TACTILE_PROTO_TARGET} PRIVATE protobuf::libprotobuf)
