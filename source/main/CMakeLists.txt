project(tactile-main CXX)

set(TACTILE_MAIN "tactile")

file(GLOB_RECURSE TACTILE_MAIN_SOURCE_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_executable(tactile WIN32)

target_sources(tactile PRIVATE "${TACTILE_MAIN_SOURCE_FILES}")

tactile_prepare_target(tactile)

target_include_directories(tactile PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(tactile
                      PRIVATE
                      tactile::base
                      tactile::render
                      tactile::core
                      SDL2::SDL2
                      SDL2::SDL2main
                      )
