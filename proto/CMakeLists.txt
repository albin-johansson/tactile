cmake_minimum_required(VERSION 3.18)

project(tactile-proto CXX)

set(TACTILE_GENERATED_PROTOBUF_FILES
    ${PROJECT_SOURCE_DIR}/generated/history.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/history.pb.h
    ${PROJECT_SOURCE_DIR}/generated/session.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/session.pb.h
    ${PROJECT_SOURCE_DIR}/generated/settings.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/settings.pb.h
    )

add_library(prototactile ${TACTILE_GENERATED_PROTOBUF_FILES})

target_include_directories(prototactile SYSTEM PUBLIC ${PROTOBUF_INCLUDE_DIRS})

target_link_libraries(prototactile PUBLIC ${PROTOBUF_LIBRARIES})