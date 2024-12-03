#pragma once

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

enum class AccessFrequency : u8;
enum class AccessType : u8;
struct BufferUsage;

class GlBuffer;
class VertexBuffer;
class IndexBuffer;

struct GlVersion;
enum class GlProfile : u8;

enum class ShaderType : u8;
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

class VertexArray;

enum class VertexLayoutElement : u8;
struct VertexLayoutElementInfo;
class VertexLayout;

} // namespace zth
