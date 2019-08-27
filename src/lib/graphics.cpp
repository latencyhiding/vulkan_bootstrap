#include "graphics.h"
#include "utils.h"

#include "graphics_gl4.h"

namespace Graphics
{
////////////////////////////////////////////////////////////////////////////////
// Graphics init
////////////////////////////////////////////////////////////////////////////////

    Backend::~Backend() {
        // Some bullshit C++ thing where you need to define virtual destructors
    }

    Backend* init_backend(BackendType type)
    {
        const BackendConfig default_config = {
            2048,
            2048,
            2048,
            2048
        };
        return init_backend(type, default_config);
    }

    Backend* init_backend(BackendType type, const BackendConfig& config)
    {
        // Just returns a singleton.
        static GL4Backend* gl4_backend_singleton = nullptr;
        
        Backend* backend_singleton;
        // We're just newing these. Not an issue, Ray...
        switch (type)
        {
            case OPENGL_4:
                if (!gl4_backend_singleton)
                    gl4_backend_singleton = new GL4Backend(config);
                backend_singleton = gl4_backend_singleton;
                break;
            default:
                RUNTIME_ERROR("Unknown backend type %d", type);
        }

        return backend_singleton;
    }

    void deinit_backend(Backend* backend)
    {
        delete backend;
    }

    void assert_pipeline_config_valid(const PipelineConfig& config)
    {
        ASSERT_MSG(config.num_buffers < GRAPHICS_PIPELINE_MAX_BUFFERS, "Invalid pipeline config: Num buffers %zu exceeds max buffers %d", config.num_buffers, GRAPHICS_PIPELINE_MAX_BUFFERS);
        ASSERT_MSG(config.num_textures < GRAPHICS_PIPELINE_MAX_TEXTURES, "Invalid pipeline config: Num textures %zu exceeds max textures %d", config.num_textures, GRAPHICS_PIPELINE_MAX_TEXTURES);
        ASSERT_MSG(config.num_attributes < GRAPHICS_MAX_VERTEX_ATTRIBS, "Invalid pipeline config: Num attributes %zu exceeds max attributes %d", config.num_textures, GRAPHICS_MAX_VERTEX_ATTRIBS);

        for (size_t i = 0; i < config.num_buffers; i++)
        {
            BufferType type = config.buffer_types[i];
            ASSERT_MSG(type != INDEX, "Invalid pipeline config: Cannot bind index buffers")
        }

        // TODO: Check shader that shader stages don't overlap, or that one shader
        // doesn't have a stage in between another
    }
}