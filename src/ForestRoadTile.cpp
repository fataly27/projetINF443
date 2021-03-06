#include "ForestRoadTile.hpp"

ForestRoadTile::ForestRoadTile(int index, int rotation) : TreeTile()
{
	Rotation = rotation;
	Index = index;

	if (index == 0)
	{
		Aretes[Up] = 1;
		Aretes[Down] = 1;
		Aretes[Left] = 2;
		Aretes[Right] = 2;

		mesh = cgp::mesh_load_file_obj("assets/tiles/1010/1010.obj");
		texture_file = "assets/tiles/1010/1010.png";

		mesh.fill_empty_field();
		n_trees = 40;
	}
	else if (index == 1)
	{
		Aretes[Up] = 2;
		Aretes[Down] = 1;
		Aretes[Left] = 1;
		Aretes[Right] = 2;

		mesh = cgp::mesh_load_file_obj("assets/tiles/1100/1100.obj");
		texture_file = "assets/tiles/1100/1100.png";

		mesh.fill_empty_field();
		n_trees = 40;
	}
}

void ForestRoadTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
    shape.transform.translation = position;

	if (Rotation == Right)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, -cgp::Pi / 2);
	else if (Rotation == Down)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi);
	else if (Rotation == Left)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi / 2);

	cgp::draw(shape, environment);

	TreeTile::drawTile(position, environment);
}

void ForestRoadTile::initialiseTile()
{
	TreeTile::initialiseTile();

	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    shape.initialize(mesh, "ForestRoadShape");
	shape.texture = texture_image_id;
}

void ForestRoadTile::updateTile(float dt)
{
	
}

bool ForestRoadTile::canPlaceTree(float x, float y)
{
	if (Index == 0)
		return y < 0.38f || y > 0.62f;
	else if (Index == 1)
		return (x > 0.62f && y < 0.38f) || x < 0.38f || y > 0.62f;
	else
		return false;
}
float ForestRoadTile::terrainHeight(float x, float y)
{
	return 0.f;
}