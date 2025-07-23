# Copyright (C) 2025 Albin Johansson

# Determine build type, e.g. "debug" or "release".
string(TOLOWER "${CMAKE_BUILD_TYPE}" TACTILE_BUILD_TYPE)
if (NOT (TACTILE_BUILD_TYPE MATCHES "debug|release|asan"))
  message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif ()
message(DEBUG "TACTILE_BUILD_TYPE: ${TACTILE_BUILD_TYPE}")

set(TACTILE_MODULE_DIR "${PROJECT_SOURCE_DIR}/modules")
set(TACTILE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(TACTILE_TEST_DIR "${PROJECT_SOURCE_DIR}/tests")
set(TACTILE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/${TACTILE_BUILD_TYPE}/output")

message(DEBUG "TACTILE_MODULE_DIR: ${TACTILE_MODULE_DIR}")
message(DEBUG "TACTILE_SOURCE_DIR: ${TACTILE_SOURCE_DIR}")
message(DEBUG "TACTILE_TEST_DIR: ${TACTILE_TEST_DIR}")
message(DEBUG "TACTILE_BINARY_DIR: ${TACTILE_BINARY_DIR}")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/data" DESTINATION "${TACTILE_BINARY_DIR}")

function(tactile_set_target_properties target)
  set_target_properties(${target}
                        PROPERTIES
                        PREFIX ""
                        POSITION_INDEPENDENT_CODE "ON"
                        INTERPROCEDURAL_OPTIMIZATION "${TACTILE_ENABLE_LTO}"
                        RUNTIME_OUTPUT_DIRECTORY "${TACTILE_BINARY_DIR}"
                        ARCHIVE_OUTPUT_DIRECTORY "${TACTILE_BINARY_DIR}"
                        LIBRARY_OUTPUT_DIRECTORY "${TACTILE_BINARY_DIR}"
                        )
endfunction()

if (MSVC)
  list(APPEND
       TACTILE_COMPILE_OPTIONS
       "/EHsc"
       "/MP"
       "/W4"
       "/bigobj"
       "/permissive-"
       "/Zc:preprocessor"
       "/Zc:__cplusplus"
       )
else ()
  list(APPEND
       TACTILE_COMPILE_OPTIONS
       "-fvisibility=hidden"
       "-Wall"
       "-Wextra"
       "-Wpedantic"
       "-Wconversion"
       "-Wsign-conversion"
       "-Wswitch-enum"
       "-Wold-style-cast"
       "-Wdecls-in-multiple-modules"
       )

  if (TACTILE_BUILD_TYPE STREQUAL "asan")
    list(APPEND
         TACTILE_COMPILE_OPTIONS
         "-fsanitize=address"
         "-fno-sanitize-recover"
         "-fno-omit-frame-pointer"
         )
    list(APPEND
         TACTILE_LINK_OPTIONS
         "-fsanitize=address"
         "-fno-sanitize-recover"
         "-fno-omit-frame-pointer"
         )
  endif ()
endif ()

message(DEBUG "TACTILE_COMPILE_OPTIONS: ${TACTILE_COMPILE_OPTIONS}")
message(DEBUG "TACTILE_LINK_OPTIONS: ${TACTILE_LINK_OPTIONS}")

add_library(tactile_interface_target INTERFACE)

target_compile_features(tactile_interface_target INTERFACE cxx_std_23)

target_compile_options(tactile_interface_target INTERFACE "${TACTILE_COMPILE_OPTIONS}")

target_link_options(tactile_interface_target INTERFACE "${TACTILE_LINK_OPTIONS}")

if (TACTILE_BUILD_TYPE STREQUAL "asan" AND NOT MSVC)
  target_link_libraries(tactile_interface_target INTERFACE "-fsanitize=address")
endif ()

if (WIN32)
  target_compile_definitions(tactile_interface_target
                             INTERFACE
                             "WIN32_LEAN_AND_MEAN"
                             "NOMINMAX"
                             )
endif ()
