project(tactile-proto CXX)

find_package(protobuf CONFIG REQUIRED)

add_library(tactile-proto STATIC)
add_library(tactile::proto ALIAS tactile-proto)

list(APPEND
     TACTILE_PROTO_FILES
     "${PROJECT_SOURCE_DIR}/history.proto"
     "${PROJECT_SOURCE_DIR}/session.proto"
     "${PROJECT_SOURCE_DIR}/settings.proto"
     )

protobuf_generate_cpp(GENERATED_SOURCES GENERATED_HEADERS "${TACTILE_PROTO_FILES}")

target_sources(tactile-proto
               PRIVATE
               "${GENERATED_HEADERS}"
               "${GENERATED_SOURCES}"
               )

tactile_prepare_target(tactile-proto)

set_target_properties(tactile-proto
                      PROPERTIES
                      UNITY_BUILD "OFF"
                      )

target_include_directories(tactile-proto
                           SYSTEM PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

target_link_libraries(tactile-proto
                      PRIVATE
                      protobuf::libprotobuf
                      )
