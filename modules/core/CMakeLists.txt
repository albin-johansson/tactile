project(tactile-core CXX)

set(TACTILE_CORE_TARGET tactile-core)

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_CORE_TARGET} SHARED "${TACTILE_CORE_SOURCE_FILES}")
add_library(tactile::core ALIAS ${TACTILE_CORE_TARGET})

tactile_prepare_target(${TACTILE_CORE_TARGET})

target_compile_definitions(${TACTILE_CORE_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_CORE
                           IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                           IMGUI_DEFINE_MATH_OPERATORS
                           )

target_include_directories(${TACTILE_CORE_TARGET}
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           PRIVATE
                           "${TACTILE_VENDOR_DIR}/fontawesome"
                           )

target_link_libraries(${TACTILE_CORE_TARGET}
                      PUBLIC
                      tactile::foundation
                      tactile::proto

                      PRIVATE
                      EnTT::EnTT
                      imgui::imgui
                      )

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()
