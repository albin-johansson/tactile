cmake_minimum_required(VERSION 3.16)

project(tactile-app CXX)

file(GLOB_RECURSE TACTILE_APP_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_INCLUDE_DIR}/tactile/app/*.hpp
     ${TACTILE_INCLUDE_DIR}/tactile/app/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     ${PROJECT_SOURCE_DIR}/*.cpp
     )

add_executable(${TACTILE_APP} EXCLUDE_FROM_ALL ${TACTILE_APP_SOURCE_FILES})

tactile_set_properties(${TACTILE_APP})
tactile_set_compiler_warnings(${TACTILE_APP})

target_include_directories(${TACTILE_APP} PUBLIC ${TACTILE_INCLUDE_DIR})

target_compile_definitions(${TACTILE_APP}
                           PRIVATE
                           TACTILE_BUILDING_APP
                           )

target_precompile_headers(${TACTILE_APP} PRIVATE ${TACTILE_STANDARD_HEADERS})

target_link_libraries(${TACTILE_APP}
                      PRIVATE
                      ${TACTILE_CORE}
                      ${TACTILE_EDITOR}
                      )