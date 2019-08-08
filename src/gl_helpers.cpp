#include "gl_helpers.h"
#include "utils.h"

namespace GLHelpers
{
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

    void create_vertex_format(VertexFormat* vf, 
                            const VertexAttribute* attribs, 
                            size_t num_attribs)
    {
        vf->num_attributes = num_attribs;
        vf->stride = 0;
        for (size_t i = 0; i < num_attribs; i++)
        {
            const VertexAttribute* attrib = &attribs[i];
            vf->attributes[i] = *attrib;
            vf->stride += get_gl_type_bytes(attrib->type);
        }
    }

    VertexAttribute create_attribute(const char* name, const VertexAttribute::Type type)
    {
        VertexAttribute attrib;
        attrib.name = name;
        attrib.normalized = false;
        
        switch (type)
        {
            case VertexAttribute::Type::FLOAT: 
                attrib.size = 1;
                attrib.type = GL_FLOAT;
                break;
            case VertexAttribute::Type::VEC2: 
                attrib.size = 2;
                attrib.type = GL_FLOAT;
                break;
            case VertexAttribute::Type::VEC3: 
                attrib.size = 3;
                attrib.type = GL_FLOAT;
                break;
            case VertexAttribute::Type::VEC4: 
                attrib.size = 4;
                attrib.type = GL_FLOAT;
                break;
        }

        return attrib;
    }
    
    void bind_buffer(GLuint index, const VertexFormat* vf, GLuint buffer)
    {
        
    }
}