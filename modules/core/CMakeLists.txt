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
                           GLM_CONFIG_XYZW_ONLY=1
                           )

target_include_directories(${TACTILE_CORE_TARGET}
                           PUBLIC
                           "${TACTILE_CORE_DIR}/inc"
                           )

target_link_libraries(${TACTILE_CORE_TARGET}
                      PUBLIC
                      glm::glm
                      fmt::fmt-header-only

                      PRIVATE
                      Boost::boost
                      FastFloat::fast_float
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

  target_link_libraries(${TACTILE_CORE_TEST_TARGET}
                        PRIVATE
                        ${TACTILE_CORE_TARGET}
                        GTest::gtest
                        )
endif ()
