project(tactile-core CXX)

set(TACTILE_CORE tactile-core)

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/**/*.cpp"
     "${PROJECT_SOURCE_DIR}/**/*.hpp"
     )

add_library("${TACTILE_CORE}" STATIC "${TACTILE_CORE_SOURCE_FILES}")
add_library(tactile::core ALIAS "${TACTILE_CORE}")

tactile_prepare_target("${TACTILE_CORE}")

target_include_directories("${TACTILE_CORE}" PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries("${TACTILE_CORE}"
                      PUBLIC
                      tactile::base
                      fmt::fmt
                      )
