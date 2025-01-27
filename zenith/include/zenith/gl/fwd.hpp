#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth::gl {

enum class BufferState : u8;
class Buffer;
class VertexBuffer;
class IndexBuffer;
class UniformBuffer;
class InstanceBuffer;

enum class BufferAccessFrequency : u8;
enum class BufferAccessType : u8;
struct BufferUsage;

struct Version;
enum class Profile : u8;

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

} // namespace zth::gl
