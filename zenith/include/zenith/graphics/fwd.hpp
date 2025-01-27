#pragma once

namespace zth {

class PerspectiveCamera;

class ImGuiRenderer;

struct LightProperties;
struct LightAttenuation;
struct DirectionalLight;
struct PointLight;
struct SpotLight;
struct AmbientLight;

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

struct LineInfo;
struct PreprocessShaderError;
class ShaderPreprocessor;

class Shape3D;
class CubeShape;
class PyramidShape;
class SphereShape;

struct CameraUboData;
struct LightUboData;
struct MaterialUboData;

struct InstanceVertex;
struct StandardVertex;

} // namespace zth
