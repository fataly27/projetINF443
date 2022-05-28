#pragma once

#include "tile.hpp"
#include "Boid.hpp"
#include <string>
#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"

#define NBoids 100

class BoidTile : public Tile
{ 

private:
	Boid B[NBoids];
	cgp::mesh mesh_green;
	cgp::mesh_drawable shape_green;
public:
	BoidTile(int);
	virtual void drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment) override;
	virtual void initialiseTile() override;
	virtual void updateTile(float) override;
};