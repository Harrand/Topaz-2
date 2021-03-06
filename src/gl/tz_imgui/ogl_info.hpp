#ifndef TOPAZ_GL_IMGUI_OGL_INFO_HPP
#define TOPAZ_GL_IMGUI_OGL_INFO_HPP
#include "gl/tz_imgui/imgui_context.hpp"

namespace tz::ext::imgui::gl
{
	class OpenGLInfoWindow : public tz::ext::imgui::ImGuiWindow
	{
	public:
		OpenGLInfoWindow();
		virtual void render() override;
	};
}

#endif // TOPAZ_GL_IMGUI_OGL_INFO_HPP