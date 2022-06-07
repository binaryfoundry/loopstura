#pragma once

#include "GL.hpp"

#include <memory>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    /*class GLFrameBuffer : public FrameBuffer
    {
    public:
        GLFrameBuffer(
            ContextPtr context,
            SamplerPresets samplers,
            uint16_t width,
            uint16_t height,
            TextureFormat format,
            bool mipmaps);
        virtual ~GLFrameBuffer();

        const bool mipmaps;
        const TextureFormat format;

        GLuint frame_buffer = 0;
        GLuint texture_buffer = 0;
        GLuint depth_renderbuffer = 0;

        GLuint gl_type = GL_FLOAT;
        GLuint gl_format = GL_RGBA;
        GLuint gl_internal_format = GL_RGBA32F;

        void BindTarget();
        void UnbindTarget();

        shared_ptr<vector<uint8_t>> Read();

        void GenerateMipMaps();
        void UploadToGPU();
        void Destroy();

        ContextPtr context;
        SamplerPresets samplers;

        void Bind(
            uint16_t texture_unit,
            uint16_t sampler_id,
            TextureSamplerMode sampler_mode);
    };*/
}
}
}
