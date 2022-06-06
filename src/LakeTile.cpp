#include "LakeTile.hpp"

LakeTile::LakeTile(int index, int rotation) : TreeTile()
{
	Rotation = rotation;

	time = 0.f;

	Aretes[Up] = 1;
	Aretes[Down] = 1;
	Aretes[Left] = 1;
	Aretes[Right] = 1;
	mesh = cgp::mesh_load_file_obj("assets/tiles/lake/lac.obj");
	texture_file = "assets/tiles/lake/lake_texture.png";

	lantern_mesh = cgp::mesh_load_file_obj("assets/tiles/lake/lantern.obj");
	lantern_texture_file = "assets/tiles/lake/lantern_textureb.png";

	lantern_billboard_texture_file = "assets/tiles/lake/lantern_billboard.png";

	//mesh.fill_empty_field();
	n_trees = 80;
	n_lantern = 200;
}

void LakeTile::drawTile(cgp::vec3 position, project_scene_environment environment)
{
	shape.transform.translation = position;

	if(Rotation == Right)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, -cgp::Pi / 2);
	else if (Rotation == Down)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi);
	else if (Rotation == Left)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi / 2);

	cgp::draw(shape, environment);

	TreeTile::drawTile(position, environment);

	for (int i = 0; i < n_lantern; i++)
	{
		lantern_shape.transform.translation = position + LanternPositions[i] + LanternDisplacements[i];
		lantern_shape.transform.scaling = lantern_scale / 1.23f;
		cgp::draw(lantern_shape, environment);
	}
}

void LakeTile::drawTileTransparent(cgp::vec3 position, project_scene_environment environment)
{
	// Re-orient the grass shape to always face the camera direction
	cgp::vec3 const front = cgp::normalize(environment.camera.front() * cgp::vec3{1, 1, 1}); // front-vector of the camera with z-component
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	cgp::rotation_transform R = cgp::rotation_transform::between_vector({ 0,1,0 }, front);
	lantern_billboard.transform.rotation = R;

	for (int i = 0; i < n_lantern; i++)
	{
		lantern_billboard.transform.translation = position + LanternPositions[i] + LanternDisplacements[i];
		draw(lantern_billboard, environment);
	}
}

void LakeTile::initialiseTile()
{
	TreeTile::initialiseTile();

	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	shape.initialize(mesh, "LakeShape");
	shape.texture = texture_image_id;

	lantern_texture_image_id = cgp::opengl_load_texture_image(lantern_texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	lantern_shape.initialize(lantern_mesh, "Lantern");
	lantern_shape.texture = lantern_texture_image_id;

	lantern_billboard.initialize(cgp::mesh_primitive_quadrangle({ -0.5f, -lantern_scale / lantern_billboard_scale, -0.5f }, { 0.5f, -lantern_scale / lantern_billboard_scale, -0.5f }, { 0.5f, -lantern_scale / lantern_billboard_scale, 0.5f }, { -0.5f, -lantern_scale / lantern_billboard_scale, 0.5f }), "LanternBillboard");
	lantern_billboard.texture = cgp::opengl_load_texture_image(lantern_billboard_texture_file);
	lantern_billboard.transform.scaling = lantern_billboard_scale;
	lantern_billboard.shader = cgp::opengl_load_shader("shaders/project_billboards/vert.glsl", "shaders/project_billboards/frag.glsl");


	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float c = 0.f;

	for (int i = 0; i < n_lantern; i++)
	{
		LanternDisplacements.push_back(cgp::vec3(0.f, 0.f, 0.f));

		bool not_okay = true;
		while (not_okay)
		{
			x = cgp::rand_interval();
			y = cgp::rand_interval();
			z = cgp::rand_interval();
			c = cgp::rand_interval(0.f, std::pow(1.8f, 8));

			float d = cgp::norm(cgp::vec2(x, y) - cgp::vec2(0.5f, 0.5f));

			not_okay = std::exp(-d * d) * std::pow(cgp::noise_perlin(cgp::vec3(x, y, z), 2, 0.8f, 3.f), 8) < c;
		}

		if (Rotation == Up)
			LanternPositions.push_back(cgp::vec3(10 * x - 5.f, 10 * y - 5.f, 5.f * z + 5.f));
		else if (Rotation == Right)
			LanternPositions.push_back(cgp::vec3(10 * y - 5.f, -(10 * x - 5.f), 5.f * z + 5.f));
		else if (Rotation == Down)
			LanternPositions.push_back(cgp::vec3(-(10 * x - 5.f), -(10 * y - 5.f), 5.f * z + 5.f));
		else if (Rotation == Left)
			LanternPositions.push_back(cgp::vec3(-(10 * y - 5.f), 10 * x - 5.f, 5.f * z + 5.f));
	}
}

void LakeTile::updateTile(float dt)
{
	time += dt / 5.f;

	for (int i = 0; i < n_lantern; i++)
	{
		float x = std::cos(time + i);
		float y = std::sin(time + i);
		float z = std::cos(time + i);
		float a = std::pow(cgp::noise_perlin(LanternPositions[i], 2, 0.6f, 2.4f), 2) / 2.f;

		LanternDisplacements[i] = a * cgp::vec3(x, y, z);
	}
}

bool LakeTile::canPlaceTree(float x, float y)
{
	bool farFromLake = std::pow((x - 0.5f), 2) + std::pow((y - 0.5f), 2) > std::pow(0.3f, 2); // On n'affiche des arbres que suffisamment loin du centre (où il y a le lac)
	bool notOnRoad = std::abs(y - 0.5f) > 0.1f || x < 0.8f; //On n'affiche que des arbres qui ne sont pas sur la route

	return farFromLake && notOnRoad;
}
float LakeTile::terrainHeight(float x, float y)
{
	return 0.f;
}