#include "case.hpp"
#include <cstdlib>

std::array<Tile, N> Case::Tiles = { Tile(0), Tile(1), Tile(2), Tile(3), Tile(4), Tile(5), Tile(6), Tile(7), Tile(8), Tile(9), Tile(10), Tile(11), Tile(12), Tile(13), Tile(14), Tile(15) };

void Case::initialiseTiles()
{
	for (int i = 0; i < N; i++)
		Tiles[i].initialiseTexture();
}

void Case::fixTile()
{
	int remaining[N];
	int j = 0;
	for (int i = 0; i < N && j < nbDispos; i++)
	{
		if (TilesDispo[i])
		{
			remaining[j] = i;
			j++;
		}
	}
	int fix = remaining[rand() % nbDispos];
	for (int i = 0; i < N; i++)
	{
		if (i != fix)
			TilesDispo[i] = false;
	}

	nbDispos = 1;
	isFixed = true;
	FixedTile = fix;

	if(Neighbors[Up] != nullptr)
		Neighbors[Up]->update();
	if (Neighbors[Down] != nullptr)
		Neighbors[Down]->update();
	if (Neighbors[Right] != nullptr)
		Neighbors[Right]->update();
	if (Neighbors[Left] != nullptr)
		Neighbors[Left]->update();
}


Case::Case(cgp::vec3 position)
{
	Neighbors = std::array<Case*, 4>();
	TilesDispo = std::array<bool, N>();
	TilesDispo.fill(true);
	Position = position;
}

void Case::setNeighbor(Case* voisin, int direction)
{
	Neighbors[direction] = voisin;
}

int Case::getNbDispos()
{
	return nbDispos;
}

void Case::update()
{
	bool changed = false;
	if (isFixed)
		return;

	for (int i = 0; i < N; i++)
	{
		if (TilesDispo[i])
		{
			TilesDispo[i] = (Neighbors[Up] == nullptr || Neighbors[Up]->isNeighborCase(&Tiles[i], Down)) && (Neighbors[Down] == nullptr || Neighbors[Down]->isNeighborCase(&Tiles[i], Up)) && (Neighbors[Right] == nullptr || Neighbors[Right]->isNeighborCase(&Tiles[i], Left)) && (Neighbors[Left] == nullptr || Neighbors[Left]->isNeighborCase(&Tiles[i], Right));
			if (!TilesDispo[i])
			{
				changed = true;
				nbDispos--;
			}
		}
	}
	if (changed)
	{
		if (nbDispos == 1)
		{
			isFixed = true;
			for (int i = 0; i < N; i++)
			{
				if (TilesDispo[i])
					FixedTile = i;
			}
		}

		if (Neighbors[Up] != nullptr)
			Neighbors[Up]->update();
		if (Neighbors[Down] != nullptr)
			Neighbors[Down]->update();
		if (Neighbors[Right] != nullptr)
			Neighbors[Right]->update();
		if (Neighbors[Left] != nullptr)
			Neighbors[Left]->update();
	}
}

bool Case::isNeighborCase(Tile* tile, int dir)
{
	for (int i = 0; i < N; i++)
	{
		if (TilesDispo[i])
		{
			if (Tiles[i].isNeighbor(tile, dir))
				return true;
		}
	}
	return false;
}
bool Case::isCaseFixed()
{
	return isFixed;
}


void Case::drawCase(scene_environment_with_multiple_lights environment)
{
	if (isFixed)
		Tiles[FixedTile].drawTile(Position, environment);
}
