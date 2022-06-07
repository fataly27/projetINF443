#include "BuildingTile.hpp"

BuildingTile::BuildingTile(int index, int rotation)
{
	Rotation = rotation;
	Index = index;

	switch (index)
	{
		case 1000:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			ground_mesh = cgp::mesh_load_file_obj("assets/tiles/1000/1000.obj");
			texture_file = "assets/tiles/1000/1000.png";
			batPos = { 0.6,-2.5,0 };
			angle = cgp::rand_interval(cgp::Pi * 7 / 8, cgp::Pi * 9 / 8);
			break;
		case 0111:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			ground_mesh = cgp::mesh_load_file_obj("assets/tiles/0111/0111.obj");
			texture_file = "assets/tiles/0111/0111.png";
			batPos = { 0.2,2.5,0 };
			angle = cgp::rand_interval(-cgp::Pi * 1 / 8, cgp::Pi * 1 / 8);
			break;
		case 1010:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			ground_mesh = cgp::mesh_load_file_obj("assets/tiles/1010/1010.obj");
			texture_file = "assets/tiles/1010/1010.png";
			batPos = { -2,-4,0 };
			angle = cgp::rand_interval(cgp::Pi * 3/ 8, cgp::Pi * 5/ 8);
			break;
		case 1100:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			ground_mesh = cgp::mesh_load_file_obj("assets/tiles/1100/1100.obj");
			texture_file = "assets/tiles/1100/1100.png";
			batPos = { -1,-3,0 };
			angle = cgp::rand_interval(cgp::Pi * 5 / 8, cgp::Pi * 7 / 8);
			break;
	}

}

void BuildingTile::initialiseTile()
{
	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	ground.initialize(ground_mesh, "Ground");
	ground.texture = texture_image_id;

	house.initializeHouse();
}

void BuildingTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
	ground.transform.translation = position;

	cgp::vec3 batPos_temp = batPos;
	float angle_temp = angle;
	cgp::rotation_transform R = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, -cgp::Pi / 2);
	if (Rotation == Right)
	{
		ground.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, -cgp::Pi / 2);
		batPos_temp = R * batPos;
		angle_temp = angle - cgp::Pi / 2;
	}
	else if (Rotation == Down)
	{
		ground.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi);
		batPos_temp = R * R * batPos;
		angle_temp = angle + cgp::Pi;
	}
	else if (Rotation == Left)
	{
		ground.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi / 2);
		batPos_temp = R*R*R * batPos;
		angle_temp = angle + cgp::Pi / 2;
	}
	house.drawHouse(environment, position + batPos_temp, angle_temp);
	cgp::draw(ground, environment);


}
