cmake_minimum_required(VERSION 3.16)

project(tactile-app-io-parser CXX)

set(TACTILE_IO_PARSER_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/parse_error.cpp
    ${PROJECT_SOURCE_DIR}/parse_error.hpp
    PARENT_SCOPE)
