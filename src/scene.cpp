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
	//GLuint const shader_lights = opengl_load_shader("shaders/mesh_lights/vert.glsl", "shaders/mesh_lights/frag.glsl");                 
	//mesh_drawable::default_shader = shader_lights;   // set this shader as the default one for all new shapes declared after this line 


	// Create the surrounding shapes
	// ***************************************** //
	// The standard frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");

	/*
	// A central cube
	cube.initialize(mesh_primitive_cube(), "Cube");
	cube.transform.translation = { -2,0,0.5f };
	cube.transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, Pi / 4.0f);

	// The ground
	int N_terrain_samples = 500;
	float terrain_length = 60;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	ground.initialize(terrain_mesh, "terrain");
	ground.shading.color = { 0.75f,0.35f,0.7f };
	ground.shading.phong.specular = 0.0f; // non-specular terrain material

	// The ocean
	ocean.initialize(mesh_primitive_quadrangle({ -30,-30,0 }, { -30,30,0 }, { 30,30,0 }, { 30,-30,0 }), "Quad");
	ocean.shading.color = { 0.1f,0.02f,0.9f };

	// The lights displayed as spheres using this helper initializer (*)-optionnal
	light_drawable.initialize(shader_lights);
	*/

	// Environment parameters
	// ***************************************** //

	// The background color is set to match the color of the fog defined in the shader (*)
	environment.background_color = { 0.7f, 0.7f, 0.7f }; 


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

	
	P = Player();
	P.initializePlayer();

	H = House();
	H.initializeHouse();

	Car Ct(Cases[1]);
	C = Ct;
	C.initializeCar();
}

void scene_structure::display()
{
	float dt = timer.update();
	float t = timer.t;

	// Update the position and color of the lights
	//compute_light_position(t, environment);

	// The standard frame
	if (gui.display_frame)
		draw(global_frame, environment);
	
	Cases[0]->updateCase(dt);

	for (int i = 0; i < NCases * NCases; i++) {
		Cases[i]->drawCase(environment);
	}
	

	C.updateCar(dt);
	C.drawCar(environment);

	P.update(sceneInputs, dt);

	//P.moveCamera(environment);
	P.drawPlayer(environment);

	H.drawHouse(environment, { 10,10,0 }, { 0,0,0 });




	// Display the elements of the scene
	//draw(cube, environment);
	//draw(ground, environment);
	//draw(ocean, environment);
	//draw(light_drawable, environment); // this is a helper function from multiple_lights (display all the spotlights as spheres) (*)-optionnal
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	display_gui_falloff(environment); // helper function from multiple_lights (*)-optionnal
	ImGui::SliderFloat("Speed", &Player::maxSpeed, 0, 10);
}


