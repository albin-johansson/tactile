cmake_minimum_required(VERSION 3.16)

project(tactile-lib CXX)

file(GLOB_RECURSE SOURCE_FILES
     CONFIGURE_DEPENDS
     **/*.cpp
     **/*.hpp
     )

add_library(${TACTILE_LIB} ${SOURCE_FILES})
add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Can't do this on MSVC for some reason, it causes issues with the precompiled headers
  set_target_properties(${TACTILE_LIB} PROPERTIES
                        PREFIX "lib"
                        OUTPUT_NAME "tactile"
                        )
endif ()

target_include_directories(${TACTILE_LIB} PUBLIC ${PROJECT_SOURCE_DIR})

tactile_target_include_system_directories(${TACTILE_LIB}
                                          SYSTEM PUBLIC
                                          ${LIBRARY_DIR}/centurion
                                          ${LIBRARY_DIR}/fontawesome
                                          ${STB_INCLUDE_DIRS}
                                          ${TACTILE_PROTO_GENERATED_DIR}
                                          ${CPPCODEC_INCLUDE_DIRS}
                                          )

target_link_libraries(${TACTILE_LIB}
                      PUBLIC
                      ${TACTILE_PROTO}
                      argparse::argparse
                      Boost::boost
                      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
                      $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
                      imgui::imgui
                      EnTT::EnTT
                      magic_enum::magic_enum
                      fmt::fmt
                      glm::glm
                      protobuf::libprotobuf
                      yaml-cpp
                      nlohmann_json::nlohmann_json
                      pugixml::static
                      tinyfiledialogs::tinyfiledialogs
                      tl::expected
                      ZLIB::ZLIB
                      GLEW::GLEW
                      OpenGL::GL
                      spdlog::spdlog
                      $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>
                      )

target_compile_definitions(${TACTILE_LIB}
                           PUBLIC
                           CENTURION_NO_SDL_MIXER
                           CENTURION_NO_SDL_TTF
                           IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                           IMGUI_DEFINE_MATH_OPERATORS
                           GLEW_NO_GLU
                           BOOST_ENABLE_ASSERT_DEBUG_HANDLER
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT
                           ENTT_STANDARD_CPP
                           SPDLOG_FMT_EXTERNAL
                           )

if (MSVC)
  target_compile_definitions(${TACTILE_LIB}
                             PUBLIC
                             WIN32_LEAN_AND_MEAN
                             NOMINMAX
                             )
endif ()

if (TACTILE_BUILD_APP_BUNDLE MATCHES ON)
  target_compile_definitions(${TACTILE_LIB} PUBLIC TACTILE_BUILD_APP_BUNDLE)
endif ()

tactile_set_compile_options(${TACTILE_LIB})

target_precompile_headers(${TACTILE_LIB} PRIVATE ${TACTILE_PRECOMPILED_HEADERS})
