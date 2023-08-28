project(tactile-core-src)

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_CORE_DIR}/inc/**/*.hpp
     ${TACTILE_CORE_DIR}/src/**/*.hpp
     ${TACTILE_CORE_DIR}/src/**/*.cpp
     )

add_library(${TACTILE_CORE_TARGET} SHARED ${TACTILE_CORE_SOURCE_FILES})

tactile_prepare_target(${TACTILE_CORE_TARGET})

target_compile_definitions(${TACTILE_CORE_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_CORE
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           SPDLOG_FMT_EXTERNAL
                           )

target_include_directories(${TACTILE_CORE_TARGET} PUBLIC "${TACTILE_CORE_DIR}/inc")

target_link_libraries(${TACTILE_CORE_TARGET}
                      PUBLIC
                      spdlog::spdlog  # Public due to profiling macros

                      PRIVATE
                      Boost::boost
                      FastFloat::fast_float
                      fmt::fmt
                      )