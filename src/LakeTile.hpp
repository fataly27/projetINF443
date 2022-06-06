#pragma once

#include "TreeTile.hpp"
#include "cgp/cgp.hpp"
#include <string>
#include "ProjectEnvironment.hpp"


class LakeTile : public TreeTile
{
	private:
		float time;

		cgp::mesh mesh;
		cgp::mesh_drawable shape;
		GLuint texture_image_id;
		std::string texture_file;

		//Lanterns
		cgp::mesh lantern_mesh;
		cgp::mesh_drawable lantern_shape;
		GLuint lantern_texture_image_id;
		std::string lantern_texture_file;
		float lantern_scale = 0.123f;

		cgp::mesh_drawable lantern_billboard;
		std::string lantern_billboard_texture_file;
		float lantern_billboard_scale = 1.f;

		int n_lantern;
		std::vector<cgp::vec3> LanternPositions;
		std::vector<cgp::vec3> LanternDisplacements;

	public:
		LakeTile(int index, int rotation = Up);
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment) override;
		virtual void drawTileTransparent(cgp::vec3 position, project_scene_environment environment) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;

		virtual bool canPlaceTree(float x, float y) override;
		virtual float terrainHeight(float x, float y) override;
};