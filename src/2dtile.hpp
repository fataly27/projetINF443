#pragma once

#include "tile.hpp"
#include <string>
#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"


class TextureTile : public Tile
{
	private:
		cgp::mesh quadrangle_mesh;
		cgp::mesh_drawable shape;
		GLuint texture_image_id;
		std::string texture_file;

	public:
		TextureTile(int type);
		virtual void drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment) override;
		virtual void initialiseTile() override;
};