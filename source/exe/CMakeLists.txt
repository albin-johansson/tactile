cmake_minimum_required(VERSION 3.16)

project(tactile-exe CXX)

file(GLOB_RECURSE SOURCE_FILES
     CONFIGURE_DEPENDS
     **/*.cpp
     **/*.hpp
     )

set(TACTILE_MAIN_FILE "main/main.cpp")

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  message("[Tactile]: Building executable as WIN32 (non-console) executable...")
  add_executable(${TACTILE_EXE} WIN32 ${SOURCE_FILES} ${TACTILE_MAIN_FILE})
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND TACTILE_BUILD_APP_BUNDLE MATCHES ON)
  message("[Tactile]: Building executable as application bundle...")

  file(GLOB_RECURSE TACTILE_BUNDLE_RESOURCES "${TACTILE_ASSETS_DIR}/*")

  foreach (FILE ${TACTILE_BUNDLE_RESOURCES})
    file(RELATIVE_PATH NEW_FILE "${TACTILE_ASSETS_DIR}/" ${FILE})
    get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
    set_property(SOURCE ${FILE}
                 PROPERTY MACOSX_PACKAGE_LOCATION "Resources/assets/${NEW_FILE_PATH}")
  endforeach ()

  # The app icon needs to be in the root Resources folder
  set_property(SOURCE "${TACTILE_ASSETS_DIR}/Tactile.icns" PROPERTY MACOSX_PACKAGE_LOCATION "Resources/")

  add_executable(${TACTILE_EXE} MACOSX_BUNDLE ${SOURCE_FILES} ${TACTILE_MAIN_FILE} ${TACTILE_BUNDLE_RESOURCES})

  set_target_properties(${TACTILE_EXE} PROPERTIES
                        MACOSX_BUNDLE TRUE
                        MACOSX_BUNDLE_GUI_IDENTIFIER "com.albinjohansson.tactile"
                        MACOSX_BUNDLE_BUNDLE_NAME "Tactile"
                        MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}"
                        MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
                        MACOSX_BUNDLE_LONG_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
                        MACOSX_BUNDLE_COPYRIGHT "GPL-3.0"
                        MACOSX_BUNDLE_ICON_FILE "Tactile"
                        )
else ()
  add_executable(${TACTILE_EXE} ${SOURCE_FILES} ${TACTILE_MAIN_FILE})
endif ()

set_target_properties(${TACTILE_EXE}
                      PROPERTIES
                      CXX_STANDARD 23
                      CXX_EXTENSIONS OFF
                      )

add_dependencies(${TACTILE_EXE} ${TACTILE_LIB})

tactile_set_compile_options(${TACTILE_EXE})

target_precompile_headers(${TACTILE_EXE} REUSE_FROM ${TACTILE_LIB})

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
  copy_directory_post_build(${TACTILE_EXE}
                            ${TACTILE_ASSETS_DIR}
                            "${CMAKE_CURRENT_BINARY_DIR}/assets")
endif ()