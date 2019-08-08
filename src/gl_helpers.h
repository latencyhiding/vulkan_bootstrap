#pragma once

#include <vector>
#include "glad/glad.h"

namespace GLHelpers
{
    struct VertexAttribute
    {
        enum Type {
            FLOAT,
            VEC2,
            VEC3,
            VEC4
        };
        const char* name;
        GLint size;
        GLenum type;
        GLboolean normalized;
    };

    struct VertexFormat
    {
        VertexAttribute attributes[GL_MAX_VERTEX_ATTRIBS];
        size_t num_attributes;
        size_t stride;        
    };

    void create_vertex_format(VertexFormat* vf, const VertexAttribute* attribs, size_t num_attribs);
    VertexAttribute create_attribute(const char* name, const VertexAttribute::Type type);
    void bind_buffer(GLuint index, const VertexFormat* vf, GLuint buffer);
}