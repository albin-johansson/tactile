project(tactile-lib CXX)

file(GLOB_RECURSE SOURCE_FILES
     CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     )

add_library(${TACTILE_LIB} ${SOURCE_FILES})
add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})

tactile_prepare_target(${TACTILE_LIB})

target_include_directories(${TACTILE_LIB}
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}

                           SYSTEM PUBLIC
                           ${TACTILE_VENDOR_DIR}/centurion
                           ${TACTILE_VENDOR_DIR}/fontawesome
                           ${STB_INCLUDE_DIRS}
                           ${TACTILE_PROTO_GENERATED_DIR}
                           ${Boost_INCLUDE_DIRS}
                           ${CPPCODEC_INCLUDE_DIRS}
                           )

target_link_libraries(${TACTILE_LIB}
                      PUBLIC
                      ${TACTILE_PROTO}
                      ${TACTILE_CORE_TARGET}
                      Boost::boost
                      argparse::argparse
                      SDL2::SDL2
                      SDL2_image::SDL2_image
                      imgui::imgui
                      EnTT::EnTT
                      magic_enum::magic_enum
                      fmt::fmt
                      glm::glm
                      protobuf::libprotobuf
                      yaml-cpp
                      nlohmann_json::nlohmann_json
                      pugixml::pugixml
                      tinyfiledialogs::tinyfiledialogs
                      tl::expected
                      ZLIB::ZLIB
                      GLEW::GLEW
                      OpenGL::GL
                      spdlog::spdlog
                      zstd::libzstd_shared
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
                           BOOST_NO_CXX98_FUNCTION_BASE
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
