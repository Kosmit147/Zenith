#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth {

enum class GlBufferState : u8;
class GlBuffer;
class VertexBuffer;
class IndexBuffer;
class UniformBuffer;
class InstanceBuffer;

enum class GlBufferAccessFrequency : u8;
enum class GlBufferAccessType : u8;
struct GlBufferUsage;

struct GlVersion;
enum class GlProfile : u8;

enum class ShaderType : u16;
struct UniformInfo;
class Shader;

enum class TextureFormat : u16;
enum class SizedTextureFormat : u16;
enum class TextureWrapMode : u16;
enum class TextureFilteringMode : u8;
enum class MipmapFilteringMode : u8;
struct TextureMinFilter;
struct TextureMagFilter;
struct TextureParams;

class Texture2D;

struct VertexArrayLayout;
class VertexArray;

enum class VertexLayoutElement : u8;
struct VertexLayoutElementInfo;
class VertexLayout;

} // namespace zth
