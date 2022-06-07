#include "FountainTile.hpp"

FountainTile::FountainTile(int index) : Tile()
{
	Aretes[Up] = 1;
	Aretes[Down] = 1;
	Aretes[Left] = 1;
	Aretes[Right] = 1;
	mesh_black = cgp::mesh_load_file_obj("assets/tiles/1111_fountain/black.obj");
	mesh_blue = cgp::mesh_load_file_obj("assets/tiles/1111_fountain/blue.obj");
	mesh_green = cgp::mesh_load_file_obj("assets/tiles/1111_fountain/green.obj");
	mesh_grey = cgp::mesh_load_file_obj("assets/tiles/1111_fountain/grey.obj");

	mesh_black.fill_empty_field();
	mesh_blue.fill_empty_field();
	mesh_green.fill_empty_field();
	mesh_grey.fill_empty_field();

}

void FountainTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
	shape_black.transform.translation = position;
	shape_blue.transform.translation = position;
	shape_green.transform.translation = position;
	shape_grey.transform.translation = position;
	cgp::draw(shape_black, environment);
	cgp::draw(shape_blue, environment);
	cgp::draw(shape_green, environment);
	cgp::draw(shape_grey, environment);

	cgp::vec3 camPos = environment.camera.position();
	if (cgp::norm(position - camPos) >= 50) return;
	for (particle_structure& particle : particles)
	{
		sphere.transform.translation = particle.p + position;
		draw(sphere, environment);
	}
}

void FountainTile::initialiseTile()
{

	sphere.initialize(cgp::mesh_primitive_sphere(sphere_radius), "sphere");
	sphere.shading.color = { 0.4f, 0.4f, 1.0f };

	shape_black.initialize(mesh_black, "Shape_black");
	shape_blue.initialize(mesh_blue, "Shape_blue");
	shape_green.initialize(mesh_green, "Shape_green");
	shape_grey.initialize(mesh_grey, "Shape_grey");
	shape_black.shading.color = { 0, 0, 0 };
	shape_blue.shading.color = { 0.3f, 0.3f, 1 };
	shape_green.shading.color = { 0.3f, 0.8f, 0.4f };
	shape_grey.shading.color = { 0.5f, 0.5f, 0.5f };

}

void FountainTile::updateTile(float dt)
{
	timeCount += dt;
	if (timeCount > waitNewSphere) {
		timeCount = 0;
		cgp::vec3 p0 = { 0,0,0 };
		float theta = cgp::rand_interval(0, 2 * cgp::Pi);
		float z = cgp::rand_interval(4, 6);
		cgp::vec3 v0 = { std::cos(theta), std::sin(theta), z };

		particles.push_back({ p0,v0 });
	}

	for (particle_structure& particle : particles)
	{
		cgp::vec3& p = particle.p;
		cgp::vec3& v = particle.v;

		cgp::vec3 F = m * g;

		v = v + dt * F / m;
		p = p + dt * v;
	}

	for (auto it = particles.begin(); it != particles.end(); ) {
		if (it->p.z < -1)
			it = particles.erase(it);
		if (it != particles.end())
			++it;
	}

}
