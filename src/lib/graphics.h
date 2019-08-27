#pragma once

#include "utils.h"

namespace Graphics
{
    #define GRAPHICS_MAX_VERTEX_ATTRIBS 16
    #define GRAPHICS_PIPELINE_MAX_BUFFERS 16
    #define GRAPHICS_PIPELINE_MAX_TEXTURES 16

    #define STRONGLY_TYPED_WEAKREF(name) struct name {Utils::WeakRef handle;}

    enum DataType
    {
        BYTE,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        INT,
        UNSIGNED_INT,
        HALF_FLOAT,
        FLOAT,
        DOUBLE,
        FIXED
    };

    struct VertexAttributeConfig
    {
        enum Type 
        {
            FLOAT,
            VEC2,
            VEC3,
            VEC4
        } type;
        size_t binding; // Buffer binding slot this attribute points to
        size_t location;
        bool normalized;
    };

    struct VertexBufferConfig
    {
        void* data;
        size_t size;
    };
    STRONGLY_TYPED_WEAKREF(VertexBuffer);

    struct IndexBufferConfig
    {
        DataType type;
        void* data;
        size_t num_indices;
    };
    STRONGLY_TYPED_WEAKREF(IndexBuffer);

    enum BufferType
    {
        VERTEX,
        INDEX
    };

    struct Buffer
    {
        BufferType type;

        union {
            VertexBuffer vertex_buffer;
        };
    };

    enum TextureType
    {
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D
    };

    struct TextureConfig
    {
        TextureType type;

        enum class WrapType {
            REPEAT,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER
        } wrap_type;

        enum class MinFilterType {
            NEAREST,
            LINEAR,
            NEAREST_MIPMAP_LINEAR,
            NEAREST_MIPMAP_NEAREST,
            LINEAR_MIPMAP_LINEAR
        } min_filter_type;

        enum class MagFilterType {
            NEAREST,
            LINEAR
        } mag_filter_type;

        void* data;
        size_t size;
    };
    STRONGLY_TYPED_WEAKREF(Texture);

    enum ShaderStageBit
    {
        VERTEX_SHADER = 1 << 0,
        GEOMETRY_SHADER = 1 << 1,
        TESSELATION_CONTROL_SHADER = 1 << 2,
        TESSELATION_EVALUATION_SHADER = 1 << 3,
        FRAGMENT_SHADER = 1 << 4,
        COMPUTE_SHADER = 1 << 5
    };

    typedef unsigned int ShaderStageBitfield;

    struct ShaderStageConfig
    {
        ShaderStageBit stage;
        const char* source;
    };
    struct ShaderConfig
    {
        ShaderStageConfig* shader_stages;
        size_t num_stages;
    };
    STRONGLY_TYPED_WEAKREF(Shader);

    // Resources a pipeline needs are defined here, ie textures, 
    // vertex/index buffers, uniform buffers etc
    // Render pass is probably also relevant but we'll cross that bridge
    // when we get there
    struct PipelineConfig
    {
        Shader* shaders;
        size_t num_shaders;
        VertexAttributeConfig* vertex_attributes;
        size_t num_attributes;
        BufferType* buffer_types;
        size_t num_buffers;
        TextureType* texture_types;
        size_t num_textures;
    };
    STRONGLY_TYPED_WEAKREF(Pipeline);
    void assert_pipeline_config_valid(const PipelineConfig& config);

    enum BackendType
    {
        OPENGL_4
    };

    struct BackendConfig
    {
        size_t num_prealloc_buffers;
        size_t num_prealloc_textures;
        size_t num_prealloc_shaders;
        size_t num_prealloc_pipelines;
    };

    class Backend
    {
    public:
        virtual ~Backend() = 0;
        virtual VertexBuffer create_vertex_buffer(const VertexBufferConfig& config) = 0;
        virtual void destroy_vertex_buffer(const VertexBuffer& buffer) = 0;
        virtual IndexBuffer create_index_buffer(const IndexBufferConfig& config) = 0;
        virtual void destroy_index_buffer(const IndexBuffer& buffer) = 0;
        virtual Texture create_texture(const TextureConfig& config) = 0;
        virtual void destroy_texture(const Texture& texture) = 0;
        virtual Shader create_shader(const ShaderConfig& config) = 0;
        virtual void destroy_shader(const Shader& shader) = 0;
        virtual Pipeline create_pipeline(const PipelineConfig& config) = 0;
        virtual void destroy_pipeline(const Pipeline& pipeline) = 0;
    };

    Backend* init_backend(BackendType type);
    Backend* init_backend(BackendType type, const BackendConfig& config);
    void deinit_backend(Backend* backend);
}