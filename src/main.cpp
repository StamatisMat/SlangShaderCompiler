#include "ShaderCompiler.h"
#include <fstream>

void stringExample(SlangCompiler& compiler, const std::string& source, const std::string& path = "") {
        // Compile both entry points in a single pass
        std::vector<std::string> entryPoints = { "vertexMain", "fragmentMain" };

        // Get all shaders as GLSL
        auto glslShaders = compiler.compileToGLSL(source, entryPoints, path);
        std::cout << "Compiled " << glslShaders.size() << " GLSL shaders:\n";
        for (const auto& shader : glslShaders)
        {
            std::cout << "\n=== " << shader.entryPointName << " ===\n";
            std::cout << shader.asText() << "\n";
        }

        // Get all shaders as SPIR-V
        auto spirvShaders = compiler.compileToSPIRV(source, entryPoints, path);
        std::cout << "\nCompiled " << spirvShaders.size() << " SPIR-V shaders:\n";
        for (const auto& shader : spirvShaders)
        {
            std::cout << shader.entryPointName << ": " << shader.binaryData.size() << " bytes\n";
        }

        // Single entry point convenience method
        std::string singleGlsl = compiler.compileToGLSLSingle(source, "vertexMain", path);
        std::cout << "\nSingle vertex shader GLSL:\n" << singleGlsl << "\n";

}



int main()
{
	std::cout << "Slang Shader Compiler Examples:\n";
    try
    {
        SlangCompiler compiler;
        std::string source = R"(
            [shader("vertex")]
            float4 vertexMain(float3 pos : POSITION) : SV_Position
            {
                return float4(pos, 1.0);
            }
            
            [shader("fragment")]
            float4 fragmentMain(float4 pos : SV_Position) : SV_Target
            {
                return float4(1.0, 0.0, 0.0, 1.0);
            }
        )";
        std::cout << "Example 1: simple source";
        stringExample(compiler, source);
                
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    try
    {
        SlangCompiler compiler;
		std::string path = "shaders/obj_tex_shader.slang";
        std::ifstream stream(path, std::ios::in);
        if (!stream.is_open()) {
			throw std::runtime_error("Failed to open shader file. Check filename");
        }
        std::string source = std::string{(std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>()};
        stringExample(compiler, source, path);
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}