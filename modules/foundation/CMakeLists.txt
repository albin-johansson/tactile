project(tactile-foundation CXX)

set(TACTILE_FOUNDATION_TARGET tactile-foundation)

file(GLOB_RECURSE TACTILE_FOUNDATION_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_FOUNDATION_TARGET} SHARED "${TACTILE_FOUNDATION_SOURCE_FILES}")
add_library(tactile::foundation ALIAS ${TACTILE_FOUNDATION_TARGET})

tactile_prepare_target(${TACTILE_FOUNDATION_TARGET})

target_compile_definitions(${TACTILE_FOUNDATION_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_FOUNDATION
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT

                           PUBLIC
                           WIN32_LEAN_AND_MEAN
                           NOMINMAX
                           GLM_FORCE_XYZW_ONLY
                           GLM_FORCE_SIZE_T_LENGTH
                           )

target_include_directories(${TACTILE_FOUNDATION_TARGET}
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )

target_link_libraries(${TACTILE_FOUNDATION_TARGET}
                      PUBLIC
                      glm::glm
                      fmt::fmt-header-only

                      PRIVATE
                      SDL2::SDL2
                      Boost::boost
                      FastFloat::fast_float
                      ZLIB::ZLIB
                      zstd::libzstd_static
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()
