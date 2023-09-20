project(tactile-main CXX)

set(TACTILE_MAIN_DIR "${TACTILE_ROOT_DIR}/modules/main")

file(GLOB_RECURSE TACTILE_MAIN_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_MAIN_DIR}/inc/**/*.hpp"
     "${TACTILE_MAIN_DIR}/src/**/*.hpp"
     "${TACTILE_MAIN_DIR}/src/**/*.cpp"
     )

add_executable(${TACTILE_MAIN_TARGET} WIN32 ${TACTILE_MAIN_SOURCE_FILES})

tactile_prepare_target(${TACTILE_MAIN_TARGET})

target_link_libraries(${TACTILE_MAIN_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}
                      ${TACTILE_EDITOR_TARGET}
                      )
