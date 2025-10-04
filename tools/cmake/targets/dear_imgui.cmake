# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

set(TACTILE_DEAR_IMGUI_DIR "${ext-dear_imgui_SOURCE_DIR}")
message(DEBUG "TACTILE_DEAR_IMGUI_DIR: ${TACTILE_DEAR_IMGUI_DIR}")

add_library(dear_imgui STATIC)

set_target_properties(dear_imgui
                      PROPERTIES
                      PREFIX ""
                      POSITION_INDEPENDENT_CODE ON
                      )

target_compile_features(dear_imgui PUBLIC "cxx_std_23")

target_include_directories(dear_imgui PUBLIC "${TACTILE_DEAR_IMGUI_DIR}/backends")

target_link_libraries(dear_imgui
                      PUBLIC
                      SDL3::SDL3
                      )

target_sources(dear_imgui
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "${TACTILE_DEAR_IMGUI_DIR}" FILES
               "${TACTILE_DEAR_IMGUI_DIR}/backends/imgui_impl_sdl3.h"
               "${TACTILE_DEAR_IMGUI_DIR}/backends/imgui_impl_sdlgpu3.h"
               "${TACTILE_DEAR_IMGUI_DIR}/backends/imgui_impl_sdlgpu3_shaders.h"
               "${TACTILE_DEAR_IMGUI_DIR}/imconfig.h"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui.h"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui_internal.h"
               "${TACTILE_DEAR_IMGUI_DIR}/imstb_rectpack.h"
               "${TACTILE_DEAR_IMGUI_DIR}/imstb_textedit.h"
               "${TACTILE_DEAR_IMGUI_DIR}/imstb_truetype.h"

               PRIVATE
               "${TACTILE_DEAR_IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
               "${TACTILE_DEAR_IMGUI_DIR}/backends/imgui_impl_sdlgpu3.cpp"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui.cpp"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui_demo.cpp"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui_draw.cpp"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui_tables.cpp"
               "${TACTILE_DEAR_IMGUI_DIR}/imgui_widgets.cpp"
               )
