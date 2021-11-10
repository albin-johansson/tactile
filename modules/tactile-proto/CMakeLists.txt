cmake_minimum_required(VERSION 3.16)

project(tactile-proto CXX)

set(GENERATED_FILES
    generated/history.pb.cc
    generated/history.pb.h
    generated/session.pb.cc
    generated/session.pb.h
    generated/settings.pb.cc
    generated/settings.pb.h
    )

add_library(${TACTILE_PROTO} ${GENERATED_FILES})

target_link_libraries(${TACTILE_PROTO} PRIVATE
    protobuf::libprotobuf
    protobuf::libprotobuf-lite
    )

set(TACTILE_PROTO_GENERATED_DIR ${PROJECT_SOURCE_DIR}/generated PARENT_SCOPE)