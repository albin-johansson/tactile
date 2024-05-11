project(tactile-core CXX)

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(tactile-core STATIC)
add_library(tactile::core ALIAS tactile-core)

target_sources(tactile-core PRIVATE "${TACTILE_CORE_SOURCE_FILES}")

tactile_prepare_target(tactile-core)

target_include_directories(tactile-core
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           SYSTEM PUBLIC
                           "${TACTILE_VENDOR_DIR}/centurion"
                           "${TACTILE_VENDOR_DIR}/fontawesome"
                           )

target_compile_definitions(tactile-core
                           PUBLIC
                           "IMGUI_DEFINE_MATH_OPERATORS"

                           PRIVATE
                           "BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED"
                           "BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT"
                           "CENTURION_NO_SDL_IMAGE"
                           "CENTURION_NO_SDL_MIXER"
                           "CENTURION_NO_SDL_TTF"
                           "WIN32_LEAN_AND_MEAN"
                           "NOMINMAX"
                           )

target_link_libraries(tactile-core
                      PUBLIC
                      tactile::base
                      tactile::render
                      EnTT::EnTT
                      fmt::fmt
                      imgui::imgui

                      PRIVATE
                      SDL2::SDL2
                      Boost::boost
                      FastFloat::fast_float
                      magic_enum::magic_enum
                      protobuf::libprotobuf
                      tinyfiledialogs::tinyfiledialogs
                      ZLIB::ZLIB
                      $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>
                      )
