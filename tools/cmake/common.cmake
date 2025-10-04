# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

if (MSVC)
  list(APPEND TACTILE_PRIVATE_COMPILE_DEFINITIONS
       "WIN32_LEAN_AND_MEAN"
       "NOMINMAX"
       )

  list(APPEND TACTILE_PUBLIC_COMPILE_OPTIONS
       "/EHsc"
       "/utf-8"
       "/Zc:__cplusplus"
       "/Zc:preprocessor"
       )

  list(APPEND TACTILE_PRIVATE_COMPILE_OPTIONS
       "/bigobj"
       "/MP"
       "/W4"
       )
else ()
  list(APPEND TACTILE_PRIVATE_COMPILE_OPTIONS
       "-Wall"
       "-Wextra"
       "-Wpedantic"
       "-Wconversion"
       "-Wsign-conversion"
       "-Wold-style-cast"
       "-Wswitch-enum"
       )

  if (TACTILE_ENABLE_UBSAN)
    list(APPEND TACTILE_PUBLIC_COMPILE_OPTIONS
         "-fsanitize=undefined"
         "-fno-sanitize-recover"
         "-fno-omit-frame-pointer"
         )

    list(APPEND TACTILE_PUBLIC_LINK_OPTIONS
         "-fsanitize=undefined"
         "-fno-sanitize-recover"
         "-fno-omit-frame-pointer"
         )
  endif ()
endif ()

message(DEBUG "TACTILE_PUBLIC_COMPILE_DEFINITIONS: [${TACTILE_PUBLIC_COMPILE_DEFINITIONS}]")
message(DEBUG "TACTILE_PRIVATE_COMPILE_DEFINITIONS: [${TACTILE_PRIVATE_COMPILE_DEFINITIONS}]")

message(DEBUG "TACTILE_PUBLIC_COMPILE_OPTIONS: [${TACTILE_PUBLIC_COMPILE_OPTIONS}]")
message(DEBUG "TACTILE_PRIVATE_COMPILE_OPTIONS: [${TACTILE_PRIVATE_COMPILE_OPTIONS}]")

message(DEBUG "TACTILE_PUBLIC_LINK_OPTIONS: [${TACTILE_PUBLIC_LINK_OPTIONS}]")
message(DEBUG "TACTILE_PRIVATE_LINK_OPTIONS: [${TACTILE_PRIVATE_LINK_OPTIONS}]")

function(tactile_init_target target)
  set_target_properties("${target}"
                        PROPERTIES
                        PREFIX ""
                        POSITION_INDEPENDENT_CODE "ON"
                        )

  target_compile_features("${target}" PUBLIC "cxx_std_23")

  target_compile_definitions("${target}"
                             PUBLIC "${TACTILE_PUBLIC_COMPILE_DEFINITIONS}"
                             PRIVATE "${TACTILE_PRIVATE_COMPILE_DEFINITIONS}"
                             )

  target_compile_options("${target}"
                         PUBLIC "${TACTILE_PUBLIC_COMPILE_OPTIONS}"
                         PRIVATE "${TACTILE_PRIVATE_COMPILE_OPTIONS}"
                         )

  target_link_options("${target}"
                      PUBLIC "${TACTILE_PUBLIC_LINK_OPTIONS}"
                      PRIVATE "${TACTILE_PRIVATE_LINK_OPTIONS}"
                      )
endfunction()
