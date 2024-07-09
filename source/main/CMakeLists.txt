project(tactile-main CXX)

add_executable(tactile WIN32)

target_sources(tactile
               PRIVATE
               "${PROJECT_SOURCE_DIR}/main.cpp"
               )

tactile_prepare_target(tactile)

target_link_libraries(tactile
                      PRIVATE
                      tactile::base
                      tactile::runtime
                      SDL2::SDL2
                      SDL2::SDL2main
                      )
