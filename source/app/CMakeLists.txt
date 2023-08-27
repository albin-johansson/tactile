project(tactile-app CXX)

file(GLOB_RECURSE SOURCE_FILES
     CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     )

add_executable(${TACTILE_EXE} ${SOURCE_FILES} runtime/main.cpp)

tactile_prepare_target(${TACTILE_EXE})

target_include_directories(${TACTILE_EXE}
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}
                           )

target_link_libraries(${TACTILE_EXE}
                      PRIVATE
                      ${TACTILE_LIB}
                      SDL2::SDL2main
                      )

# Do not copy the resources when building as macOS app bundle
if (TACTILE_BUILD_APP_BUNDLE MATCHES OFF)
  tactile_copy_directory_post_build(${TACTILE_EXE}
                                    "${TACTILE_RESOURCE_DIR}"
                                    "${TACTILE_BUILD_DIR}/assets")
endif ()