project(tactile-main CXX)

add_executable(tactile WIN32)

target_sources(tactile
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/tactile/main.cpp"
               )

tactile_prepare_target(tactile)

target_include_directories(tactile PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(tactile
                      PRIVATE
                      tactile::base
                      tactile::core
                      tactile::runtime
                      SDL2::SDL2
                      SDL2::SDL2main
                      )
