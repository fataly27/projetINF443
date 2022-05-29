#pragma once

#include <array>
#include "tile.hpp"
#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"

#define N 16

class Case
{
	private:
		static std::array<Tile*, N> Tiles;

		std::array<bool, N> TilesDispo;
		std::array<Case*, 4> Neighbors;
		cgp::vec3 Position;
		bool isFixed = false;
		int nbDispos = N;
		int FixedTile = -1;

	public:
		Case(cgp::vec3 position);

		void setNeighbor(Case* voisin, int direction);

		void fixTile();
		void update();
		void drawCase(scene_environment_with_multiple_lights environment);
		bool isNeighborCase(Tile* tile, int dir);

		bool isCaseFixed();
		int getNbDispos();

		static void initialiseTiles();

		friend std::ostream& operator<< (std::ostream& stream, const Case& laCase)
		{
			stream << laCase.FixedTile;
			return stream;
		}
};