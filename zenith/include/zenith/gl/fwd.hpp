#pragma once

#include "zenith/core/typedefs.hpp"

namespace zth::gl {

enum class BufferAccessFrequency : u8;
enum class BufferAccessType : u8;
struct BufferUsage;
enum class BufferState : u8;
class Buffer;
class VertexBuffer;
class IndexBuffer;
class InstanceBuffer;
class UniformBuffer;
class ShaderStorageBuffer;

struct Version;
enum class Profile : u8;
class Context;

enum class ShaderType : u16;
struct ShaderSources;
struct ShaderSourcePaths;
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
