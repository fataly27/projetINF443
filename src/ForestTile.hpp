#pragma once

#include "TreeTile.hpp"
#include "cgp/cgp.hpp"
#include <string>
#include "ProjectEnvironment.hpp"


class ForestTile : public TreeTile
{
	private:
		cgp::mesh terrain_mesh;
		cgp::mesh_drawable terrain_shape;

	public:
		ForestTile(int index);
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;

		virtual bool canPlaceTree(float x, float y) override;
		virtual float terrainHeight(float x, float y) override;

		cgp::mesh create_terrain_mesh(int n, float terrain_length);
};