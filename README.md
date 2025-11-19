# SLang Shader Compiler

SLang Shader Compiler is a simple C++ project that utilizes the SLang C++ API to compile Slang Graphics shaders to HLSL, Vulkan GLSL and SpirV on runtime.

## Disclaimer
This project was built using slang 2025.19.1 and CMake 3.20

## Installation

Get slang api from https://github.com/shader-slang/slang/releases/tag/v2025.19.1

1.
Dependencies/include/slang/
get whole slang/include directory (contains slang.h, slang-com-ptr.h etc.) and drop all the header files there

Dependencies/lib/slang/
get slang.lib from slang/lib

Dependencies/bin/slang/
get gfx.dll, gfx.slang, slang.dll, slang-slang, slang-glslang.dll, slang-glsl-module.dll from slang/bin

2. run cmake -B

## Usage

An example slang file is given, you can modify it to suit your needs.
You can modify main.cpp to use any other filename.

The program will compile SLang code and print GLSL or HLSL in console, or provide diagnostics if it can't.

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