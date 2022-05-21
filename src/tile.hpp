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
	private:
		std::array<int, 4> Aretes;

		cgp::mesh quadrangle_mesh;
		cgp::mesh_drawable shape;
		GLuint texture_image_id;
		std::string texture_file;

	public:
		Tile(int type);
		bool isNeighbor(Tile* voisin, int direction);
		void drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment);
		void initialiseTexture();
};