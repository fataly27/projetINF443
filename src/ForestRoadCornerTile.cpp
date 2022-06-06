#include "ForestRoadCornerTile.hpp"

ForestRoadCornerTile::ForestRoadCornerTile(int index) : TreeTile()
{
	Aretes[Up] = 1;
	Aretes[Down] = 1;
	Aretes[Left] = 1;
	Aretes[Right] = 1;

	mesh = cgp::mesh_load_file_obj("assets/tiles/forest_road_corner/forest_road_corner.obj");
	texture_file = "assets/tiles/forest_road_corner/forest_road_corner_texture.png";

	//mesh.fill_empty_field();
	n_trees = 150;
}

void ForestRoadCornerTile::drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment)
{
    shape.transform.translation = position;
	cgp::draw(shape, environment);

	TreeTile::drawTile(position, environment);
}

void ForestRoadCornerTile::initialiseTile()
{
	TreeTile::initialiseTile();

	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    shape.initialize(mesh, "ForestRoadCornerShape");
	shape.texture = texture_image_id;
}

void ForestRoadCornerTile::updateTile(float dt)
{
	
}

bool ForestRoadCornerTile::canPlaceTree(float x, float y)
{
	return (x > 0.62f && y < 0.38f) || x < 0.38f || y > 0.62f;
}
float ForestRoadCornerTile::terrainHeight(float x, float y)
{
	return 0.f;
}