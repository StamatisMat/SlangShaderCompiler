#include "ShaderCompiler.h"
#include <fstream>
#include <sstream>

void printUsage(const char* programName) {
    std::cout << "Slang Shader Compiler Test Harness\n";
    std::cout << "Usage:\n";
    std::cout << "  " << programName << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -string                      Run hardcoded string example\n";
    std::cout << "  -file <path>                 Run file example (default: shaders/obj_tex_shader.slang)\n";
    std::cout << "  -entry <name1,name2,...>     Specify entry points (default: vertexMain,fragmentMain)\n";
    std::cout << "  <path>                       Quick file test (shorthand for -file <path>)\n";
    std::cout << "  (no args)                    Run both examples with defaults\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " -file shaders/test.slang -entry myVertex,myFragment\n";
    std::cout << "  " << programName << " shaders/test.slang -entry computeMain\n";
}

void stringExample(SlangCompiler& compiler, const std::string& source, const std::vector<std::string>& entryPoints, const std::string& path = "") {
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

void fileExample(SlangCompiler& compiler, const std::vector<std::string>& entryPoints, const std::string& path) {
    std::ifstream stream(path, std::ios::in);
    if (!stream.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + path);
    }
    std::string source = std::string{(std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>()};
    stringExample(compiler, source, entryPoints, path);
}

void TestShaderReflection(SlangCompiler& compiler, const std::string& source, const std::vector<std::string>& entryPoints, const std::string& path = "") 
{
    std::vector<ShaderOutput> shaderOutputs = compiler.compileToHLSL(source, entryPoints, path);
    ShaderOutput shaderOutput{ shaderOutputs.at(0) };

    // 2. Print extracted bindings
    std::cout << "=== Texture Shader Reflection ===" << std::endl;
    for (const auto& binding : shaderOutput.resourceBindings)
    {
        std::cout << "Name: " << binding.name
            << ", Type: " << (int)binding.resourceType
            << ", Binding: " << binding.binding
            << ", Space: " << binding.set
            << ", Count: " << binding.count << std::endl;
    }

    // 3. Verify expected bindings
    assert(shaderOutput.resourceBindings.size() == 6); // b0, b1, b2, t0, s0

    // Find each binding and verify
    auto findBinding = [&](const std::string& name) {
        auto it = std::find_if(shaderOutput.resourceBindings.begin(),
            shaderOutput.resourceBindings.end(),
            [&](const auto& b) { return b.name == name; });
        assert(it != shaderOutput.resourceBindings.end());
        return *it;
        };

    auto globals = findBinding("Globals");
    assert(globals.resourceType == ShaderResourceBinding::ResourceType::ConstantBuffer);
    assert(globals.binding == 0 && globals.set == 0);

    auto colors = findBinding("FragmentColors");
    assert(colors.resourceType == ShaderResourceBinding::ResourceType::ConstantBuffer);
    assert(colors.binding == 1 && colors.set == 0);

    auto camera = findBinding("FragmentCameraPos");
    assert(camera.resourceType == ShaderResourceBinding::ResourceType::ConstantBuffer);
    assert(camera.binding == 2 && camera.set == 0);

    auto texture = findBinding("TextureSampler");
    assert(texture.resourceType == ShaderResourceBinding::ResourceType::Texture);
    assert(texture.binding == 0 && texture.set == 0);

    auto sampler = findBinding("TextureSampler_sampler");
    assert(sampler.resourceType == ShaderResourceBinding::ResourceType::Sampler);
    assert(sampler.binding == 0 && sampler.set == 0);

    std::cout << "All texture shader reflection tests passed!" << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout << "Slang Shader Compiler Example Tests:\n";
    std::string testFilePath = "shaders/obj_tex_shader.slang";
    std::vector<std::string> entryPoints = { "vertexMain", "fragmentMain" };
    uint16_t examplesFailed = 0;

    bool runStringTest = false, runFileTest = false;
    if (argc == 1) {
        // No arguments: run both tests
        runStringTest = true;
        runFileTest = true;
    } else {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "-string") {
                runStringTest = true;
            } 
            else if (arg == "-entry") {
                if (i + 1 < argc) {
                    // Parse comma-separated entry points
                    std::string entryStr = argv[++i];
                    entryPoints.clear();
                    std::stringstream ss(entryStr);
                    std::string entry;
                    while (std::getline(ss, entry, ',')) {
                        if (!entry.empty()) {
                            entryPoints.push_back(entry);
                        }
                    }
                    if (entryPoints.empty()) {
                        std::cerr << "Error: No valid entry points provided after -entry\n";
                        return 1;
                    }
                } else {
                    std::cerr << "Error: -entry requires entry point names\n";
                    return 1;
                }
            }
            else if (arg == "-file") {
                runFileTest = true;
                if (i + 1 < argc && argv[i + 1][0] != '-') {
                    testFilePath = argv[++i];
                }
            }
            else if (arg == "-h" || arg == "--help") {
                printUsage(argv[0]);
                return 0;
            }
            else {
                std::cerr << "Unknown argument: " << arg << "\n";
                printUsage(argv[0]);
                return 1;
            }
        }
    }
    SlangCompiler compiler;
    if (runStringTest) {
        try
        {
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
            std::cout << "Example 1: simple string source";
            stringExample(compiler, source, {"vertexMain", "fragmentMain"});
                    
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            ++examplesFailed;
        }
    }
    if (runFileTest) {
        try
        {
            std::ifstream stream(testFilePath, std::ios::in);
            if (!stream.is_open()) {
                throw std::runtime_error("Failed to open shader file. Check filename");
            }
            std::string source = std::string{(std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>()};
            stringExample(compiler, source, entryPoints, testFilePath);
        } 
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            ++examplesFailed;
        }
        try
        {
			std::ifstream stream(testFilePath, std::ios::in);
			if (!stream.is_open()) {
				throw std::runtime_error("Failed to open shader file. Check filename");
			}
			std::string source = std::string{(std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>()};
            TestShaderReflection(compiler, source, entryPoints, testFilePath);

        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            ++examplesFailed;
        }
    }
    std::cout << "Summary: " << examplesFailed << " example(s) failed.\n";
    if (examplesFailed > 0) return 1;
    return 0;
}

