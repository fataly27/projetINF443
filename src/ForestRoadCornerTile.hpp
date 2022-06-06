#pragma once

#include "TreeTile.hpp"
#include "cgp/cgp.hpp"
#include <string>
#include "multiple_lights/multiple_lights.hpp"


class ForestRoadCornerTile : public TreeTile
{
	private:
		cgp::mesh mesh;
		cgp::mesh_drawable shape;
		GLuint texture_image_id;
		std::string texture_file;

	public:
		ForestRoadCornerTile(int index);
		virtual void drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;

		virtual bool canPlaceTree(float x, float y) override;
		virtual float terrainHeight(float x, float y) override;
};