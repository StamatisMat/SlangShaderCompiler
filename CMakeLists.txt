cmake_minimum_required(VERSION 3.20)
project(SlangShaderCompiler CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SLANG_VERSION "2025.19.1")

include(FetchContent)

if(WIN32)
    set(SLANG_ASSET_NAME "slang-${SLANG_VERSION}-windows-x86_64.zip")
elseif(APPLE)
    set(SLANG_ASSET_NAME "slang-${SLANG_VERSION}-macos-aarch64.zip") # Verify exact name on GitHub
else()
    set(SLANG_ASSET_NAME "slang-${SLANG_VERSION}-linux-x86_64.zip")
endif()

set(SLANG_URL "https://github.com/shader-slang/slang/releases/download/v${SLANG_VERSION}/${SLANG_ASSET_NAME}")

message(STATUS "Downloading Slang ${SLANG_VERSION} from ${SLANG_URL}...")

FetchContent_Declare(
    slang_binaries
    URL ${SLANG_URL}
)
FetchContent_MakeAvailable(slang_binaries)

add_library(slang::slang SHARED IMPORTED GLOBAL)

set(SLANG_ROOT "${slang_binaries_SOURCE_DIR}")

target_include_directories(slang::slang INTERFACE "${SLANG_ROOT}/include")

if(WIN32)
    set_target_properties(slang::slang PROPERTIES
        IMPORTED_IMPLIB "${SLANG_ROOT}/lib/slang.lib"
        IMPORTED_LOCATION "${SLANG_ROOT}/bin/slang.dll"
    )
else()
    set_target_properties(slang::slang PROPERTIES
        IMPORTED_LOCATION "${SLANG_ROOT}/lib/libslang.so" # Check if it's .so or .a in the zip
    )
endif()

# SOURCE COLLECTION
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.slang" # Just to appear on the ide
)

add_executable(SlangCompiler ${SRC_FILES})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT SlangCompiler)

target_link_libraries(SlangCompiler PRIVATE slang::slang)

if(WIN32)
    set_target_properties(SlangCompiler PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SlangCompiler>")
    add_custom_command(TARGET SlangCompiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SLANG_ROOT}/bin/slang.dll"
        "${SLANG_ROOT}/bin/slang-glslang.dll" # Required for GLSL
        $<TARGET_FILE_DIR:SlangCompiler>
        COMMENT "Copying Slang DLLs to output directory..."
    )
endif()

# ALWAYS /permissive- on MSVC
if (MSVC)
    target_compile_options(SlangCompiler PRIVATE /W4 /permissive-)
endif()

add_custom_command(TARGET SlangCompiler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:SlangCompiler>/shaders
    COMMENT "Copying slang shaders to output directory"
)