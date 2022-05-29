#include "tile.hpp"

Tile::Tile()
{
	Aretes = std::array<int, 4>();
}

bool Tile::isNeighbor(Tile* voisin, int direction)
{
	return voisin->Aretes[(direction + 2) % 4] == Aretes[direction];
}

void Tile::updateTile(float)
{
	return;
}

int Tile::getProximity(int dir)
{
	return Aretes[dir];
}
