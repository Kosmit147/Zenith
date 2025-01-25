#pragma once

namespace zth {

class CubeShape;
class PyramidShape;
class Shape3D;
class SphereShape;

class PerspectiveCamera;

struct DrawCommand;

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

struct RenderBatch;
class Renderer;

class Renderer2D;

namespace shaders {

struct ShaderList;

} // namespace shaders

struct CameraUboData;
struct LightUboData;
struct MaterialUboData;

struct InstanceVertex;
struct StandardVertex;

} // namespace zth
