project(tactile-editor CXX)

file(GLOB_RECURSE TACTILE_EDITOR_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_EDITOR_DIR}/inc/**/*.hpp
     ${TACTILE_EDITOR_DIR}/src/**/*.hpp
     ${TACTILE_EDITOR_DIR}/src/**/*.cpp
     )

add_library(${TACTILE_EDITOR_TARGET} SHARED ${TACTILE_EDITOR_SOURCE_FILES})

tactile_prepare_target(${TACTILE_EDITOR_TARGET})

target_compile_definitions(${TACTILE_EDITOR_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_EDITOR
                           )

target_include_directories(${TACTILE_EDITOR_TARGET} PUBLIC "${TACTILE_EDITOR_DIR}/inc")

target_link_libraries(${TACTILE_EDITOR_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}
                      )
