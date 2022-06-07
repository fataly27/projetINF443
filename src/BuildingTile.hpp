#pragma once

#include "tile.hpp"
#include "ProjectEnvironment.hpp"
#include "cgp/cgp.hpp"
#include "building.hpp"

class BuildingTile : public Tile
{
	public :
		BuildingTile(int index, int rotation = Up);
		virtual void initialiseTile() override;
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment) override;
	
	private :
		House house;

		cgp::mesh ground_mesh;
		cgp::mesh_drawable ground;
		GLuint texture_image_id;
		std::string texture_file;

		cgp::vec3 batPos;
		float angle;

		int Index = 0;
}; 