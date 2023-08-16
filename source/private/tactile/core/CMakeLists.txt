cmake_minimum_required(VERSION 3.16)

project(tactile-core CXX)

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_INCLUDE_DIR}/tactile/core/*.hpp
     ${TACTILE_INCLUDE_DIR}/tactile/core/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     )

add_library(${TACTILE_CORE} SHARED ${TACTILE_CORE_SOURCE_FILES})

tactile_set_properties(${TACTILE_CORE})
tactile_set_compiler_warnings(${TACTILE_CORE})

target_include_directories(${TACTILE_CORE} PUBLIC ${TACTILE_INCLUDE_DIR})

target_compile_definitions(${TACTILE_CORE}
                           PRIVATE
                           TACTILE_BUILDING_CORE
                           WIN32_LEAN_AND_MEAN
                           NOMINMAX
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           )

target_precompile_headers(${TACTILE_CORE} PRIVATE ${TACTILE_STANDARD_HEADERS})

target_link_libraries(${TACTILE_CORE}
                      PRIVATE
                      protobuf::libprotobuf
                      fmt::fmt
                      Boost::boost
                      glm::glm
                      SDL2::SDL2
                      )