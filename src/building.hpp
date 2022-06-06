#pragma once
#include "tile.hpp"
#include "multiple_lights/multiple_lights.hpp"
#include "cgp/cgp.hpp"

class House
{
public:
	House();
	void initializeHouse();
	void drawHouse(scene_environment_with_multiple_lights environment,  cgp::vec3 position, cgp::vec3 dir);
private:
	int height;
	float doorPos;

	cgp::mesh house_mesh;
	cgp::mesh roof_mesh;

	cgp::mesh window_mesh;
	cgp::mesh door_mesh;


	cgp::mesh_drawable house;
	cgp::mesh_drawable roof;
	
	cgp::mesh_drawable window;
	cgp::mesh_drawable door;
};

class Building
{

};