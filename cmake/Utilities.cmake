# Copies a directory.
#   target: the associated target.
#   from: the directory that will be copied.
#   to: the target destination of the copied directory.
function(copy_directory_post_build target from to)
    add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${from}
            ${to})
endfunction()

# Creates an interface library target for a header-only library.
#   name: the name of the library target.
#   includeDirectory: the path of the directory that contains the headers of the library.
#   sources: the headers associated with the library.
function(add_header_only_lib name includeDirectory sources)
    add_library(${name} INTERFACE)
    target_sources(${name} INTERFACE ${sources})
    target_include_directories(${name} SYSTEM INTERFACE ${includeDirectory})
endfunction()

# Checks if an environment variable is defined.
#   name: the name of the actual environment variable.
function(find_env_var name)
    if (DEFINED ENV{${name}})
        message("${name} = \"$ENV{${name}}\"")
    else ()
        message(WARNING "Couldn't find environment variable ${name}!")
    endif ()
endfunction()