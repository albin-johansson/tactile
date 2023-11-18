project(tactile-core CXX)

set(TACTILE_CORE_DIR "${TACTILE_ROOT_DIR}/modules/core")

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_CORE_DIR}/inc/**/*.hpp"
     "${TACTILE_CORE_DIR}/src/**/*.hpp"
     "${TACTILE_CORE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_CORE_TARGET} SHARED ${TACTILE_CORE_SOURCE_FILES})

tactile_prepare_target(${TACTILE_CORE_TARGET})

target_compile_definitions(${TACTILE_CORE_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_CORE
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT
                           GLM_CONFIG_XYZW_ONLY=1
                           )

target_include_directories(${TACTILE_CORE_TARGET}
                           PUBLIC
                           "${TACTILE_CORE_DIR}/inc"

                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )

target_link_libraries(${TACTILE_CORE_TARGET}
                      PUBLIC
                      glm::glm
                      fmt::fmt-header-only

                      PRIVATE
                      Boost::boost
                      FastFloat::fast_float
                      ZLIB::ZLIB
                      zstd::libzstd_static
                      SDL2::SDL2
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  set(TACTILE_CORE_TEST_TARGET tactile-core-test)

  file(GLOB_RECURSE TACTILE_CORE_TEST_FILES
       CONFIGURE_DEPENDS
       "${TACTILE_CORE_DIR}/test/*.hpp"
       "${TACTILE_CORE_DIR}/test/*.cpp"
       "${TACTILE_CORE_DIR}/test/**/*.hpp"
       "${TACTILE_CORE_DIR}/test/**/*.cpp"
       )

  add_executable(${TACTILE_CORE_TEST_TARGET} ${TACTILE_CORE_TEST_FILES})

  tactile_prepare_target(${TACTILE_CORE_TEST_TARGET})

  target_include_directories(${TACTILE_CORE_TEST_TARGET}
                             PRIVATE
                             "${TACTILE_CORE_DIR}/test"
                             )

  target_link_libraries(${TACTILE_CORE_TEST_TARGET}
                        PRIVATE
                        ${TACTILE_CORE_TARGET}
                        GTest::gmock
                        )

  if (WIN32)
    add_custom_command(TARGET ${TACTILE_CORE_TEST_TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TACTILE_CORE_TEST_TARGET}> $<TARGET_FILE_DIR:${TACTILE_CORE_TEST_TARGET}>
                       COMMAND_EXPAND_LISTS
                       )
  endif ()
endif ()
