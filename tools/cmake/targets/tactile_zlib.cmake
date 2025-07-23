# Copyright (C) 2025 Albin Johansson

find_package(ZLIB REQUIRED)

add_library(tactile_zlib STATIC)

tactile_set_target_properties(tactile_zlib)

target_sources(tactile_zlib
               PUBLIC FILE_SET CXX_MODULES FILES
               "${TACTILE_MODULE_DIR}/tactile/zlib/zlib.cppm"

               PRIVATE
               "${TACTILE_SOURCE_DIR}/tactile/zlib/zlib.cpp"
               )

target_link_libraries(tactile_zlib
                      PUBLIC
                      tactile_interface_target
                      tactile_core

                      PRIVATE
                      ZLIB::ZLIB
                      )
