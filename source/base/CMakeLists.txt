project(tactile-base CXX)

add_library(tactile-base INTERFACE)
add_library(tactile::base ALIAS tactile-base)

target_include_directories(tactile-base INTERFACE "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(tactile-base
                      INTERFACE
                      tl::expected
                      )
