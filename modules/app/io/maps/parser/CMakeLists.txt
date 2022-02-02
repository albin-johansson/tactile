cmake_minimum_required(VERSION 3.16)

project(tactile-app-io-maps-parser CXX)

set(TACTILE_IO_MAPS_PARSER_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/map_parser.cpp
    ${PROJECT_SOURCE_DIR}/map_parser.hpp
    ${PROJECT_SOURCE_DIR}/parse_data.cpp
    ${PROJECT_SOURCE_DIR}/parse_data.hpp
    ${PROJECT_SOURCE_DIR}/parse_error.cpp
    ${PROJECT_SOURCE_DIR}/parse_error.hpp

    ${PROJECT_SOURCE_DIR}/xml/xml_attributes.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_attributes.hpp
    ${PROJECT_SOURCE_DIR}/xml/xml_layers.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_layers.hpp
    ${PROJECT_SOURCE_DIR}/xml/xml_parser.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_parser.hpp
    ${PROJECT_SOURCE_DIR}/xml/xml_tilesets.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_tilesets.hpp

    ${PROJECT_SOURCE_DIR}/yaml/yaml_attributes.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_attributes.hpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_layers.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_layers.hpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_parser.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_parser.hpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_tilesets.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_tilesets.hpp

    PARENT_SCOPE)
