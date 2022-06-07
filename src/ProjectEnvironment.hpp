#pragma once

#include "cgp/cgp.hpp"

struct project_scene_environment : cgp::scene_environment_basic
{
	// The characteristic attenuation due to the fog
	float fog_falloff = 0.005f;
	cgp::vec4 plane = cgp::vec4(0, 0, 1, -100);
};

void opengl_uniform(GLuint shader, project_scene_environment const& environment);