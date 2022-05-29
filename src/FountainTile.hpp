#pragma once

#include "tile.hpp"
#include <string>
#include <list>
#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"




struct particle_structure
{
	cgp::vec3 p; // Position
	cgp::vec3 v; // Speed
};

class FountainTile : public Tile
{
	private:
		float const sphere_radius = 0.05f;
		float const waitNewSphere = 0.01f;
		const cgp::vec3 g = { 0.0f,0.0f,-9.81f };
		const float m = 0.01f;
	
		cgp::mesh mesh_black;
		cgp::mesh mesh_blue;
		cgp::mesh mesh_green;
		cgp::mesh mesh_grey;
		cgp::mesh_drawable shape_black;
		cgp::mesh_drawable shape_blue;
		cgp::mesh_drawable shape_green;
		cgp::mesh_drawable shape_grey;

		std::list<particle_structure> particles;

		cgp::mesh_drawable sphere;

		float timeCount = 0;

	public:
		FountainTile(int index);
		virtual void drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;
};