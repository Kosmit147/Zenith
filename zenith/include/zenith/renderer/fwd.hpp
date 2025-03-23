#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth {

class PerspectiveCamera;

class ImGuiRenderer;

enum class LightType : u8;
struct LightProperties;
struct LightAttenuation;
struct DirectionalLight;
struct PointLight;
struct SpotLight;
struct AmbientLight;
struct DirectionalLightRenderData;
struct PointLightRenderData;
struct SpotLightRenderData;
struct AmbientLightRenderData;

struct Material;

namespace materials {

struct MaterialList;

} // namespace materials

class Mesh;

namespace meshes {

struct MeshList;

} // namespace meshes

struct DrawCommand;
struct RenderBatch;
class Renderer;

namespace shaders {

struct ShaderList;

} // namespace shaders

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
