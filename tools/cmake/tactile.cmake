
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
       "/wd4251"
       "/wd4275"
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
       )

  if (TACTILE_BUILD_TYPE STREQUAL "asan")
    if (NOT MSVC)
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
endif ()

message(DEBUG "TACTILE_COMPILE_OPTIONS: ${TACTILE_COMPILE_OPTIONS}")
message(DEBUG "TACTILE_LINK_OPTIONS: ${TACTILE_LINK_OPTIONS}")

function(tactile_set_output_directory target directory)
  set_target_properties("${target}"
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY "${directory}"
                        ARCHIVE_OUTPUT_DIRECTORY "${directory}"
                        LIBRARY_OUTPUT_DIRECTORY "${directory}"
                        )
endfunction()

function(tactile_prepare_target target)
  target_compile_features(${target} PUBLIC cxx_std_23)
  set_target_properties(${target}
                        PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        INTERPROCEDURAL_OPTIMIZATION ${TACTILE_USE_LTO}
                        PREFIX ""
                        )
  tactile_set_output_directory(${target} ${TACTILE_BUILD_DIR})
endfunction()

add_library(tactile_basic_target INTERFACE)

target_compile_options(tactile_basic_target INTERFACE ${TACTILE_COMPILE_OPTIONS})

target_link_options(tactile_basic_target INTERFACE ${TACTILE_LINK_OPTIONS})

if (TACTILE_BUILD_TYPE STREQUAL "asan" AND NOT MSVC)
  target_link_libraries(tactile_basic_target INTERFACE "-fsanitize=address")
endif ()

if (WIN32)
  target_compile_definitions(tactile_basic_target
                             INTERFACE
                             "WIN32_LEAN_AND_MEAN"
                             "NOMINMAX"
                             )
endif ()

if (APPLE)
  message(DEBUG "Applying workaround for CLion 'import std;' issue")

  # See https://youtrack.jetbrains.com/issue/CPP-39632/import-std-CLion-cant-resolve-module-std-in-case-of-clang
  add_library(tactile_import_std_clion_workaround STATIC)

  target_compile_features(tactile_import_std_clion_workaround PUBLIC cxx_std_23)

  target_sources(tactile_import_std_clion_workaround
                 PRIVATE FILE_SET "CXX_MODULES" BASE_DIRS "/opt/homebrew/opt/llvm/share/libc++/v1" FILES
                 "/opt/homebrew/opt/llvm/share/libc++/v1/std.cppm"
                 "/opt/homebrew/opt/llvm/share/libc++/v1/std.compat.cppm"
                 )
endif ()
