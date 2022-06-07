#include "TreeTile.hpp"

TreeTile::TreeTile() : Tile()
{
	tree_mesh = cgp::mesh_load_file_obj("assets/tiles/tree.obj");
	tree_texture_file = "assets/tiles/tree_texture.png";

	n_trees = 20;
}

void TreeTile::initialiseTile()
{
	tree_texture_image_id = cgp::opengl_load_texture_image(tree_texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	tree_shape.initialize(tree_mesh, "TreeShape");
	tree_shape.texture = tree_texture_image_id;

	for (int i = 0; i < n_trees; i++)
	{
		float x = 0.f;
		float y = 0.f;

		bool not_okay = true;
		while (not_okay)
		{
			x = cgp::rand_interval();
			y = cgp::rand_interval();

			not_okay = !canPlaceTree(x, y);
		}

		if (Rotation == Up)
			TreePositions.push_back(cgp::vec3(10 * x - 5.f, 10 * y - 5.f, terrainHeight(x, y) - 0.15f));
		else if(Rotation == Right)
			TreePositions.push_back(cgp::vec3(10 * y - 5.f, -(10 * x - 5.f), terrainHeight(x, y) - 0.15f));
		else if (Rotation == Down)
			TreePositions.push_back(cgp::vec3(-(10 * x - 5.f), -(10 * y - 5.f), terrainHeight(x, y) - 0.15f));
		else if (Rotation == Left)
			TreePositions.push_back(cgp::vec3(-(10 * y - 5.f), 10 * x - 5.f, terrainHeight(x, y) - 0.15f));
	}
}

void TreeTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
	if (cgp::norm(position - environment.camera.position()) < 60.f && cgp::dot(environment.camera.front(), position - environment.camera.position()) > 0.f)
	{
		for (int i = 0; i < n_trees; i++)
		{
			tree_shape.transform.translation = position + TreePositions[i];
			//tree_shape.transform.scaling = 0.1;
			cgp::draw(tree_shape, environment);
		}
	}
}