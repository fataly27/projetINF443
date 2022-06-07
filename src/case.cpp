#include "case.hpp"
#include "2dtile.hpp"
#include "BoidTile.hpp"
#include "FountainTile.hpp"
#include "LakeTile.hpp"
#include <cstdlib>
#include "buildingTile.hpp"
#include "ForestTile.hpp"
#include "ForestRoadTile.hpp"
#include "LakeTile.hpp"

std::array<Tile*, N> Case::Tiles = {
	new BuildingTile(1000, 0), new BuildingTile(1000, 1), new BuildingTile(1000, 2), new BuildingTile(1000, 3) ,
	new BuildingTile(0111, 0), new BuildingTile(0111, 1), new BuildingTile(0111, 2), new BuildingTile(0111, 3) ,
	new BuildingTile(1010, 0), new BuildingTile(1010, 1), new BuildingTile(1010, 2), new BuildingTile(1010, 3) ,
	new BuildingTile(1100, 0), new BuildingTile(1100, 1), new BuildingTile(1100, 2), new BuildingTile(1100, 3) ,
	new ForestTile(0),
	new ForestRoadTile(0,0),new ForestRoadTile(0,1),new ForestRoadTile(0,2),new ForestRoadTile(0,3),
	new ForestRoadTile(1,0),new ForestRoadTile(1,1),new ForestRoadTile(1,2),new ForestRoadTile(1,3),
	new LakeTile(0,0), new LakeTile(0,1),new LakeTile(0,2),new LakeTile(0,3),
	new BoidTile(0), new FountainTile(0)
};

void Case::initialiseTiles()
{
	for (int i = 0; i < N; i++)
		Tiles[i]->initialiseTile();
}

cgp::vec3 Case::getPosition()
{
	return Position;
}

std::array<Case*, 4> Case::getNeighbors()
{
	return Neighbors;
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

int Case::getFixedTile()
{
	return FixedTile;
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
			TilesDispo[i] = (Neighbors[Up] == nullptr || Neighbors[Up]->isNeighborCase(Tiles[i], Down)) && (Neighbors[Down] == nullptr || Neighbors[Down]->isNeighborCase(Tiles[i], Up)) && (Neighbors[Right] == nullptr || Neighbors[Right]->isNeighborCase(Tiles[i], Left)) && (Neighbors[Left] == nullptr || Neighbors[Left]->isNeighborCase(Tiles[i], Right));
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

void Case::updateCase(float dt)
{
	for (int i = 0; i < N; i++)
		Tiles[i]->updateTile(dt);
}

bool Case::isNeighborCase(Tile* tile, int dir)
{
	for (int i = 0; i < N; i++)
	{
		if (TilesDispo[i])
		{
			if (Tiles[i]->isNeighbor(tile, dir))
				return true;
		}
	}
	return false;
}
bool Case::isCaseFixed()
{
	return isFixed;
}


void Case::drawCase(project_scene_environment environment)
{
	if (isFixed)
		Tiles[FixedTile]->drawTile(Position, environment);
}
void Case::drawCaseTransparent(project_scene_environment environment)
{
	if (isFixed)
		Tiles[FixedTile]->drawTileTransparent(Position, environment);
}
