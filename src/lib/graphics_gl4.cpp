#pragma once
#include "graphics_gl4.h"

////////////////////////////////////////////////////////////////////////////////
// GL4 implementation
////////////////////////////////////////////////////////////////////////////////

namespace Graphics
{
    static DataType data_type_for_attribute_type(VertexAttributeConfig::Type type, int* out_size)
    {
        switch (type)
        {
            case VertexAttributeConfig::Type::FLOAT: *out_size = 1; return DataType::FLOAT;
            case VertexAttributeConfig::Type::VEC2: *out_size = 2; return DataType::FLOAT;
            case VertexAttributeConfig::Type::VEC3: *out_size = 3; return DataType::FLOAT;
            case VertexAttributeConfig::Type::VEC4: *out_size = 4; return DataType::FLOAT;
            default: RUNTIME_ERROR("Unknown type %d", type);
        }
    }
    
    static GLenum get_gl_type_enum(DataType type)
    {
        switch (type)
        {
            case BYTE: return GL_BYTE;
            case UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
            case SHORT: return GL_SHORT;
            case UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
            case INT: return GL_INT;
            case UNSIGNED_INT: return GL_UNSIGNED_INT;
            case HALF_FLOAT: return GL_HALF_FLOAT;
            case FLOAT: return GL_FLOAT;
            case DOUBLE: return GL_DOUBLE;
            case FIXED: return GL_FIXED;
            default: RUNTIME_ERROR("Unknown type %d", type);
        }
    }

    static size_t get_gl_type_bytes(GLenum type)
    {
        switch (type)
        {
            case GL_BYTE: 
            case GL_UNSIGNED_BYTE: return 1;
            case GL_SHORT: 
            case GL_UNSIGNED_SHORT: return 2;
            case GL_INT:
            case GL_UNSIGNED_INT: return 4;
            case GL_HALF_FLOAT: return 2;
            case GL_FLOAT: return 4;
            case GL_DOUBLE: return 8;
            case GL_FIXED: return 4;
            default: RUNTIME_ERROR("Unknown GLenum type %d", type);
        }
    }

    static GLbitfield get_gl_shader_stage_bitfield(ShaderStageBitfield shader_stage_bitfield)
    {
        GLbitfield result = 0;
        if (shader_stage_bitfield & VERTEX_SHADER)
            result |= GL_VERTEX_SHADER_BIT;
        if (shader_stage_bitfield & FRAGMENT_SHADER)
            result |= GL_FRAGMENT_SHADER_BIT;
        if (shader_stage_bitfield & GEOMETRY_SHADER)
            result |= GL_GEOMETRY_SHADER_BIT;
        if (shader_stage_bitfield & TESSELATION_CONTROL_SHADER)
            result |= GL_TESS_CONTROL_SHADER_BIT;
        if (shader_stage_bitfield & TESSELATION_EVALUATION_SHADER)
            result |= GL_TESS_EVALUATION_SHADER_BIT;
        if (shader_stage_bitfield & COMPUTE_SHADER)
            result |= GL_COMPUTE_SHADER_BIT;
        return result;
    }

    static GLenum get_gl_shader_stage_type(ShaderStageBit stage)
    {
        switch (stage)
        {
            case VERTEX_SHADER: return GL_VERTEX_SHADER;
            case GEOMETRY_SHADER: return GL_GEOMETRY_SHADER;
            case TESSELATION_CONTROL_SHADER: return GL_TESS_CONTROL_SHADER;
            case TESSELATION_EVALUATION_SHADER: return GL_TESS_EVALUATION_SHADER;
            case COMPUTE_SHADER: return GL_COMPUTE_SHADER;
            default: RUNTIME_ERROR("Unknown shader stage type %d", stage);
        }
    }

    static GLuint gl_compile_shader(const char* source, GLenum shader_type)
    {
        GLuint new_shader = glCreateShader(shader_type); 
        int length = strlen(source);
        glShaderSource(new_shader, 1, &source, &length);
                    
        GLint is_compiled = 0;
        glGetShaderiv(new_shader, GL_COMPILE_STATUS, &is_compiled);
        if(is_compiled == GL_FALSE)
        {
            GLint max_length = 0;
            glGetShaderiv(new_shader, GL_INFO_LOG_LENGTH, &max_length);

            std::vector<GLchar> error_log(max_length);
            glGetShaderInfoLog(new_shader, max_length, &max_length, &error_log[0]);

            glDeleteShader(new_shader);
            RUNTIME_ERROR("Shader compilation error: %s", &error_log[0])
        }
        return new_shader;
    }

    static GLuint gl_link_shader(GLuint* shaders, size_t num_shaders)
    {
        GLuint program = glCreateProgram();
        glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);

        for (size_t i = 0; i < num_shaders; i++)
            glAttachShader(program, shaders[i]);
        
        glLinkProgram(program);

        GLint is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*) &is_linked);
        if (is_linked == GL_FALSE)
        {
            GLint max_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

            std::vector<GLchar> error_log(max_length);
            glGetProgramInfoLog(program, max_length, &max_length, &error_log[0]);
            
            glDeleteProgram(program);
            
            RUNTIME_ERROR("Shader linking error: %s", &error_log[0]);
        }

        for (size_t i = 0; i < num_shaders; i++)
            glDetachShader(program, shaders[i]);

        return program;
    }

    static GLuint gl_create_shader_pipeline(GL4Shader* shaders, size_t num_shaders)
    {
        GLuint pipeline;
        glGenProgramPipelines(1, &pipeline);

        for (size_t i = 0; i < num_shaders; i++)
        {
            glUseProgramStages(pipeline, shaders[i].stage_bitfield, shaders[i].program);
        }
        glValidateProgramPipeline(pipeline);

        GLint validation_status;
        glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, (GLint*) &validation_status);

        if (validation_status != GL_TRUE)
        {
            GLint max_length = 0;
            glGetProgramPipelineiv(pipeline, GL_INFO_LOG_LENGTH, &max_length);

            std::vector<GLchar> error_log(max_length);
            glGetProgramPipelineInfoLog(pipeline, max_length, &max_length, &error_log[0]);
            
            glDeleteProgramPipelines(1, &pipeline);
            
            RUNTIME_ERROR("Shader linking error: %s", &error_log[0]);
        }

        return pipeline;
    }

    static GLenum get_gl_buffer_type(BufferType type)
    {
        switch (type)
        {
            case VERTEX: return GL_ARRAY_BUFFER;
            case INDEX: return GL_ELEMENT_ARRAY_BUFFER;
            default: RUNTIME_ERROR("Unknown type %d", type);
        }
    }

    static void get_gl_vertex_attribute(GL4VertexAttribute* out_attribute, const VertexAttributeConfig* attribute)
    {
        out_attribute->binding = attribute->binding;
        out_attribute->location = attribute->location;
        out_attribute->normalized = attribute->normalized;
        out_attribute->type = get_gl_type_enum(data_type_for_attribute_type(attribute->type, &(out_attribute->size)));
    }

    static void get_gl_vertex_format(GL4VertexFormat* format, VertexAttributeConfig* attributes, size_t num_attributes)
    {
        format->stride = 0;
        format->num_attributes = num_attributes;
        for (size_t i = 0; i < num_attributes; i++)
        {
            get_gl_vertex_attribute(&(format->attributes[i]), &attributes[i]);
            format->stride += format->attributes[i].size * get_gl_type_bytes(format->attributes[i].type);
        }
    }

    GL4Backend::GL4Backend(const BackendConfig& config)
        : m_buffers(config.num_prealloc_buffers)
        , m_textures(config.num_prealloc_textures)
        , m_shaders(config.num_prealloc_shaders)
        , m_pipelines(config.num_prealloc_pipelines)
    {
    }

    GL4Backend::~GL4Backend()
    {
    }

    VertexBuffer GL4Backend::create_vertex_buffer(const VertexBufferConfig& config)
    {
        GLuint new_buffer;
        glGenBuffers(1, &new_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, new_buffer);
        glBufferData(GL_ARRAY_BUFFER, config.size, config.data, GL_STATIC_DRAW);
        return {m_buffers.add(new_buffer)};
    }

    void GL4Backend::destroy_vertex_buffer(const VertexBuffer& buffer)
    {
        destroy_buffer(buffer.handle);
    }

    IndexBuffer GL4Backend::create_index_buffer(const IndexBufferConfig& config)
    {
        GLuint new_buffer;
        glGenBuffers(1, &new_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_gl_type_bytes(get_gl_type_enum(config.type)) * config.num_indices, config.data, GL_STATIC_DRAW);
        return {m_buffers.add(new_buffer)};
    }

    void GL4Backend::destroy_index_buffer(const IndexBuffer& buffer)
    {
        destroy_buffer(buffer.handle);
    }

    Texture GL4Backend::create_texture(const TextureConfig& config)
    {
        // TODO
    }

    void GL4Backend::destroy_texture(const Texture& texture)
    {
        // TODO
    }

    Shader GL4Backend::create_shader(const ShaderConfig& config)
    {
        ASSERT_MSG(config.num_stages > 0, "Cannot create shader with zero stages");

        GL4Shader result;

        // Create stage bitfield
        ShaderStageBitfield stage_bitfield = 0;
        for (size_t i = 0; i < config.num_stages; i++)
            stage_bitfield |= config.shader_stages[i].stage;

        // Convert stage bitfield to GLbitfield
        result.stage_bitfield = get_gl_shader_stage_bitfield(stage_bitfield);
        
        // Compile shaders
        std::vector<GLuint> shader_stages;
        for (size_t i = 0; i < config.num_stages; i++)
            shader_stages.push_back(gl_compile_shader(config.shader_stages[i].source, get_gl_shader_stage_type(config.shader_stages[i].stage)));
        
        // Link shaders
        result.program = gl_link_shader(&shader_stages[0], shader_stages.size());

        // Clean up shader objects
        for (size_t i = 0; i < shader_stages.size(); i++)
            glDeleteShader(shader_stages[i]);

        return {m_shaders.add(result)};
    }

    void GL4Backend::destroy_shader(const Shader& shader)
    {
        GL4Shader* shader_obj = m_shaders.get(shader.handle);
        if (!shader_obj)
        {
            LOG_WARNING("Invalid shader handle")
            return;
        }

        glDeleteProgram(shader_obj->program);
        m_shaders.remove(shader.handle);
    }

    Pipeline GL4Backend::create_pipeline(const PipelineConfig& config)
    {
        assert_pipeline_config_valid(config);

        GL4Pipeline new_pipeline;

        // Fill in buffer types
        new_pipeline.num_buffers = config.num_buffers;
        for (size_t i = 0 ; i < config.num_buffers; i++)
        {
            new_pipeline.buffer_types[i] = get_gl_buffer_type(config.buffer_types[i]);
        }

        // TODO: Fill in texture types

        // Fill in vertex format
        // TODO: Hash 'n cache vertex formats
        get_gl_vertex_format(&(new_pipeline.vertex_format), config.vertex_attributes, config.num_attributes);

        // Create shader pipeline
        // TODO: Hash 'n cache shader pipelines
        std::vector<GL4Shader> shaders;
        for (size_t i = 0; i < config.num_shaders; i++)
        {
            GL4Shader* shader = m_shaders.get(config.shaders[i].handle);
            ASSERT_MSG(shader, "Shader not found. Did you delete it?");

            shaders[i] = *shader;
        }
        
        new_pipeline.shader_pipeline = gl_create_shader_pipeline(&shaders[0], shaders.size());

        return {m_pipelines.add(new_pipeline)};
    }

    void GL4Backend::destroy_pipeline(const Pipeline& pipeline)
    {
        GL4Pipeline* pipeline_obj = m_pipelines.get(pipeline.handle);
        if (!pipeline_obj)
        {
            LOG_WARNING("Invalid pipeline handle")
            return;
        }

        glDeleteProgramPipelines(1, &(pipeline_obj->shader_pipeline));
        m_pipelines.remove(pipeline.handle);
    }

    void GL4Backend::destroy_buffer(const Utils::WeakRef& handle)
    {
        const GLuint* gl_buffer = m_buffers.get(handle);
        if (!gl_buffer)
        {
            LOG_WARNING("Invalid buffer handle");
            return;
        }

        glDeleteBuffers(1, gl_buffer);
        m_buffers.remove(handle);
    }
}