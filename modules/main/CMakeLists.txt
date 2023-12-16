project(tactile-main CXX)

set(TACTILE_MAIN_TARGET tactile)

file(GLOB_RECURSE TACTILE_MAIN_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_executable(${TACTILE_MAIN_TARGET} "${TACTILE_MAIN_SOURCE_FILES}")

tactile_prepare_target(${TACTILE_MAIN_TARGET})

target_include_directories(${TACTILE_MAIN_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(${TACTILE_MAIN_TARGET}
                      PUBLIC
                      tactile::foundation
                      tactile::core

                      PRIVATE
                      SDL2::SDL2
                      SDL2::SDL2main
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()
