cmake_minimum_required(VERSION 3.16)

project(tactile-proto CXX)

set(PROTO_FILES
    history.proto
    session.proto
    settings.proto)

protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS ${PROTO_FILES})

add_library(${TACTILE_PROTO} ${PROTO_HEADERS} ${PROTO_SOURCES})

target_include_directories(${TACTILE_PROTO} SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${TACTILE_PROTO} PRIVATE
                      protobuf::libprotobuf
                      protobuf::libprotobuf-lite
                      )