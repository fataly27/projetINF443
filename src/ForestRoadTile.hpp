#pragma once

#include "TreeTile.hpp"
#include "cgp/cgp.hpp"
#include <string>
#include "ProjectEnvironment.hpp"


class ForestRoadTile : public TreeTile
{
	private:
		cgp::mesh mesh;
		cgp::mesh_drawable shape;
		GLuint texture_image_id;
		std::string texture_file;

		int Index = 0;

	public:
		ForestRoadTile(int index, int rotation);
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment, int width = 0, int height = 0) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;

		virtual bool canPlaceTree(float x, float y) override;
		virtual float terrainHeight(float x, float y) override;
};