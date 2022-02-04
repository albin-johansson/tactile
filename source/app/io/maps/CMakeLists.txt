cmake_minimum_required(VERSION 3.16)

project(tactile-app-io-maps CXX)

add_subdirectory(emitter)
add_subdirectory(parser)

set(TACTILE_IO_MAPS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/convert_document_to_ir.cpp
    ${PROJECT_SOURCE_DIR}/convert_document_to_ir.hpp
    ${PROJECT_SOURCE_DIR}/ir.hpp
    ${PROJECT_SOURCE_DIR}/json_utils.cpp
    ${PROJECT_SOURCE_DIR}/json_utils.hpp
    ${PROJECT_SOURCE_DIR}/restore_document_from_ir.cpp
    ${PROJECT_SOURCE_DIR}/restore_document_from_ir.hpp
    ${PROJECT_SOURCE_DIR}/save_document.cpp
    ${PROJECT_SOURCE_DIR}/save_document.hpp
    ${PROJECT_SOURCE_DIR}/tiled_info.hpp
    ${PROJECT_SOURCE_DIR}/xml_utils.cpp
    ${PROJECT_SOURCE_DIR}/xml_utils.hpp
    ${PROJECT_SOURCE_DIR}/yaml_utils.cpp
    ${PROJECT_SOURCE_DIR}/yaml_utils.hpp

    ${TACTILE_IO_MAPS_PARSER_SOURCE_FILES}
    ${TACTILE_IO_MAPS_EMITTER_SOURCE_FILES}

    PARENT_SCOPE)
