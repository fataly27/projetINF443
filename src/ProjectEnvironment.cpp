#include "ProjectEnvironment.hpp"

using namespace cgp;

void opengl_uniform(GLuint shader, project_scene_environment const& environment)
{
	// Send the default uniforms from the basic scene_environment
	opengl_uniform(shader, scene_environment_basic(environment));

	// Send the our custom values as uniforms
	opengl_uniform(shader, "fog_falloff", environment.fog_falloff);
	opengl_uniform(shader, "plane", environment.plane);
};