# SLang Shader Compiler

SLang Shader Compiler is a simple C++ project that utilizes the SLang C++ API to compile Slang Graphics shaders to HLSL, Vulkan GLSL and SpirV on runtime. main.cpp is an example/testing suite that uses the SlangCompiler class.

## Disclaimer
This project was built using C++ 20, slang 2025.19.1, CMake 3.20 and MSVC 19.50.35718. Tested using Visual Studio 2026 

## Installation

1. clone this repository
2. run cmake -B "add your target build directory here" (I run cmake -B build because my target directory is named "build")
3. Depending on your system, you might get a Visual Studio solution or an executable file.
4. profit

## Installation for CMakeListsLight, for lower disk usage.

Get slang api from https://github.com/shader-slang/slang/releases/tag/v2025.19.1

1. Dependencies/include/slang/
get whole slang/include directory (contains slang.h, slang-com-ptr.h etc.) and drop all the header files there

2. Dependencies/lib/slang/
get slang.lib from slang/lib

3. Dependencies/bin/slang/
get gfx.dll, gfx.slang, slang.dll, slang-slang, slang-glslang.dll, slang-glsl-module.dll from slang/bin

4. Rename CMakeListsLight.txt to CMakeLists.txt

5. run cmake -B build

6. Depending on your system, you might get a Visual Studio solution or an executable file.

## Usage

SlangCompiler [options]

An example slang file is given, you can modify it to suit your needs.
for custom usage there are options that can be used to ignore hardcoded testing and to use another file.
Options:
[path] shorthand for -file [path]
-string compiles the provided test string
-file [path] compiles the slang file at [path]
-entry [entry points seperated by commas] (default: vertexMain, fragmentMain)
-h or -help prints usage

The program will compile SLang code and print GLSL and SpirV statistics in console, or provide diagnostics if it can't.
Main is intended as a platform for SlangCompiler class, to provide examples of Slang Compilation.

## TODO

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Feel free to test new SLang versions

Please make sure to update tests as appropriate.

## License

MIT License

Copyright (c) 2025 Stamatis Matziounis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.