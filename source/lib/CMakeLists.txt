cmake_minimum_required(VERSION 3.16)

project(tactile-lib CXX)

file(GLOB_RECURSE SOURCE_FILES
     CONFIGURE_DEPENDS
     **/*.cpp
     **/*.hpp
     )

add_library(${TACTILE_LIB} ${SOURCE_FILES})
add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})

set_target_properties(${TACTILE_LIB}
                      PROPERTIES
                      CXX_STANDARD 23
                      CXX_EXTENSIONS OFF
                      )

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
                                          ${TACTILE_VENDOR_DIR}/centurion/src
                                          ${TACTILE_VENDOR_DIR}/fontawesome
                                          ${CPPCODEC_INCLUDE_DIRS}
                                          ${Stb_INCLUDE_DIR}
                                          ${VULKAN_INCLUDE_DIR}
                                          ${TACTILE_PROTO_GENERATED_DIR}
                                          )

target_link_libraries(${TACTILE_LIB}
                      PUBLIC
                      ${TACTILE_PROTO}
                      argparse::argparse
                      Boost::boost
                      EnTT::EnTT
                      fmt::fmt
                      glad::glad
                      glm::glm
                      imgui::imgui
                      magic_enum::magic_enum
                      nlohmann_json::nlohmann_json
                      OpenGL::GL
                      protobuf::libprotobuf
                      pugixml::pugixml
                      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
                      $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
                      spdlog::spdlog
                      Vulkan::Vulkan
                      unofficial::vulkan-memory-allocator::vulkan-memory-allocator
                      tinyfiledialogs::tinyfiledialogs
                      tl::expected
                      yaml-cpp
                      $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>
                      ZLIB::ZLIB
                      )

target_compile_definitions(${TACTILE_LIB}
                           PUBLIC
                           CENTURION_NO_SDL_MIXER
                           CENTURION_NO_SDL_TTF
                           IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                           IMGUI_DEFINE_MATH_OPERATORS
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

# The Intel macOS GitHub runners seemingly don't provide some of
# the required Vulkan subset definitions, so we condition on the
# presence of an Apple Silicon processor.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND
    CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
  target_compile_definitions(${TACTILE_LIB} PUBLIC TACTILE_USE_VULKAN_SUBSET)
endif ()

if (TACTILE_BUILD_APP_BUNDLE MATCHES ON)
  target_compile_definitions(${TACTILE_LIB} PUBLIC TACTILE_BUILD_APP_BUNDLE)
endif ()

tactile_set_compile_options(${TACTILE_LIB})

target_precompile_headers(${TACTILE_LIB} PRIVATE ${TACTILE_PRECOMPILED_HEADERS})
