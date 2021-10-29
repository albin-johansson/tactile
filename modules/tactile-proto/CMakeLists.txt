cmake_minimum_required(VERSION 3.16)

project(tactile-proto CXX)

if (LINUX)
  find_package(Protobuf)
endif ()

set(GENERATED_FILES
    generated/history.pb.cc
    generated/history.pb.h
    generated/session.pb.cc
    generated/session.pb.h
    generated/settings.pb.cc
    generated/settings.pb.h
    )

add_library(${TACTILE_PROTO} ${GENERATED_FILES})

target_include_directories(${TACTILE_PROTO} SYSTEM PUBLIC ${PROTOBUF_INCLUDE_DIRS})

target_link_libraries(${TACTILE_PROTO} PUBLIC protobuf::libprotobuf)

set_property(TARGET ${TACTILE_PROTO} PROPERTY POSITION_INDEPENDENT_CODE ON)

set(TACTILE_PROTO_GENERATED_DIR ${PROJECT_SOURCE_DIR}/generated PARENT_SCOPE)