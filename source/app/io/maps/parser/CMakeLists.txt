cmake_minimum_required(VERSION 3.16)

project(tactile-app-io-maps-parser CXX)

set(TACTILE_IO_MAPS_PARSER_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/parse_data.cpp
    ${PROJECT_SOURCE_DIR}/parse_data.hpp
    ${PROJECT_SOURCE_DIR}/parse_error.cpp
    ${PROJECT_SOURCE_DIR}/parse_error.hpp
    ${PROJECT_SOURCE_DIR}/parse_map.cpp
    ${PROJECT_SOURCE_DIR}/parse_map.hpp

    ${PROJECT_SOURCE_DIR}/json/json_attribute_parser.cpp
    ${PROJECT_SOURCE_DIR}/json/json_attribute_parser.hpp
    ${PROJECT_SOURCE_DIR}/json/json_layer_parser.cpp
    ${PROJECT_SOURCE_DIR}/json/json_layer_parser.hpp
    ${PROJECT_SOURCE_DIR}/json/json_parser.cpp
    ${PROJECT_SOURCE_DIR}/json/json_parser.hpp
    ${PROJECT_SOURCE_DIR}/json/json_tileset_parser.cpp
    ${PROJECT_SOURCE_DIR}/json/json_tileset_parser.hpp

    ${PROJECT_SOURCE_DIR}/xml/xml_attribute_parser.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_attribute_parser.hpp
    ${PROJECT_SOURCE_DIR}/xml/xml_layer_parser.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_layer_parser.hpp
    ${PROJECT_SOURCE_DIR}/xml/xml_parser.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_parser.hpp
    ${PROJECT_SOURCE_DIR}/xml/xml_tileset_parser.cpp
    ${PROJECT_SOURCE_DIR}/xml/xml_tileset_parser.hpp

    ${PROJECT_SOURCE_DIR}/yaml/yaml_attribute_parser.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_attribute_parser.hpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_layer_parser.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_layer_parser.hpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_parser.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_parser.hpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_tileset_parser.cpp
    ${PROJECT_SOURCE_DIR}/yaml/yaml_tileset_parser.hpp

    PARENT_SCOPE)
