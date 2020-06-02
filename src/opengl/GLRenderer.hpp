#pragma once

#include "../Renderer.hpp"
#include "../Texture.hpp"

#include "GL.hpp"
#include "GLMaterial.hpp"
#include "GLMesh.hpp"

#include "materials/GLMaterialBasic.hpp"

#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace OpenGL
{
    class GLRenderer : public Application::Renderer
    {
    private:
        const std::function<void()> swap_buffers;

        GLuint shader_program_basic = 0;

        std::shared_ptr<GLMaterialBasic> shader_basic;
        std::shared_ptr<GLMesh> mesh_basic;

    public:
        GLRenderer(
            uint32_t width,
            uint32_t height,
            std::function<void()> swap_buffers);
        virtual ~GLRenderer();

        void Render();

        std::shared_ptr<Texture<TextureDataByteRGBA>> MakeTexture(
            uint32_t width,
            uint32_t height);
    };
}
}
