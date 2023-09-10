project(tactile-tmj-format-src CXX)

file(GLOB_RECURSE TACTILE_TMJ_FORMAT_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_TMJ_FORMAT_DIR}/inc/**/*.hpp
     ${TACTILE_TMJ_FORMAT_DIR}/src/**/*.hpp
     ${TACTILE_TMJ_FORMAT_DIR}/src/**/*.cpp
     )

add_library(${TACTILE_TMJ_FORMAT_TARGET} SHARED ${TACTILE_TMJ_FORMAT_SOURCE_FILES})

tactile_prepare_target(${TACTILE_TMJ_FORMAT_TARGET})

target_compile_definitions(${TACTILE_TMJ_FORMAT_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_TMJ_FORMAT
                           )

target_include_directories(${TACTILE_TMJ_FORMAT_TARGET} PUBLIC "${TACTILE_TMJ_FORMAT_DIR}/inc")

target_link_libraries(${TACTILE_TMJ_FORMAT_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}
                      )
