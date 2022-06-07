#pragma once

#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"

// Create a mesh_drawable_lake in reusing the mesh_drawable structure and adding two extra texture IDs
struct mesh_drawable_lake : cgp::mesh_drawable 
{
	using mesh_drawable::mesh_drawable; // reuse the same constructor

	GLuint depth_texture = 0; // add a second texture
	GLuint dudv_texture = 0; // add a third texture
	float moveFactor = 0;
};


// Adapt the draw function for a mesh_drawable_multitexture
void draw(mesh_drawable_lake const& drawable, project_scene_environment const& environment);