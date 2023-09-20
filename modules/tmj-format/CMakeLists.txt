project(tactile-tmj-format CXX)

set(TACTILE_TMJ_FORMAT_DIR "${TACTILE_ROOT_DIR}/modules/tmj-format")

file(GLOB_RECURSE TACTILE_TMJ_FORMAT_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_TMJ_FORMAT_DIR}/inc/**/*.hpp"
     "${TACTILE_TMJ_FORMAT_DIR}/src/**/*.hpp"
     "${TACTILE_TMJ_FORMAT_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_TMJ_FORMAT_TARGET} SHARED ${TACTILE_TMJ_FORMAT_SOURCE_FILES})

tactile_prepare_target(${TACTILE_TMJ_FORMAT_TARGET})

target_compile_definitions(${TACTILE_TMJ_FORMAT_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_TMJ_FORMAT
                           )

target_include_directories(${TACTILE_TMJ_FORMAT_TARGET}
                           PUBLIC
                           "${TACTILE_TMJ_FORMAT_DIR}/inc"
                           )

target_link_libraries(${TACTILE_TMJ_FORMAT_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  set(TACTILE_TMJ_FORMAT_TEST_TARGET tactile-tmj-format-test)

  file(GLOB_RECURSE TACTILE_TMJ_FORMAT_TEST_FILES
       CONFIGURE_DEPENDS
       "${TACTILE_TMJ_FORMAT_DIR}/test/*.hpp"
       "${TACTILE_TMJ_FORMAT_DIR}/test/*.cpp"
       "${TACTILE_TMJ_FORMAT_DIR}/test/**/*.hpp"
       "${TACTILE_TMJ_FORMAT_DIR}/test/**/*.cpp"
       )

  add_executable(${TACTILE_TMJ_FORMAT_TEST_TARGET} ${TACTILE_TMJ_FORMAT_TEST_FILES})

  tactile_prepare_target(${TACTILE_TMJ_FORMAT_TEST_TARGET})

  target_link_libraries(${TACTILE_TMJ_FORMAT_TEST_TARGET}
                        PRIVATE
                        ${TACTILE_TMJ_FORMAT_TARGET}
                        GTest::gtest
                        )
endif ()