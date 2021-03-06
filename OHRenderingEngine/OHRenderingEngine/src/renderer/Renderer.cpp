#include "Renderer.h"


#include <GLFW/glfw3.h>


bool Renderer::s_AntiAliasingEnabled = true;

Renderer::Renderer():
	m_ActiveWindow(nullptr)
{
}


Renderer::~Renderer()
{
}


void Renderer::SetActiveWindow(GLFWwindow*& window)
{
	m_ActiveWindow = window;
}

void Renderer::Clear(unsigned int flag, glm::vec4 color)
{
	GlCall(glClearColor(color.x, color.y, color.z, color.a));
	GlCall(glClear(flag));
}

void Renderer::Draw(const VertexArray & vao, const Shader & shader, unsigned int count, unsigned int offset)
{
	vao.Bind();
	shader.Use();
	GlCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)offset));
}


void Renderer::ResizeWindow(int width, int height)
{
	GlCall(glViewport(0, 0, width, height));
}

void Renderer::SetLineWidth(float width)
{
	GlCall(glLineWidth(width));
}

void Renderer::DrawPoints(int count)
{
	GlCall(glDrawArrays(GL_POINTS, 0, count));
}

void Renderer::EnableDepthTesting()
{
	GlCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::DisableDepthTesting()
{
	GlCall(glDisable(GL_DEPTH_TEST));
}

void Renderer::EnableStencilTesting()
{
	GlCall(glEnable(GL_STENCIL_TEST));
}

void Renderer::DisableStencilTesting()
{
	GlCall(glDisable(GL_STENCIL_TEST));
}

void Renderer::EnableBlending()
{
	GlCall(glEnable(GL_BLEND));
}

void Renderer::SetDepthFunc(GLenum fn)
{
	GlCall(glDepthFunc(fn));
}

void Renderer::SetStencilOp(GLenum sfail, GLenum dpthfail, GLenum dpthpass)
{
	GlCall(glStencilOp(sfail, dpthfail, dpthpass));
}

void Renderer::EnableGammaCorrection()
{
	GlCall(glEnable(GL_FRAMEBUFFER_SRGB));
}

void Renderer::DisableGammaCorrection()
{
	GlCall(glDisable(GL_FRAMEBUFFER_SRGB));
}

void Renderer::BlitNamedFrameBuffer(const FrameBuffer & fboRead, const FrameBuffer & fboDraw, unsigned int srcX0, unsigned int srcY0, unsigned int srcX1, unsigned int srcY1, unsigned int dstX0, unsigned int dstY0, unsigned int dstX1, unsigned int dstY1)
{
	fboRead.Bind(GL_READ_FRAMEBUFFER);
	fboDraw.Bind(GL_DRAW_FRAMEBUFFER);
	GlCall(glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, GL_COLOR_BUFFER_BIT, GL_NEAREST));
}

void Renderer::VertexAttribPointer(unsigned int index, int size, GLenum type, bool normalized, GLsizei stride, const void * pointer)
{
	GlCall(glVertexAttribPointer(index, size, type, normalized, stride , pointer));
}

void Renderer::EnableVertexAttribArray(int index)
{
	GlCall(glEnableVertexAttribArray(index));
}

void Renderer::SetStencilMask(unsigned int mask)
{
	GlCall(glStencilMask(mask));
}

void Renderer::SetStencilFunc(GLenum fn, int ref, unsigned int mask)
{
	GlCall(glStencilFunc(fn, ref, mask));
}

void Renderer::DisableBlending()
{
	GlCall(glDisable(GL_BLEND));
}

void Renderer::SetBlendFactors(GLenum source_factor, GLenum dest_factor)
{
	GlCall(glBlendFunc(source_factor, dest_factor));
}

void Renderer::SetBlendFactorsSeparate(GLenum source_factor_rgb, GLenum dest_factor_rgb, GLenum source_factor_a, GLenum dest_factor_a)
{
	GlCall(glBlendFuncSeparate(source_factor_rgb, dest_factor_rgb, source_factor_a, dest_factor_a));
}

void Renderer::SetBlendEquation(GLenum func)
{
	GlCall(glBlendEquation(func));
}

void Renderer::EnableCulling()
{
	GlCall(glEnable(GL_CULL_FACE));
}

void Renderer::DisableCulling()
{
	GlCall(glDisable(GL_CULL_FACE));
}

void Renderer::CullFace(GLenum face)
{
	GlCall(glCullFace(face));
}

void Renderer::SetFrontFace(GLenum dir)
{
	GlCall(glFrontFace(dir));
}

void Renderer::SetDepthMask(bool mask)
{
	GlCall(glDepthFunc(mask));
}

void Renderer::EnableAntiAliasing()
{
	GlCall(glEnable(GL_MULTISAMPLE));
	s_AntiAliasingEnabled = true;
}

void Renderer::DisableAntiAliasing()
{
	GlCall(glDisable(GL_MULTISAMPLE));
	s_AntiAliasingEnabled = false;
}
