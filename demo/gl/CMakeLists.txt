cmake_minimum_required(VERSION 3.9)
project(topaz_graphics_demo)

add_executable(topaz_bindless_texture_demo
		bindless_texture_demo.cpp
)

add_dependencies(topaz_bindless_texture_demo topaz)
target_link_libraries(topaz_bindless_texture_demo PRIVATE topaz)

add_executable(topaz_graphics_demo
		graphics_demo.cpp
)

add_dependencies(topaz_graphics_demo topaz)
target_link_libraries(topaz_graphics_demo PRIVATE topaz)

add_executable(topaz_mesh_demo
		mesh_demo.cpp
)

add_dependencies(topaz_mesh_demo topaz)
target_link_libraries(topaz_mesh_demo PRIVATE topaz)

add_executable(topaz_multi_draw_demo
	multi_draw_demo.cpp
)

add_dependencies(topaz_multi_draw_demo topaz)
target_link_libraries(topaz_multi_draw_demo PRIVATE topaz)

add_executable(topaz_postprocess_demo
	postprocess_demo.cpp
)

add_dependencies(topaz_postprocess_demo topaz)
target_link_libraries(topaz_postprocess_demo PRIVATE topaz)

add_executable(topaz_ssbo_tzglp_demo
		ssbo_tzglp_demo.cpp
)

add_dependencies(topaz_ssbo_tzglp_demo topaz)
target_link_libraries(topaz_ssbo_tzglp_demo PRIVATE topaz)