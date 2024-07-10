project(tactile-yaml-format-lib CXX)

find_package(yaml-cpp CONFIG REQUIRED)

add_library(tactile-yaml-format SHARED)
add_library(tactile::yaml_format ALIAS tactile-yaml-format)

target_sources(tactile-yaml-format
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/yaml_format/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/yaml_format/yaml_format_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/yaml_format/yaml_format_plugin.cpp"
               )

tactile_prepare_target(tactile-yaml-format)

target_include_directories(tactile-yaml-format PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_definitions(tactile-yaml-format PRIVATE "TACTILE_BUILDING_YAML_FORMAT")

target_link_libraries(tactile-yaml-format
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      yaml-cpp::yaml-cpp
                      )

