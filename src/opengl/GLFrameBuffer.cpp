#include "GLFrameBuffer.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    /*GLFrameBuffer::GLFrameBuffer(
        ContextPtr context,
        SamplerPresets samplers,
        uint16_t width_,
        uint16_t height_,
        TextureFormat format,
        bool mipmaps) :
    FrameBuffer(
        width_,
        height_),
    context(context),
    samplers(samplers),
    mipmaps(mipmaps),
    format(format)
    {
    }

    GLFrameBuffer::~GLFrameBuffer()
    {
    }

    void GLFrameBuffer::BindTarget()
    {
        glBindFramebuffer(
            GL_FRAMEBUFFER,
            frame_buffer);

        glViewport(
            0, 0,
            width, height);

        glClearColor(
            clear_color.x,
            clear_color.y,
            clear_color.z,
            clear_color.w);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);
    }

    void GLFrameBuffer::UnbindTarget()
    {
        glBindFramebuffer(
            GL_FRAMEBUFFER,
            0);

        glViewport(
            0,
            0,
            context->backend->window_width,
            context->backend->window_height);
    }

    void GLFrameBuffer::GenerateMipMaps()
    {
        glActiveTexture(
            GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            texture_buffer);

        glGenerateMipmap(
            GL_TEXTURE_2D);
    }

    void GLFrameBuffer::Destroy()
    {
        context->graphics->GPUAssetUnregister(
            Asset::GetID());

        // TODO destroy other buffers
    }

    shared_ptr<vector<uint8_t>> GLFrameBuffer::Read()
    {
        if (format != TextureFormat::RGBA8)
        {
            throw std::runtime_error("Only RGBA8 format is currently readable.");
        }

        // TODO: match pixels array to gl_format

        shared_ptr<vector<uint8_t>> pixels = make_shared<vector<uint8_t>>(
            4 * width * height);

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            frame_buffer);

        glViewport(
            0, 0,
            width, height);

        glReadPixels(
            0, 0,
            width, height,
            gl_format,
            gl_type,
            &(*pixels)[0]);

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            NULL);

        return pixels;
    }

    void GLFrameBuffer::UploadToGPU()
    {
        glGenFramebuffers(
            1,
            &frame_buffer);

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            frame_buffer);

        glGenTextures(
            1,
            &texture_buffer);

        glBindTexture(
            GL_TEXTURE_2D,
            texture_buffer);

        GetGLTextureFormat(
            format,
            gl_type,
            gl_format,
            gl_internal_format);

        glTexImage2D(
            GL_TEXTURE_2D,
            mipmaps ? 0 : 1,
            gl_internal_format,
            width,
            height,
            0,
            gl_format,
            gl_type,
            0);

        glGenRenderbuffers(
            1,
            &depth_renderbuffer);

        glBindRenderbuffer(
            GL_RENDERBUFFER,
            depth_renderbuffer);

        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            width,
            height);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            cout << "OpenGL error: " << err << endl;
        }

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            depth_renderbuffer);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            texture_buffer,
            0);

        GLenum draw_buffers[1] = {
            GL_COLOR_ATTACHMENT0
        };

        glDrawBuffers(
            1,
            draw_buffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            assert(false);
        }

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            0);
    }

    void GLFrameBuffer::Bind(
        uint16_t texture_unit,
        uint16_t sampler_id,
        TextureSamplerMode sampler_mode)
    {
        glActiveTexture(
            GL_TEXTURE0 + texture_unit);

        glBindTexture(
            GL_TEXTURE_2D,
            texture_buffer);

        glBindSampler(
            texture_unit,
            samplers[sampler_mode]);

        glUniform1i(
            sampler_id,
            texture_unit);
    }*/
}
}
}
