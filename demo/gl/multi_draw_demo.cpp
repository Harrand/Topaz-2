#include "core/core.hpp"
#include "core/debug/print.hpp"
#include "gl/shader.hpp"
#include "gl/shader_compiler.hpp"
#include "gl/object.hpp"
#include "gl/buffer.hpp"
#include "gl/frame.hpp"
#include "gl/draw_command.hpp"
#include "render/device.hpp"
#include "GLFW/glfw3.h"

const char *vtx_shader_src = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *frg_shader_src = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.8f, 0.15f, 0.0f, 1.0f);\n"
	"}\n\0";

int main()
{
	// Minimalist Graphics Demo.
	tz::core::initialise("Topaz Graphics Demo");
	{
		tz::gl::ShaderCompiler cpl;
		tz::gl::ShaderProgram prg;
		tz::gl::Shader* vs = prg.emplace(tz::gl::ShaderType::Vertex);
		vs->upload_source(vtx_shader_src);
		tz::gl::Shader* fs = prg.emplace(tz::gl::ShaderType::Fragment);
		fs->upload_source(frg_shader_src);

		cpl.compile(*vs);
		cpl.compile(*fs);
		cpl.link(prg);

		const float vertices[] = {
		-0.5f, -0.5f, 0.0f, // A
		 0.0f, -0.5f, 0.0f, // B
		 -0.25f, 0.5f, 0.0f,  // C 
		 0.25f, 0.5f, 0.0f, // D  
		 0.5f, -0.5f, 0.0f // E
		};

		/**
		 *          C       D
		 *          o       o
		 *         / \     / \
		 *        /   \   /   \
		 *       /     \ /     \
		 *      o-------o-------o
		 *      A       B       E
		 * 
		 * tri-left = {0, 1, 2} ABC
		 * tri-right = {1, 4, 3} BED
		 */
		tz::gl::Object o;
		std::size_t vbo_id = o.emplace_buffer<tz::gl::BufferType::Array>();
		tz::gl::VBO* vbo = o.get<tz::gl::BufferType::Array>(vbo_id);

		vbo->terminal_resize(sizeof(vertices));
		tz::mem::UniformPool<float> vertex_pool = vbo->map_uniform<float>();
		for(std::size_t i = 0; i < vertex_pool.capacity(); i++)
			vertex_pool.set(i, vertices[i]);
		auto add_pos = [&vertex_pool](float x, float y, float z)
		{
			vertex_pool[0] += x;
			vertex_pool[3] += x;
			vertex_pool[6] += x;
			vertex_pool[9] += x;
			vertex_pool[12] += x;

			vertex_pool[1] += y;
			vertex_pool[4] += y;
			vertex_pool[7] += y;
			vertex_pool[10] += y;
			vertex_pool[13] += y;

			vertex_pool[2] += z;
			vertex_pool[5] += z;
			vertex_pool[8] += z;
			vertex_pool[11] += z;
			vertex_pool[14] += z;
		};

		o.format(vbo_id, tz::gl::fmt::three_floats);
		std::size_t ibo_id = o.emplace_buffer<tz::gl::BufferType::Index>();
		tz::gl::IBO* ibo = o.get<tz::gl::BufferType::Index>(ibo_id);
		unsigned int indices[] = {0, 1, 2, 1, 4, 3};
		ibo->resize(sizeof(indices));
		ibo->send(indices);

		tz::core::IWindow& wnd = tz::core::get().window();
		wnd.register_this();
		wnd.emplace_custom_key_listener([&add_pos](tz::input::KeyPressEvent e)
		{
			switch(e.key)
			{
			case GLFW_KEY_W:
				add_pos(0.0f, 0.05f, 0.0f);
				tz::debug_printf("moving forward.\n");
			break;
			case GLFW_KEY_S:
				add_pos(0.0f, -0.05f, 0.0f);
				tz::debug_printf("moving backward.\n");
			break;
			case GLFW_KEY_A:
				add_pos(-0.05f, 0.0f, 0.0f);
				tz::debug_printf("moving left\n");
			break;
			case GLFW_KEY_D:
				add_pos(0.05f, 0.0f, 0.0f);
				tz::debug_printf("moving right\n");
			break;
			}
		});

		glClearColor(0.0f, 0.3f, 0.15f, 1.0f);
		tz::render::Device dev{wnd.get_frame(), &prg, &o};
		dev.set_handle(ibo_id);
		// Make a snippet.
		tz::gl::IndexSnippetList snippets;
		// ibo_id is an opaque-handle corresponding to an array equal to:
		// unsigned int indices[] = {0, 1, 2, 1, 4, 3};
		// One render invocation contains the index-range between 0-3 (0, 1, 2)
		snippets.emplace_range(0, 2, 0);
		// The other contains 3-6 (1, 4, 3)
		snippets.emplace_range(3, 5, 0);
		// Tell the render device to prepare a command list.
		dev.set_indices(snippets);
		while(!wnd.is_close_requested())
		{
			// Clear backbuffer.
			dev.clear();
			// Bind everything and go! MDI!
			dev.render();

			wnd.update();
			tz::core::update();
		}
	}
	tz::core::terminate();
}