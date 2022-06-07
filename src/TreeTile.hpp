#pragma once

#include <array>
#include <vector>
#include <string>
#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"
#include "tile.hpp"

class TreeTile : public Tile
{
	protected:
		cgp::mesh tree_mesh;
		cgp::mesh_drawable tree_shape;
		GLuint tree_texture_image_id;
		std::string tree_texture_file;

		int n_trees;
		std::vector<cgp::vec3> TreePositions;

	public:
		TreeTile();

		virtual void drawTile(cgp::vec3 position, project_scene_environment environment, int width = 0, int height = 0) override;
		virtual void initialiseTile() override;

		virtual bool canPlaceTree(float x, float y) = 0;
		virtual float terrainHeight(float x, float y) = 0;
};