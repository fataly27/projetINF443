#include "scene.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

/** This file contains the custom implementation associated the scene 
*  The (*) indicates elements that are specific to the use of the multiple_lights and that should be handled if you want to use this effect in another scene. */

using namespace cgp;


void scene_structure::initialize()
{
	// Specific Shader (*)
	// ***************************************** //
	// Load a new custom shader that take into account spotlights (note the new shader file in shader/ directory)
	// Make sure you load an set this shader for the shapes that need to be illuminated
	GLuint const shader_lights = opengl_load_shader("shaders/project/vert.glsl", "shaders/project/frag.glsl");                 
	mesh_drawable::default_shader = shader_lights;   // set this shader as the default one for all new shapes declared after this line 

	// Initialize the skybox (*)
	// ***************************************** //
	skybox.initialize("assets/skybox/");         // indicate a path where to load the 6 texture images


	// Create the surrounding shapes
	// ***************************************** //
	// The standard frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");

	// Environment parameters
	// ***************************************** //

	// The background color is set to match the color of the fog defined in the shader (*)
	environment.background_color = { 0.75f, 0.82f, 0.9f };


	// Initialize the camera
	environment.projection = camera_projection::perspective(50.0f *Pi/180, 1.0f, 0.1f, 500.0f);
	environment.camera.distance_to_center = 10.0f;
	environment.camera.look_at({ 3,1,2 }, { 0,0,0.5 }, { 0,0,1 });

	Case::initialiseTiles();

	for (int i = 0; i < NCases; i++)
	{
		for (int j = 0; j < NCases; j++)
		{
			Cases[i * NCases + j] = new Case(cgp::vec3(10*i, 10*j, 0));

			if (j != 0)
			{
				Cases[i * NCases + j]->setNeighbor(Cases[i * NCases + j - 1], Left);
				Cases[i * NCases + j - 1]->setNeighbor(Cases[i * NCases + j], Right);
			}
			if (i != 0)
			{
				Cases[i * NCases + j]->setNeighbor(Cases[(i - 1) * NCases + j], Up);
				Cases[(i - 1) * NCases + j]->setNeighbor(Cases[i * NCases + j], Down);
			}
		}
	}

	std::srand(std::time(nullptr));

	bool isFixed = false;
	while (!isFixed)
	{
		Case* caseToFix = nullptr;
		int min = N;

		for (int i = 0; i < NCases * NCases; i++)
		{
			if (!Cases[i]->isCaseFixed())
			{
				if (caseToFix == nullptr || min > Cases[i]->getNbDispos())
				{
					caseToFix = Cases[i];
					min = Cases[i]->getNbDispos();
				}
			}
		}

		if (caseToFix != nullptr)
			caseToFix->fixTile();
		else
			isFixed = true;
	}

	for (int i = 0; i < NCases; i++)
	{
		for (int j = 0; j < NCases; j++)
		{
			std::cout << *Cases[i * NCases + j] << " ";
		}
		std::cout << std::endl;
	}
	
	BT = new BoidTile(0);
	FT = new FountainTile(0);
	TestLakeTile = new LakeTile(0, Up);
	BuildT = new BuildingTile(1000, Up);

	BuildT->initialiseTile();
	//TestLakeTile->initialiseTile();

	Car Ct(Cases[1]);

	P = Player();
	P.initializePlayer();

	H = House();
	H.initializeHouse();
}

void scene_structure::display()
{
	draw(skybox, environment);

	float dt = timer.update();
	float t = timer.t;

	// Update the position and color of the lights
	//compute_light_position(t, environment);

	// The standard frame
	if (gui.display_frame)
		draw(global_frame, environment);

	//TestLakeTile->updateTile(dt);
	//TestLakeTile->drawTile(vec3(-10, -10, 0), environment);
	BuildT->drawTile(vec3(-10, -10, 0), environment);


	H.drawHouse(environment, { 10,10,0 }, 0.23);
	
	Cases[0]->updateCase(dt);

	for (int i = 0; i < NCases * NCases; i++)
		Cases[i]->drawCase(environment);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(false);
	
	for (int i = 0; i < NCases * NCases; i++)
		Cases[i]->drawCaseTransparent(environment);

	//TestLakeTile->drawTileTransparent(vec3(-10, -10, 0), environment);

	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);

	P.update(sceneInputs, dt);
	P.moveCamera(environment);
	P.drawPlayer(environment);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);

	ImGui::SliderFloat("Fog falloff", &environment.fog_falloff, 0, 0.05f, "%0.5f", 2.0f);
	ImGui::SliderFloat("Speed", &Player::maxSpeed, 0, 10);
}