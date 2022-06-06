#include "tile.hpp"

Tile::Tile()
{
	Aretes = std::array<int, 4>();
	Rotation = Up;
}

bool Tile::isNeighbor(Tile* voisin, int direction)
{
	return voisin->getProximity((direction + 2) % 4) == getProximity(direction);
}

void Tile::updateTile(float)
{
	return;
}

int Tile::getProximity(int dir)
{
	return Aretes[(dir + 4 - Rotation) % 4];
}

void Tile::drawTileTransparent(cgp::vec3 position, project_scene_environment environment)
{

}