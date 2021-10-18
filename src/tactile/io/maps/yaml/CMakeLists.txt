cmake_minimum_required(VERSION 3.18)

project(tactile-io-yaml-parser CXX)

set(TACTILE_YAML_PARSER_SOURCES
    yaml_parser.hpp

    internal/parse_fancy_tiles.hpp
    internal/parse_layers.hpp
    internal/parse_object_layer.hpp
    internal/parse_properties.hpp
    internal/parse_tile_layer.hpp
    internal/parse_tilesets.hpp

    internal/parse_fancy_tiles.cpp
    internal/parse_layers.cpp
    internal/parse_object_layer.cpp
    internal/parse_properties.cpp
    internal/parse_tile_layer.cpp
    internal/parse_tilesets.cpp
    internal/yaml_parser.cpp
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