project(tactile-zstd-compression-lib CXX)

add_library(tactile-zstd-compression SHARED)
add_library(tactile::zstd_compression ALIAS tactile-zstd-compression)

target_sources(tactile-zstd-compression
               PRIVATE
               "src/tactile/zstd_compression/zstd_compression_plugin.cpp"
               "src/tactile/zstd_compression/zstd_compressor.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/zstd_compression/api.hpp"
               "inc/tactile/zstd_compression/zstd_compression_plugin.hpp"
               "inc/tactile/zstd_compression/zstd_compressor.hpp"
               )

tactile_prepare_target(tactile-zstd-compression)

target_compile_definitions(tactile-zstd-compression
                           PRIVATE
                           "TACTILE_BUILDING_ZSTD_COMPRESSION"
                           )

target_link_libraries(tactile-zstd-compression
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      zstd::libzstd
                      )
