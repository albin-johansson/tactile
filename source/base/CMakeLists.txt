project(tactile-base CXX)

set(TACTILE_BASE tactile-base)

add_library("${TACTILE_BASE}" INTERFACE)
add_library(tactile::base ALIAS "${TACTILE_BASE}")

target_include_directories("${TACTILE_BASE}" INTERFACE "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries("${TACTILE_BASE}"
                      INTERFACE
                      tl::expected
                      )
