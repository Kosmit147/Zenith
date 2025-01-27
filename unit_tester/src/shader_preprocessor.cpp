#include <string_view>

#include <zenith/platform/shader_preprocessor.hpp>

#include "matchers.hpp"

TEST_CASE("ShaderPreprocessor", "[ShaderPreprocessor]")
{
    using namespace std::string_view_literals;

    // clang-format off

    constexpr auto source1 = 
R"(// This is from source1)"sv;

    constexpr auto source2 = 
R"(// This is from source2
// source2 includes source3
   #include<source3.glsl>)"sv;

    constexpr auto source3 = 
R"(// This is from source3)"sv;

    constexpr auto input = 
R"(// This is a comment

#version 460 core

#include "source1.glsl"

void someFunc() {}

#include <source2.glsl>

void main() {})"sv;

    constexpr auto expected_output = 
R"(// This is a comment

#version 460 core

// This is from source1

void someFunc() {}

// This is from source2
// source2 includes source3
   // This is from source3

void main() {})"sv;

    // clang-format on

    zth::ShaderPreprocessor::add_source("source1.glsl", source1);
    zth::ShaderPreprocessor::add_source("source2.glsl", source2);
    zth::ShaderPreprocessor::add_source("source3.glsl", source3);

    auto output = zth::ShaderPreprocessor::preprocess(input);

    REQUIRE(output.has_value());
    REQUIRE_THAT(*output, Equals(expected_output));
}
