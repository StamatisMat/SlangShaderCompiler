cmake_minimum_required(VERSION 3.20)
include(CMakePrintHelpers)
project(ShaderCompiler LANGUAGES CXX)

# ---------- SETTINGS ----------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SLANG_VERSION "2025.19.1")
if(WIN32)
    set(SLANG_OS "windows-x86_64")
elseif(APPLE)
    set(SLANG_OS "macos-aarch64")
elseif(UNIX)
    set(SLANG_OS "linux-x86_64")
endif()

file(GLOB SLANG_FOLDERS true "${CMAKE_SOURCE_DIR}/Dependencies/*/slang")

find_library(slang PRIVATE EXCLUDE_FROM_ALL ${SLANG_FOLDERS})



# SOURCE COLLECTION
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.slang" # Just to appear on the ide
)
set(SLANG_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Dependencies/include/slang")

# Exclude Dependencies directory files
#list(FILTER SRC_FILES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/Dependencies/.*")
# EXECUTABLE
add_executable(ShaderCompiler ${SRC_FILES})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ShaderCompiler)
#cmake_print_variables(${SRC_FILES})

# INCLUDE PATHS
target_include_directories(ShaderCompiler PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Dependencies/src
    ${SLANG_SOURCE_DIR}  # Slang headers from its own directory
)

#  LINK LIBRARIES
file(GLOB LINK_DIRECTORIES true "${CMAKE_SOURCE_DIR}/Dependencies/lib/*")

# REMOVE ALL NON DIRECTORIES
foreach(item ${LINK_DIRECTORIES})
    if(NOT IS_DIRECTORY ${item})
        list(REMOVE_ITEM ${LINK_DIRECTORIES} ${item})
    endif()
endforeach()

target_link_directories(ShaderCompiler PRIVATE ${LINK_DIRECTORIES})

target_link_libraries(ShaderCompiler PRIVATE
    slang
)

# ALWAYS /permissive- on MSVC
if (MSVC)
    target_compile_options(ShaderCompiler PRIVATE /W4 /permissive-)
endif()

# Copy Slang DLL 
if(WIN32)
    set_target_properties(ShaderCompiler PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:ShaderCompiler>")
    add_custom_command(TARGET ShaderCompiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/Dependencies/bin/slang/slang.dll
        $<TARGET_FILE_DIR:ShaderCompiler>
        COMMENT "Copying Slang DLL to output directory"
    )
    add_custom_command(TARGET ShaderCompiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:ShaderCompiler>/shaders
        COMMENT "Copying slang file to output directory"
    )
endif()