project(tactile-yml-format CXX)

find_package(yaml-cpp CONFIG REQUIRED)

set(TACTILE_YML_FORMAT_TARGET tactile-yml-format)

file(GLOB_RECURSE TACTILE_YML_FORMAT_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_YML_FORMAT_TARGET} SHARED ${TACTILE_YML_FORMAT_SOURCE_FILES})
add_library(tactile::tactile-yml-format ALIAS ${TACTILE_YML_FORMAT_TARGET})

tactile_prepare_target(${TACTILE_YML_FORMAT_TARGET})

target_compile_definitions(${TACTILE_YML_FORMAT_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_TACTILE_YML_FORMAT
                           )

target_include_directories(${TACTILE_YML_FORMAT_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(${TACTILE_YML_FORMAT_TARGET}
                      PUBLIC
                      tactile::foundation
                      yaml-cpp::yaml-cpp
                      )

install(TARGETS ${TACTILE_YML_FORMAT_TARGET} DESTINATION "${TACTILE_BUILD_DIR}/plugins/formats")

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()
