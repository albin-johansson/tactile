project(tactile_zlib_project CXX)

add_library(tactile_zlib STATIC)

target_sources(tactile_zlib
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "zlib.cppm"

               PRIVATE
               "zlib.cpp"
               )

tactile_set_target_properties(tactile_zlib)

target_link_libraries(tactile_zlib
                      PUBLIC
                      tactile_interface_target
                      tactile_core

                      PRIVATE
                      ZLIB::ZLIB
                      )
