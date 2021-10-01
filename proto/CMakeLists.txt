cmake_minimum_required(VERSION 3.18)

project(tactile-proto CXX)

set(GENERATED_PROTOBUF_FILES
    ${PROJECT_SOURCE_DIR}/generated/history.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/history.pb.h
    ${PROJECT_SOURCE_DIR}/generated/session.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/session.pb.h
    ${PROJECT_SOURCE_DIR}/generated/settings.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/settings.pb.h
    )

add_library(${TACTILE_PROTO} ${GENERATED_PROTOBUF_FILES})

target_include_directories(${TACTILE_PROTO} SYSTEM PUBLIC ${PROTOBUF_INCLUDE_DIRS})

target_link_libraries(${TACTILE_PROTO} PUBLIC ${PROTOBUF_LIBRARIES})