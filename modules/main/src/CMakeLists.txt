project(tactile-main CXX)

add_executable(${TACTILE_MAIN_TARGET} main.cpp)

tactile_prepare_target(${TACTILE_MAIN_TARGET})

target_link_libraries(${TACTILE_MAIN_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}
                      ${TACTILE_EDITOR_TARGET}
                      )
