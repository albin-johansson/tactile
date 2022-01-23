cmake_minimum_required(VERSION 3.16)

project(tactile-app-io CXX)

set(TACTILE_IO_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/convert_document_to_ir.cpp
    ${PROJECT_SOURCE_DIR}/convert_document_to_ir.hpp
    ${PROJECT_SOURCE_DIR}/directories.cpp
    ${PROJECT_SOURCE_DIR}/directories.hpp
    ${PROJECT_SOURCE_DIR}/file_dialog.cpp
    ${PROJECT_SOURCE_DIR}/file_dialog.hpp
    ${PROJECT_SOURCE_DIR}/history.cpp
    ${PROJECT_SOURCE_DIR}/history.hpp
    ${PROJECT_SOURCE_DIR}/map_parser.cpp
    ${PROJECT_SOURCE_DIR}/map_parser.hpp
    ${PROJECT_SOURCE_DIR}/preferences.cpp
    ${PROJECT_SOURCE_DIR}/preferences.hpp
    ${PROJECT_SOURCE_DIR}/save_document.cpp
    ${PROJECT_SOURCE_DIR}/save_document.hpp
    ${PROJECT_SOURCE_DIR}/session.cpp
    ${PROJECT_SOURCE_DIR}/session.hpp
    ${PROJECT_SOURCE_DIR}/create_document_from_ir.cpp
    ${PROJECT_SOURCE_DIR}/create_document_from_ir.hpp

    ${PROJECT_SOURCE_DIR}/emitter/emit_info.cpp
    ${PROJECT_SOURCE_DIR}/emitter/emit_info.hpp

    ${PROJECT_SOURCE_DIR}/emitter/yaml/yaml_emitter.cpp
    ${PROJECT_SOURCE_DIR}/emitter/yaml/yaml_emitter.hpp

    PARENT_SCOPE)
