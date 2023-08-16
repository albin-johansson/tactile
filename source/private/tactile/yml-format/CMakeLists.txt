cmake_minimum_required(VERSION 3.16)

project(tactile-yml-format CXX)

file(GLOB_RECURSE TACTILE_YML_FORMAT_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_INCLUDE_DIR}/tactile/yml-format/*.hpp
     ${TACTILE_INCLUDE_DIR}/tactile/yml-format/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     ${PROJECT_SOURCE_DIR}/**.cpp
     )

add_library(${TACTILE_YML_FORMAT} SHARED ${TACTILE_YML_FORMAT_SOURCE_FILES})

tactile_set_properties(${TACTILE_YML_FORMAT})
tactile_set_compiler_warnings(${TACTILE_YML_FORMAT})
tactile_mark_as_plugin(${TACTILE_YML_FORMAT})

target_include_directories(${TACTILE_YML_FORMAT} PUBLIC ${TACTILE_INCLUDE_DIR})

target_compile_definitions(${TACTILE_YML_FORMAT}
                           PRIVATE
                           TACTILE_BUILDING_YML_FORMAT
                           )

target_precompile_headers(${TACTILE_YML_FORMAT} PRIVATE ${TACTILE_STANDARD_HEADERS})

target_link_libraries(${TACTILE_YML_FORMAT}
                      PRIVATE
                      ${TACTILE_CORE}
                      yaml-cpp
                      )