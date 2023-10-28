project(tactile-main CXX)

set(TACTILE_MAIN_DIR "${TACTILE_ROOT_DIR}/modules/main")

file(GLOB_RECURSE TACTILE_MAIN_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_MAIN_DIR}/inc/**/*.hpp"
     "${TACTILE_MAIN_DIR}/src/**/*.hpp"
     "${TACTILE_MAIN_DIR}/src/**/*.cpp"
     )

add_executable(${TACTILE_MAIN_TARGET} ${TACTILE_MAIN_SOURCE_FILES})

tactile_prepare_target(${TACTILE_MAIN_TARGET})

target_include_directories(${TACTILE_MAIN_TARGET} PUBLIC "${TACTILE_MAIN_DIR}/inc")

target_link_libraries(${TACTILE_MAIN_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}
                      ${TACTILE_EDITOR_TARGET}

                      PRIVATE
                      SDL2::SDL2
                      SDL2::SDL2main
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  set(TACTILE_INTEGRATION_TEST_TARGET tactile-integration-test)

  file(GLOB_RECURSE TACTILE_INTEGRATION_TEST_FILES
       CONFIGURE_DEPENDS
       "${TACTILE_MAIN_DIR}/test/*.hpp"
       "${TACTILE_MAIN_DIR}/test/*.cpp"
       "${TACTILE_MAIN_DIR}/test/**/*.hpp"
       "${TACTILE_MAIN_DIR}/test/**/*.cpp"
       )

  add_executable(${TACTILE_INTEGRATION_TEST_TARGET} ${TACTILE_INTEGRATION_TEST_FILES})

  tactile_prepare_target(${TACTILE_INTEGRATION_TEST_TARGET})

  target_include_directories(${TACTILE_INTEGRATION_TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

  # Note, here we explicitly link plugin modules to facilitate debugging.
  target_link_libraries(${TACTILE_INTEGRATION_TEST_TARGET}
                        PRIVATE
                        ${TACTILE_CORE_TARGET}
                        ${TACTILE_EDITOR_TARGET}
                        ${TACTILE_TMJ_FORMAT_TARGET}
                        GTest::gtest
                        )
endif ()
