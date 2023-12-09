project(tactile-core CXX)

set(TACTILE_CORE_DIR "${TACTILE_ROOT_DIR}/modules/core")

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_CORE_DIR}/inc/**/*.hpp"
     "${TACTILE_CORE_DIR}/src/**/*.hpp"
     "${TACTILE_CORE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_CORE_TARGET} SHARED ${TACTILE_CORE_SOURCE_FILES})
add_library(tactile::core ALIAS ${TACTILE_CORE_TARGET})

tactile_prepare_target(${TACTILE_CORE_TARGET})

target_compile_definitions(${TACTILE_CORE_TARGET} PRIVATE TACTILE_BUILDING_CORE)

target_include_directories(${TACTILE_CORE_TARGET} PUBLIC "${TACTILE_CORE_DIR}/inc")

target_link_libraries(${TACTILE_CORE_TARGET}
                      PUBLIC
                      tactile::foundation

                      PRIVATE
                      imgui::imgui
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()
