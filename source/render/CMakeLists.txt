project(tactile-render CXX)

add_library(tactile-render INTERFACE)
add_library(tactile::render ALIAS tactile-render)

target_include_directories(tactile-render INTERFACE "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(tactile-render INTERFACE tactile::base)
