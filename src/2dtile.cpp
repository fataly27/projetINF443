#include "2dtile.hpp"

TextureTile::TextureTile(int type) : Tile()
{
	int dir = Up;

	switch (type)
	{
		case 0:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			texture_file = "imgs/0000.png";
			break;
		case 1:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			texture_file = "imgs/0011.png";
			dir = Left;
			break;
		case 2:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/0011.png";
			dir = Up;
			break;
		case 3:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			texture_file = "imgs/0011.png";
			dir = Right;
			break;
		case 4:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/0011.png";
			dir = Down;
			break;
		case 5:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/0101.png";
			dir = Left;
			break;
		case 6:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/0101.png";
			dir = Down;
			break;
		case 7:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			texture_file = "imgs/0111.png";
			dir = Left;
			break;
		case 8:
			Aretes[Up] = 1;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			texture_file = "imgs/0111.png";
			dir = Up;
			break;
		case 9:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/0111.png";
			dir = Right;
			break;
		case 10:
			Aretes[Up] = 1;
			Aretes[Right] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/0111.png";
			dir = Down;
			break;
		case 11:
			Aretes[Up] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			Aretes[Right] = 0;
			texture_file = "imgs/1000.png";
			dir = Left;
			break;
		case 12:
			Aretes[Up] = 0;
			Aretes[Right] = 1;
			Aretes[Down] = 0;
			Aretes[Left] = 0;
			texture_file = "imgs/1000.png";
			dir = Up;
			break;
		case 13:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 1;
			Aretes[Left] = 0;
			texture_file = "imgs/1000.png";
			dir = Right;
			break;
		case 14:
			Aretes[Up] = 0;
			Aretes[Right] = 0;
			Aretes[Down] = 0;
			Aretes[Left] = 1;
			texture_file = "imgs/1000.png";
			dir = Down;
			break;
		case 15:
			Aretes[Up] = 1;
			Aretes[Down] = 1;
			Aretes[Left] = 1;
			Aretes[Right] = 1;
			texture_file = "imgs/1111.png";
			break;
	}

	quadrangle_mesh.position = { {0.5f,-0.5f,0}, { -0.5f,-0.5f,0}, { -0.5f, 0.5f,0}, {0.5f, 0.5f,0} };
	quadrangle_mesh.position *= 10;
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

void TextureTile::initialiseTile()
{
	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	shape.initialize(quadrangle_mesh, "Shape");
	shape.texture = texture_image_id;
}

void TextureTile::drawTile(cgp::vec3 position, project_scene_environment environment)
{
	shape.transform.translation = position;
	cgp::draw(shape, environment);
}