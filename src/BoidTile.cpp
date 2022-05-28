#include "BoidTile.hpp"

BoidTile::BoidTile(int) : Tile()
{
	Aretes[Up] = 0;
	Aretes[Down] = 0;
	Aretes[Left] = 0;
	Aretes[Right] = 0;
	mesh_green = cgp::mesh_load_file_obj("assets/tiles/0000/green.obj");
	mesh_green.fill_empty_field();


	float walls[6] = { -5,5,-5,5,0,5 };
	Boid A(1);
	A.setWalls(walls);
	for (int i = 0; i < NBoids; i++) {
		B[i] = A;
		B[i].setPosition({ i / 100 ,2.5,2.5 });
		B[i].setVelocity({ 0.1,1,0.1 });
		std::array<Boid*, Boid::max> others;
		int k = 0;
		for (int j = 0; j < NBoids; j++) {
			if (j != i) {
				others[k] = &B[j];
				k++;
			}
		}
		B[i].setOthers(others, NBoids - 1);
	}
}

void BoidTile::drawTile(cgp::vec3 position, scene_environment_with_multiple_lights environment)
{
	shape_green.transform.translation = position;
	cgp::draw(shape_green, environment);

	for (int i = 0; i < NBoids; i++) {
		B[i].drawBoid(environment, position);
	}
}

void BoidTile::initialiseTile()
{
	shape_green.initialize(mesh_green, "Shape_green");
	shape_green.shading.color = { 0, 0.2, 0 };

	for (int i = 0; i < NBoids; i++) {
		B[i].initialize();
	}
}

void BoidTile::updateTile(float dt)
{
	for (int i = 0; i < NBoids; i++) {
		B[i].applyForces();
	}
	for (int i = 0; i < NBoids; i++) {
		B[i].update(dt);
	}
}
