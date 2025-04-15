#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth {

class ImGuiRenderer;

enum class LightType : u8;
struct LightProperties;
struct LightAttenuation;
struct DirectionalLight;
struct PointLight;
struct SpotLight;
struct AmbientLight;

struct Material;

class Mesh;

struct DrawCommand;
struct RenderBatch;
struct DirectionalLightRenderData;
struct PointLightRenderData;
struct SpotLightRenderData;
struct AmbientLightRenderData;
class Renderer;

struct LightPropertiesShaderData;
struct LightAttenuationShaderData;
struct CameraUboData;
struct DirectionalLightShaderData;
struct DirectionalLightsSsboData;
struct PointLightShaderData;
struct PointLightsSsboData;
struct SpotLightShaderData;
struct SpotLightsSsboData;
struct AmbientLightShaderData;
struct AmbientLightsSsboData;
struct MaterialUboData;

struct LineInfo;
struct PreprocessShaderError;
class ShaderPreprocessor;

struct InstanceVertex;
struct StandardVertex;

} // namespace zth
