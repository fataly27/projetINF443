#include "BoidTile.hpp"

BoidTile::BoidTile(int) : Tile()
{
	Aretes[Up] = 0;
	Aretes[Down] = 0;
	Aretes[Left] = 0;
	Aretes[Right] = 0;

	mesh_green = cgp::mesh_load_file_obj("assets/tiles/0000/0000.obj");
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

void BoidTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
	shape_green.transform.translation = position;
	cgp::draw(shape_green, environment);


	cgp::vec3 camPos = environment.camera.position();
	if (cgp::norm(position - camPos) >= 50 || (cgp::dot(environment.camera.front(), position - environment.camera.position()) < 0.f && cgp::norm(position - camPos) > 10)) return;
	if (cgp::norm(position - camPos) > 30) {
		for (int i = 0; i < NBoids/2; i++)
			B[i].drawBoid(environment, position);
		return;
	}
	for (int i = 0; i < NBoids; i++) {
		
		B[i].drawBoid(environment, position);
	}
}

void BoidTile::initialiseTile()
{
	shape_green.initialize(mesh_green, "Shape_green");
	shape_green.texture = cgp::opengl_load_texture_image("assets/tiles/0000/0000.png", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	B[0].initialize();
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
