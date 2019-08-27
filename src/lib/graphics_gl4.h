#pragma once

#include <glad/glad.h>
#include "graphics.h"

namespace Graphics
{
    struct GL4Shader
    {
        GLbitfield stage_bitfield;
        GLuint program;
    };

    struct GL4VertexAttribute
    {
        size_t binding;
        size_t location;
        GLint size;
        GLenum type;
        GLboolean normalized;
    };

    struct GL4VertexFormat
    {
        GL4VertexAttribute attributes[GRAPHICS_MAX_VERTEX_ATTRIBS];
        size_t num_attributes;
        size_t stride;
    };

    struct GL4Pipeline
    {
        // Replace this with a hash to get cached value and quick comparisons
        GL4VertexFormat vertex_format;
        // Can cache these separately by the components
        GLuint shader_pipeline;

        GLenum buffer_types[GRAPHICS_PIPELINE_MAX_BUFFERS];
        size_t num_buffers;
        GLenum texture_types[GRAPHICS_PIPELINE_MAX_TEXTURES];
        size_t num_textures;
    };

    class GL4Backend : public Backend
    {
    public:
        GL4Backend(const BackendConfig& config);
        ~GL4Backend();

        VertexBuffer create_vertex_buffer(const VertexBufferConfig& config);
        void destroy_vertex_buffer(const VertexBuffer& buffer);
        IndexBuffer create_index_buffer(const IndexBufferConfig& config);
        void destroy_index_buffer(const IndexBuffer& buffer);
        Texture create_texture(const TextureConfig& config);
        void destroy_texture(const Texture& texture);
        Shader create_shader(const ShaderConfig& config);
        void destroy_shader(const Shader& shader);
        Pipeline create_pipeline(const PipelineConfig& config);
        void destroy_pipeline(const Pipeline& pipeline);
    private:
        void destroy_buffer(const Utils::WeakRef& handle);
        void destroy_shader(const Utils::WeakRef& handle);
        void destroy_pipeline(const Utils::WeakRef& handle);

        Utils::WeakRefManager<GLuint> m_buffers;
        Utils::WeakRefManager<GLuint> m_textures;
        Utils::WeakRefManager<GL4Shader> m_shaders;
        Utils::WeakRefManager<GL4Pipeline> m_pipelines;
    };
}