//
// Created by Harrand on 13/12/2019.
//

#include "core/core.hpp"
#include "core/debug/assert.hpp"
#include "core/debug/print.hpp"
#include "core/tz_glad/glad_context.hpp"
#include "gl/tz_imgui/imgui_context.hpp"
#include "GLFW/glfw3.h"

namespace tz::core
{
	TopazCore::TopazCore() noexcept: tz_window(nullptr), secondary_windows(), initialised(false){}

	void TopazCore::initialise(const char* app_name)
	{
		topaz_assert(!this->initialised, "TopazCore::initialise(): Attempt to initialise but we're already marked as initialised!");
		this->initialised = true;
		// Initialise GLFW...
		tz::ext::glfw::initialise(tz::ext::glfw::WindowCreationArgs{app_name, 1920, 1080});
		// Create the GLFW window and set this to be the global GLFW context window.
		tz::ext::glad::get().pre_init();
		this->tz_window = std::make_unique<GLFWWindow>(tz::ext::glfw::get());
		this->tz_window->set_active_context();
		tz::ext::glad::get().load();
		tz::ext::imgui::initialise();

		tz::debug_printf("tz::initialise(): Success\n");
	}

	void TopazCore::terminate()
	{
		topaz_assert(this->initialised, "TopazCore::terminate(): Attempt to terminate but we're not marked as initialised!");
		this->initialised = false;
		tz::ext::imgui::terminate();
		// TODO: Burn stuff here.
		tz::ext::glfw::terminate();

		tz::debug_printf("tz::terminate(): Success\n");
	}

	bool TopazCore::is_initialised() const
	{
		return this->initialised;
	}

	const tz::ext::glfw::GLFWContext& TopazCore::context() const
	{
		return tz::ext::glfw::get();
	}

	const tz::ext::glad::GLADContext& TopazCore::glad_context() const
	{
		return tz::ext::glad::get();
	}
	
	const tz::core::IWindow& TopazCore::window() const
	{
		topaz_assert(this->tz_window != nullptr, "TopazCore::window(): Window wasn't initialised yet. Did you forget to tz::initialise?");
		return *this->tz_window;
	}
	
	tz::core::IWindow& TopazCore::window()
	{
		topaz_assert(this->tz_window != nullptr, "TopazCore::window(): Window wasn't initialised yet. Did you forget to tz::initialise?");
		return *this->tz_window;
	}

	void TopazCore::enable_wireframe_mode(bool wireframe) const
	{
		if(wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.0f);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	
	const tz::core::IWindow* TopazCore::get_extra_window(std::size_t window_id) const
	{
		topaz_assert(tz::core::get().is_initialised(), "TopazCore::get_extra_window(", window_id, "): tz::core is not yet initalised!");
		if(this->secondary_windows.size() > window_id)
		{
			return this->secondary_windows[window_id].get();
		}
		return nullptr;
	}
	
	tz::core::IWindow* TopazCore::get_extra_window(std::size_t window_id)
	{
		topaz_assert(tz::core::get().is_initialised(), "TopazCore::get_extra_window(", window_id, "): tz::core is not yet initalised!");
		if(this->secondary_windows.size() > window_id)
		{
			return this->secondary_windows[window_id].get();
		}
		return nullptr;
	}
	
	void TopazCore::destroy_extra_window(std::size_t window_id)
	{
		topaz_assert(tz::core::get().is_initialised(), "TopazCore::destroy_extra_window(", window_id, "): tz::core is not yet initalised!");
		if(this->secondary_windows.size() > window_id)
		{
			this->secondary_windows[window_id] = nullptr;
		}
	}

	static TopazCore global_core;
	static ResourceManager root_manager{tz::core::project_directory};

	void initialise(const char* app_name)
	{
		global_core.initialise(app_name);
	}

	void update()
	{
		glfwPollEvents();
		tz::ext::imgui::update();
	}
	
	void terminate()
	{
		global_core.terminate();
	}

	TopazCore& get()
	{
		topaz_assert(global_core.is_initialised(), "Attempted tz::core::get() but tz::initialise() has not yet been invoked!");
		return global_core;
	}

	const ResourceManager& res()
	{
		return root_manager;
	}
}

