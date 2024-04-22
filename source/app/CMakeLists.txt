project(tactile-app CXX)

file(GLOB_RECURSE TACTILE_APP_SOURCE_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/**/*.cpp"
     "${PROJECT_SOURCE_DIR}/**/*.hpp"
     )

add_executable(tactile-exe
               "${TACTILE_APP_SOURCE_FILES}"
               "${PROJECT_SOURCE_DIR}/runtime/main.cpp"
               )

tactile_prepare_target(tactile-exe)

target_include_directories(tactile-exe
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           )

target_link_libraries(tactile-exe
                      PRIVATE
                      tactile::lib
                      SDL2::SDL2main
                      )

# Do not copy the resources when building as macOS app bundle
if (TACTILE_BUILD_APP_BUNDLE MATCHES "OFF")
  install(DIRECTORY "${TACTILE_RESOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/assets")
endif ()
