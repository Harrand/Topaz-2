#include "gl/index_snippet.hpp"
#include <algorithm>

namespace tz::gl
{
	IndexSnippet::IndexSnippet(std::size_t begin, std::size_t end, std::size_t offset): begin(begin), end(end), index_offset(offset){}
	IndexSnippet::IndexSnippet(const tz::gl::Manager& manager, tz::gl::Manager::Handle mesh_handle): IndexSnippet(manager.get_indices_offset(mesh_handle), manager.get_indices_offset(mesh_handle) + manager.get_number_of_indices(mesh_handle), manager.get_vertices_offset(mesh_handle)){}

	gpu::DrawElementsIndirectCommand IndexSnippet::mdi() const
	{
		std::size_t first = this->begin;
		std::size_t second = this->end;
		std::size_t big = std::max(first, second);
		std::size_t small = std::min(first, second);
		std::size_t index_count = 1 + big - small;
		return {static_cast<GLuint>(index_count), 1, static_cast<GLuint>(small), static_cast<GLint>(this->index_offset), 0};
	}

	std::size_t IndexSnippetList::size() const
	{
		return this->snippets.size();
	}

	bool IndexSnippetList::empty() const
	{
		return this->snippets.empty();
	}

	std::size_t IndexSnippetList::emplace_range(std::size_t begin, std::size_t end)
	{
		this->snippets.emplace_back(begin, end, 0u);
		return this->snippets.size() - 1;
	}

	std::size_t IndexSnippetList::emplace_range(std::size_t begin, std::size_t end, std::size_t index_offset)
	{
		this->snippets.emplace_back(begin, end, index_offset);
		return this->snippets.size() - 1;
	}

	std::size_t IndexSnippetList::emplace_range(const tz::gl::Manager& manager, tz::gl::Manager::Handle mesh_handle)
	{
		this->snippets.emplace_back(manager, mesh_handle);
		return this->snippets.size() - 1;
	}

	tz::gl::MDIDrawCommandList IndexSnippetList::get_command_list() const
	{
		tz::gl::MDIDrawCommandList cmds;
		for(const IndexSnippet& snippet : this->snippets)
		{
			cmds.add(snippet.mdi());
		}
		return cmds;
	}

	const IndexSnippet& IndexSnippetList::operator[](std::size_t idx) const
	{
		topaz_assert(idx < this->size(), "tz::gl::IndexSnippetList::operator[", idx, "]: Out of range! Size: ", this->size());
		return this->snippets[idx];
	}
}