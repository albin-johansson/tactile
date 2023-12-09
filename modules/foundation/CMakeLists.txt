project(tactile-foundation CXX)

set(TACTILE_FOUNDATION_DIR "${TACTILE_ROOT_DIR}/modules/foundation")

file(GLOB_RECURSE TACTILE_FOUNDATION_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_FOUNDATION_DIR}/inc/**/*.hpp"
     "${TACTILE_FOUNDATION_DIR}/src/**/*.hpp"
     "${TACTILE_FOUNDATION_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_FOUNDATION_TARGET} SHARED ${TACTILE_FOUNDATION_SOURCE_FILES})
add_library(tactile::foundation ALIAS ${TACTILE_FOUNDATION_TARGET})

tactile_prepare_target(${TACTILE_FOUNDATION_TARGET})

target_compile_definitions(${TACTILE_FOUNDATION_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_FOUNDATION
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT

                           PUBLIC
                           GLM_CONFIG_XYZW_ONLY=1
                           )

target_include_directories(${TACTILE_FOUNDATION_TARGET}
                           PUBLIC
                           "${TACTILE_FOUNDATION_DIR}/inc"

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
