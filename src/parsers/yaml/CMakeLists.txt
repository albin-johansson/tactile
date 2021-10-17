cmake_minimum_required(VERSION 3.18)

project(tactile-src-parsers-yaml CXX)

set(TACTILE_YAML_PARSER_SOURCES
    include/api.hpp
    include/parse_yaml_map.hpp

    private/parse_fancy_tiles.hpp
    private/parse_layers.hpp
    private/parse_object_layer.hpp
    private/parse_properties.hpp
    private/parse_tile_layer.hpp
    private/parse_tilesets.hpp

    private/parse_fancy_tiles.cpp
    private/parse_layers.cpp
    private/parse_object_layer.cpp
    private/parse_properties.cpp
    private/parse_tile_layer.cpp
    private/parse_tilesets.cpp
    private/parse_yaml_map.cpp
    )

add_library(TactileYAML SHARED ${TACTILE_YAML_PARSER_SOURCES})

target_include_directories(TactileYAML
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${ROOT_DIR}/src
    ${ROOT_DIR}/src/io/parsing

    SYSTEM PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${LIBRARY_DIR}/magic-enum
    ${LIBRARY_DIR}/tl
    ${LIBRARY_DIR}/nenya
    )

target_link_libraries(TactileYAML PUBLIC
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_LIBRARY}
    centurion
    yaml-cpp
    )

target_compile_definitions(TactileYAML PRIVATE
    CENTURION_NO_SDL_MIXER
    TACTILE_YAML_PARSER_EXPORT
    )