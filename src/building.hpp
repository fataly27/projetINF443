#pragma once
#include "ProjectEnvironment.hpp"
#include "cgp/cgp.hpp"

class House
{
	public:
		House();
		void initializeHouse();
		void drawHouse(project_scene_environment environment,  cgp::vec3 position, float angle);
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