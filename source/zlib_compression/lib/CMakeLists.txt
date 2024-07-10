project(tactile-zlib-compression-lib CXX)

add_library(tactile-zlib-compression SHARED)
add_library(tactile::zlib_compression ALIAS tactile-zlib-compression)

target_sources(tactile-zlib-compression
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/zlib_compression/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/zlib_compression/zlib_compression_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/zlib_compression/zlib_compression_plugin.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/zlib_compression/zlib_compressor.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/zlib_compression/zlib_compressor.cpp"
               )

tactile_prepare_target(tactile-zlib-compression)

target_include_directories(tactile-zlib-compression PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_definitions(tactile-zlib-compression PRIVATE "TACTILE_BUILDING_ZLIB_COMPRESSION")

target_link_libraries(tactile-zlib-compression
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      ZLIB::ZLIB
                      )
