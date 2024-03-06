project(tactile-core CXX)

file(GLOB_RECURSE TACTILE_CORE_SOURCE_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(tactile-core STATIC "${TACTILE_CORE_SOURCE_FILES}")
add_library(tactile::core ALIAS tactile-core)

tactile_prepare_target(tactile-core)

target_include_directories(tactile-core PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_definitions(tactile-core
                           PRIVATE
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           )

target_link_libraries(tactile-core
                      PUBLIC
                      tactile::base
                      tactile::render
                      fmt::fmt

                      PRIVATE
                      Boost::boost
                      )
