#pragma once

#include <glad/glad.h>

#include <ranges>
#include <span>
#include <type_traits>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/gl/vertex_layout.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"

namespace zth::gl {

enum class BufferAccessFrequency : u8
{
    // @volatile: These numbers are chosen for converting BufferUsage to an OpenGL value.

    Stream = 0,  // The data store contents will be modified once and used at most a few times.
    Static = 4,  // The data store contents will be modified once and used many times.
    Dynamic = 8, // The data store contents will be modified repeatedly and used many times.
};

enum class BufferAccessType : u8
{
    // @volatile: These numbers are chosen for converting BufferUsage to an OpenGL value.

    Draw = 0, // The data store contents are modified by the application, and used as the source for GL drawing and
              // image specification commands. The user will be writing data to the buffer, but will not read it.
    Read = 1, // The data store contents are modified by reading data from the GL, and used to return that data when
              // queried by the application. The user will not be writing data to the buffer, but will be reading it
              // back.
    Copy = 2, // The data store contents are modified by reading data from the GL, and used as the source for GL drawing
              // and image specification commands. The user will be neither writing not reading the data.
};

struct BufferUsage
{
    BufferAccessFrequency access_frequency;
    BufferAccessType access_type;

    static constinit const BufferUsage stream_draw;
    static constinit const BufferUsage static_draw;
    static constinit const BufferUsage dynamic_draw;

    static constinit const BufferUsage stream_read;
    static constinit const BufferUsage static_read;
    static constinit const BufferUsage dynamic_read;

    static constinit const BufferUsage stream_copy;
    static constinit const BufferUsage static_copy;
    static constinit const BufferUsage dynamic_copy;
};

enum class BufferState : u8
{
    Uninitialized,
    InitializedStatic,  // Buffer cannot be reinitialized, size cannot be changed and cannot be 0.
    InitializedDynamic, // Buffer can be reinitialized, size can be changed.
};

// --------------------------- Buffer ---------------------------

class Buffer
{
public:
    using BufferId = GLuint;

public:
    explicit Buffer();

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> Buffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data) -> Buffer;

    [[nodiscard]] static auto create_static_with_data(auto&& data) -> Buffer;

    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> Buffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;

    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> Buffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::span<const byte> data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;

    [[nodiscard]] static auto create_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw)
        -> Buffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;

    Buffer(const Buffer& other);
    auto operator=(const Buffer& other) -> Buffer&;

    Buffer(Buffer&& other) noexcept;
    auto operator=(Buffer&& other) noexcept -> Buffer&;

    ~Buffer();

    auto init_static_with_size(u32 size_bytes) -> void;

    auto init_static_with_data(std::span<const byte> data) -> void;

    auto init_static_with_data(auto&& data) -> void;

    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::span<const byte> data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    // Returns the number of bytes written.
    auto buffer_data(std::span<const byte> data, u32 offset = 0) -> u32;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, u32 offset = 0) -> u32;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto resize(u32 size_bytes) -> void;
    auto resize_to_at_least(u32 min_size_bytes) -> void;
    auto reserve(u32 min_size_bytes) -> void;
    auto shrink_to_fit() -> void;
    auto free() noexcept -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto capacity_bytes() const { return _capacity_bytes; }
    [[nodiscard]] auto is_static() const { return _state == BufferState::InitializedStatic; }
    [[nodiscard]] auto is_dynamic() const { return _state == BufferState::InitializedDynamic; }
    [[nodiscard]] auto is_initialized() const { return _state != BufferState::Uninitialized; }
    [[nodiscard]] auto state() const { return _state; }

private:
    BufferId _id = GL_NONE;
    u32 _size_bytes = 0;
    u32 _capacity_bytes = 0;
    BufferState _state = BufferState::Uninitialized;
    Optional<BufferUsage> _usage = nil;

private:
    auto create() noexcept -> void;
    auto destroy() noexcept -> void;

    auto copy_initialize(const Buffer& other) -> void;

    auto buffer_data_static(std::span<const byte> data, u32 offset) -> u32;
    auto buffer_data_dynamic(std::span<const byte> data, u32 offset) -> u32;

    auto reallocate_exactly(u32 new_capacity_bytes) -> void;
    auto reallocate_at_least(u32 min_capacity_bytes) -> void;

    [[nodiscard]] static auto calculate_growth(u32 old_size_bytes) -> u32;
};

// --------------------------- VertexBuffer ---------------------------

template<typename T>
concept VertexRange = std::ranges::contiguous_range<T>;

class VertexBuffer
{
public:
    explicit VertexBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, const VertexLayout& layout) -> VertexBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data, const VertexLayout& layout)
        -> VertexBuffer;

    template<VertexRange V>
    [[nodiscard]] static auto create_static_with_data(V&& vertices,
                                                      const VertexLayout& layout = derive_vertex_layout<V>())
        -> VertexBuffer;

    [[nodiscard]] static auto create_dynamic(const VertexLayout& layout, BufferUsage usage = BufferUsage::dynamic_draw)
        -> VertexBuffer;

    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, const VertexLayout& layout,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::span<const byte> data, const VertexLayout& layout,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;

    template<VertexRange V>
    [[nodiscard]] static auto create_dynamic_with_data(V&& vertices, BufferUsage usage = BufferUsage::dynamic_draw,
                                                       const VertexLayout& layout = derive_vertex_layout<V>())
        -> VertexBuffer;

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(VertexBuffer)

    ~VertexBuffer() = default;

    auto init_static_with_size(u32 size_bytes, const VertexLayout& layout) -> void;

    auto init_static_with_data(std::span<const byte> data, const VertexLayout& layout) -> void;

    template<VertexRange V>
    auto init_static_with_data(V&& vertices, const VertexLayout& layout = derive_vertex_layout<V>()) -> void;

    auto init_dynamic(const VertexLayout& layout, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_size(u32 size_bytes, const VertexLayout& layout,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::span<const byte> data, const VertexLayout& layout,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    template<VertexRange V>
    auto init_dynamic_with_data(V&& vertices, BufferUsage usage = BufferUsage::dynamic_draw,
                                const VertexLayout& layout = derive_vertex_layout<V>()) -> void;

    auto buffer_data(std::span<const byte> data, u32 offset = 0) -> u32;
    auto buffer_data(VertexRange auto&& vertices, u32 offset = 0) -> u32;

    auto resize(u32 size_bytes) -> void { _buffer.resize(size_bytes); }
    auto resize_to_at_least(u32 min_size_bytes) -> void { _buffer.resize_to_at_least(min_size_bytes); }
    auto reserve(u32 min_size_bytes) -> void { _buffer.reserve(min_size_bytes); }
    auto shrink_to_fit() -> void { _buffer.shrink_to_fit(); }
    auto free() noexcept -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_layout(const VertexLayout& layout) -> void;
    auto set_stride(u32 stride_bytes) -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto capacity_bytes() const { return _buffer.capacity_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto state() const { return _buffer.state(); }
    [[nodiscard]] auto layout() const -> auto& { return _layout; }
    [[nodiscard]] auto stride() const { return _layout.stride(); }
    [[nodiscard]] auto count() const -> u32;

    template<VertexRange V> [[nodiscard]] constexpr static auto derive_vertex_layout() -> VertexLayout;

private:
    Buffer _buffer;
    VertexLayout _layout;
};

// --------------------------- IndexBuffer ---------------------------

template<typename T>
concept IndexRange = std::ranges::contiguous_range<T> && IndexingType<std::ranges::range_value_t<T>>;

class IndexBuffer
{
public:
    explicit IndexBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, DataType type) -> IndexBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data, DataType type) -> IndexBuffer;

    template<IndexRange I>
    [[nodiscard]] static auto create_static_with_data(I&& indices, DataType type = derive_indexing_data_type<I>())
        -> IndexBuffer;

    [[nodiscard]] static auto create_dynamic(DataType type, BufferUsage usage = BufferUsage::dynamic_draw)
        -> IndexBuffer;

    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, DataType type,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::span<const byte> data, DataType type,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;

    template<IndexRange I>
    [[nodiscard]] static auto create_dynamic_with_data(I&& indices, BufferUsage usage = BufferUsage::dynamic_draw,
                                                       DataType type = derive_indexing_data_type<I>()) -> IndexBuffer;

    ZTH_DEFAULT_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() = default;

    auto init_static_with_size(u32 size_bytes, DataType type) -> void;

    auto init_static_with_data(std::span<const byte> data, DataType type) -> void;

    template<IndexRange I>
    auto init_static_with_data(I&& indices, DataType type = derive_indexing_data_type<I>()) -> void;

    auto init_dynamic(DataType type, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_size(u32 size_bytes, DataType type, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::span<const byte> data, DataType type,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    template<IndexRange I>
    auto init_dynamic_with_data(I&& indices, BufferUsage usage = BufferUsage::dynamic_draw,
                                DataType type = derive_indexing_data_type<I>()) -> void;

    auto buffer_data(std::span<const byte> data, u32 offset = 0) -> u32;
    auto buffer_data(IndexRange auto&& indices, u32 offset = 0) -> u32;

    auto resize(u32 size_bytes) -> void { _buffer.resize(size_bytes); }
    auto resize_to_at_least(u32 min_size_bytes) -> void { _buffer.resize_to_at_least(min_size_bytes); }
    auto reserve(u32 min_size_bytes) -> void { _buffer.reserve(min_size_bytes); }
    auto shrink_to_fit() -> void { _buffer.shrink_to_fit(); }
    auto free() noexcept -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

    template<IndexingType T> auto set_indexing_data_type() -> void;
    auto set_indexing_data_type(DataType type) -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto capacity_bytes() const { return _buffer.capacity_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto state() const { return _buffer.state(); }
    [[nodiscard]] auto indexing_data_type() const { return _indexing_data_type; }
    [[nodiscard]] auto count() const -> u32;

    template<IndexRange I> [[nodiscard]] constexpr static auto derive_indexing_data_type() -> DataType;

private:
    Buffer _buffer;
    DataType _indexing_data_type = DataType::None;
};

// --------------------------- InstanceBuffer ---------------------------

class InstanceBuffer : public VertexBuffer
{
public:
    explicit InstanceBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, const VertexLayout& layout) -> InstanceBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data, const VertexLayout& layout)
        -> InstanceBuffer;

    template<VertexRange V>
    [[nodiscard]] static auto create_static_with_data(V&& data, const VertexLayout& layout = derive_vertex_layout<V>())
        -> InstanceBuffer;

    [[nodiscard]] static auto create_dynamic(const VertexLayout& layout, BufferUsage usage = BufferUsage::dynamic_draw)
        -> InstanceBuffer;

    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, const VertexLayout& layout,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::span<const byte> data, const VertexLayout& layout,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;

    template<VertexRange V>
    [[nodiscard]] static auto create_dynamic_with_data(V&& data, BufferUsage usage = BufferUsage::dynamic_draw,
                                                       const VertexLayout& layout = derive_vertex_layout<V>())
        -> InstanceBuffer;

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(InstanceBuffer)

    ~InstanceBuffer() = default;
};

// --------------------------- UniformBuffer ---------------------------

class UniformBuffer
{
public:
    explicit UniformBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, u32 binding_point) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data, u32 binding_point) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_data(auto&& data) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_data(auto&& data, u32 binding_point) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> UniformBuffer;

    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point)
        -> UniformBuffer;

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(UniformBuffer)

    ~UniformBuffer() = default;

    auto init_static_with_size(u32 size_bytes) -> void;

    auto init_static_with_size(u32 size_bytes, u32 binding_point) -> void;

    auto init_static_with_data(std::span<const byte> data) -> void;

    auto init_static_with_data(std::span<const byte> data, u32 binding_point) -> void;

    auto init_static_with_data(auto&& data) -> void;

    auto init_static_with_data(auto&& data, u32 binding_point) -> void;

    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;

    auto init_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point) -> void;

    auto buffer_data(std::span<const byte> data, u32 offset = 0) -> u32;
    auto buffer_data(auto&& data, u32 offset = 0) -> u32;
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto free() noexcept -> void;

    auto bind() const -> void;
    auto bind(u32 binding_point) const -> void;
    static auto unbind() -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto capacity_bytes() const { return _buffer.capacity_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto state() const { return _buffer.state(); }

private:
    Buffer _buffer;
};

// --------------------------- ShaderStorageBuffer ---------------------------

class ShaderStorageBuffer
{
public:
    explicit ShaderStorageBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, u32 binding_point) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_data(std::span<const byte> data, u32 binding_point)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_data(auto&& data) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_data(auto&& data, u32 binding_point) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic(u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::span<const byte> data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::span<const byte> data, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(auto&& data, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    ZTH_DEFAULT_COPY_DEFAULT_MOVE(ShaderStorageBuffer)

    ~ShaderStorageBuffer() = default;

    auto init_static_with_size(u32 size_bytes) -> void;

    auto init_static_with_size(u32 size_bytes, u32 binding_point) -> void;

    auto init_static_with_data(std::span<const byte> data) -> void;

    auto init_static_with_data(std::span<const byte> data, u32 binding_point) -> void;

    auto init_static_with_data(auto&& data) -> void;

    auto init_static_with_data(auto&& data, u32 binding_point) -> void;

    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;

    auto init_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic(u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_size(u32 size_bytes, u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw)
        -> void;

    auto init_dynamic_with_data(std::span<const byte> data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::span<const byte> data, u32 binding_point,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(auto&& data, u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(std::span<const byte> data, u32 offset = 0) -> u32;
    auto buffer_data(auto&& data, u32 offset = 0) -> u32;
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto resize(u32 size_bytes) -> void { _buffer.resize(size_bytes); }
    auto resize_to_at_least(u32 min_size_bytes) -> void { _buffer.resize_to_at_least(min_size_bytes); }
    auto reserve(u32 min_size_bytes) -> void { _buffer.reserve(min_size_bytes); }
    auto shrink_to_fit() -> void { _buffer.shrink_to_fit(); }
    auto free() noexcept -> void;

    auto bind() const -> void;
    auto bind(u32 binding_point) const -> void;
    static auto unbind() -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto capacity_bytes() const { return _buffer.capacity_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto state() const { return _buffer.state(); }

private:
    Buffer _buffer;
};

// clang-format off

template<typename T> struct is_gl_buffer : std::false_type {};
template<typename T> constexpr auto is_gl_buffer_v = is_gl_buffer<T>::value;

template<> struct is_gl_buffer<Buffer> : std::true_type {};
template<> struct is_gl_buffer<const Buffer> : std::true_type {};
template<> struct is_gl_buffer<VertexBuffer> : std::true_type {};
template<> struct is_gl_buffer<const VertexBuffer> : std::true_type {};
template<> struct is_gl_buffer<IndexBuffer> : std::true_type {};
template<> struct is_gl_buffer<const IndexBuffer> : std::true_type {};
template<> struct is_gl_buffer<InstanceBuffer> : std::true_type {};
template<> struct is_gl_buffer<const InstanceBuffer> : std::true_type {};
template<> struct is_gl_buffer<UniformBuffer> : std::true_type {};
template<> struct is_gl_buffer<const UniformBuffer> : std::true_type {};
template<> struct is_gl_buffer<ShaderStorageBuffer> : std::true_type {};
template<> struct is_gl_buffer<const ShaderStorageBuffer> : std::true_type {};

// clang-format on

template<typename T>
concept GlBuffer = is_gl_buffer_v<T>;

// Returns the number of bytes copied.
template<GlBuffer DstBuffer, GlBuffer SrcBuffer>
    requires(!std::is_const_v<std::remove_reference_t<DstBuffer>>)
auto copy_buffer_data(DstBuffer& dst, SrcBuffer& src, u32 size_bytes, u32 dst_offset = 0, u32 src_offset = 0) -> u32;

// Returns the number of bytes copied. Resizes the destination buffer to at least dst_offset + size_bytes bytes.
// Destination buffer must be an initialized dynamic buffer.
template<GlBuffer DstBuffer, GlBuffer SrcBuffer>
    requires(!std::is_const_v<std::remove_reference_t<DstBuffer>>)
auto copy_buffer_data_to_dynamic_buffer(DstBuffer& dst, SrcBuffer& src, u32 size_bytes, u32 dst_offset = 0,
                                        u32 src_offset = 0) -> u32;

[[nodiscard]] auto to_gl_enum(BufferUsage buffer_usage) -> GLenum;

} // namespace zth::gl

#include "buffer.inl"
