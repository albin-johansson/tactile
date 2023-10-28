project(tactile-editor CXX)

set(TACTILE_EDITOR_DIR "${TACTILE_ROOT_DIR}/modules/editor")

file(GLOB_RECURSE TACTILE_EDITOR_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_EDITOR_DIR}/inc/**/*.hpp"
     "${TACTILE_EDITOR_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_EDITOR_TARGET} SHARED ${TACTILE_EDITOR_SOURCE_FILES})

tactile_prepare_target(${TACTILE_EDITOR_TARGET})

target_compile_definitions(${TACTILE_EDITOR_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_EDITOR
                           )

target_include_directories(${TACTILE_EDITOR_TARGET}
                           PUBLIC
                           "${TACTILE_EDITOR_DIR}/inc"
                           )

target_link_libraries(${TACTILE_EDITOR_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}

                      PRIVATE
                      SDL2::SDL2
                      imgui::imgui
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  set(TACTILE_EDITOR_TEST_TARGET tactile-editor-test)

  file(GLOB_RECURSE TACTILE_EDITOR_TEST_FILES
       CONFIGURE_DEPENDS
       "${TACTILE_EDITOR_DIR}/test/*.hpp"
       "${TACTILE_EDITOR_DIR}/test/*.cpp"
       "${TACTILE_EDITOR_DIR}/test/**/*.hpp"
       "${TACTILE_EDITOR_DIR}/test/**/*.cpp"
       )

  add_executable(${TACTILE_EDITOR_TEST_TARGET} ${TACTILE_EDITOR_TEST_FILES})

  tactile_prepare_target(${TACTILE_EDITOR_TEST_TARGET})

  target_link_libraries(${TACTILE_EDITOR_TEST_TARGET}
                        PRIVATE
                        ${TACTILE_EDITOR_TARGET}
                        GTest::gtest
                        )

  if (WIN32)
    add_custom_command(TARGET ${TACTILE_EDITOR_TEST_TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TACTILE_EDITOR_TEST_TARGET}> $<TARGET_FILE_DIR:${TACTILE_EDITOR_TEST_TARGET}>
                       COMMAND_EXPAND_LISTS
                       )
  endif ()
endif ()
