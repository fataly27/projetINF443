#include "3dtile.hpp"

VolumeTile::VolumeTile(int type) : Tile()
{
	int dir = Up;

	switch (type)
	{
		case 0:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			texture_file = "imgs/texture0.png";
			break;
		case 1:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			texture_file = "imgs/texture1.png";
			dir = Left;
			break;
		case 2:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/texture1.png";
			dir = Up;
			break;
		case 3:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			texture_file = "imgs/texture1.png";
			dir = Right;
			break;
		case 4:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/texture1.png";
			dir = Down;
			break;
		case 5:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/texture2.png";
			dir = Left;
			break;
		case 6:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/texture2.png";
			dir = Down;
			break;
		case 7:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			texture_file = "imgs/texture3.png";
			dir = Left;
			break;
		case 8:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			texture_file = "imgs/texture3.png";
			dir = Up;
			break;
		case 9:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/texture3.png";
			dir = Right;
			break;
		case 10:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/texture3.png";
			dir = Down;
			break;
		case 11:
			Aretes[Up] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			Aretes[Right] = 0;
			texture_file = "imgs/texture4.png";
			dir = Left;
			break;
		case 12:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			texture_file = "imgs/texture4.png";
			dir = Up;
			break;
		case 13:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/texture4.png";
			dir = Right;
			break;
		case 14:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/texture4.png";
			dir = Down;
			break;
		case 15:
			Aretes[Up] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			Aretes[Right] = 1;
			texture_file = "imgs/texture5.png";
			break;
	}

	quadrangle_mesh.position = { {0.5f,-0.5f,0}, { -0.5f,-0.5f,0}, { -0.5f, 0.5f,0}, {0.5f, 0.5f,0} };
	quadrangle_mesh.connectivity = { {0,1,2}, {0,2,3} };

	switch (dir)
	{
		case Up:
			quadrangle_mesh.uv = { {1,0}, {0,0}, {0,1} , {1,1}, };
			break;
		case Right:
			quadrangle_mesh.uv = { {1,1},{1,0},{0,0},{0,1}, };
			break;
		case Down:
			quadrangle_mesh.uv = { {0,1},{1,1},{1,0},{0,0}, };
			break;
		case Left:
			quadrangle_mesh.uv = { {0,0},{0,1},{1,1},{1,0}, };
			break;
	}

	quadrangle_mesh.fill_empty_field();
}

void VolumeTile::initialiseTile()
{
	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	shape.initialize(quadrangle_mesh, "Shape");
	shape.texture = texture_image_id;
}

void VolumeTile::drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment)
{
	shape.transform.translation = position;
	cgp::draw(shape, environment);
}