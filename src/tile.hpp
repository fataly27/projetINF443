#pragma once

#include <array>
#include <string>
#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"

enum
{
	Up,
	Right,
	Down,
	Left
};

class Tile
{
	protected:
		std::array<int, 4> Aretes;

	public:
		Tile();
		bool isNeighbor(Tile* voisin, int direction);
		virtual void updateTile(float);
		virtual void drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment) = 0;
		virtual void initialiseTile() = 0;


};