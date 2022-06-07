#pragma once

#include <array>
#include <string>
#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"

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
		int Rotation;

	public:
		Tile();
		bool isNeighbor(Tile* voisin, int direction);
		virtual void updateTile(float);
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment, int width = 0, int height = 0) = 0;
		virtual void drawTileTransparent(cgp::vec3 position, project_scene_environment environment);
		virtual void initialiseTile() = 0;

		int getProximity(int dir);

};