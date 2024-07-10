project(tactile-zstd-compression-lib CXX)

add_library(tactile-zstd-compression SHARED)
add_library(tactile::zstd_compression ALIAS tactile-zstd-compression)

target_sources(tactile-zstd-compression
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/zstd_compression/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/zstd_compression/zstd_compression_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/zstd_compression/zstd_compression_plugin.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/zstd_compression/zstd_compressor.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/zstd_compression/zstd_compressor.cpp"
               )

tactile_prepare_target(tactile-zstd-compression)

target_compile_definitions(tactile-zstd-compression PRIVATE "TACTILE_BUILDING_ZSTD_COMPRESSION")

target_include_directories(tactile-zstd-compression
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

target_link_libraries(tactile-zstd-compression
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      zstd::libzstd
                      )
