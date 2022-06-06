#pragma once

#include "tile.hpp"
#include "Boid.hpp"
#include <string>
#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"

#define NBoids 200

class BoidTile : public Tile
{ 
	private:
		Boid B[NBoids];
		cgp::mesh mesh_green;
		cgp::mesh_drawable shape_green;
		
	public:
		BoidTile(int);
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;
};