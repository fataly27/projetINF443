#include "Boid.hpp"

//variables to set
float Boid::factors[6] = { 10, 1, 1, 1, 1, 1 };
float Boid::proximity[3] = { 0.3, 0.5, 0.7 };
float Boid::v = 2;
  

cgp::mesh body_mesh_in;
cgp::mesh_drawable body_in;

cgp::mesh* Boid::body_mesh = &body_mesh_in;
cgp::mesh_drawable* Boid::body = &body_in;


float len(cgp::vec3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float dist(cgp::vec3 u, cgp::vec3 v) {
	return len(u - v);
}

Boid::Boid(cgp::vec3 position)
{
	this->position = position;
	velocity = { 1,0.1,0.2 };
}

Boid::Boid(int)
{
	position = { 0.5,0.5,0.5 };
	velocity = { 1,0.1,0.2 };
}

void Boid::setWalls(float walls[6])
{
	for (int i = 0; i < 6; i++) {
		this->walls[i] = walls[i];
	}
}

void Boid::setPosition(cgp::vec3 position)
{
	this->position = position;
}

void Boid::setVelocity(cgp::vec3 velocity)
{
	this->velocity = velocity;
}

void Boid::setOthers(std::array<Boid *, max> others, int nbOthers)
{
	this->others = others;
	this->nbOthers = nbOthers;
}

cgp::vec3 Boid::getPosition()
{
	return position;
}

void Boid::setBodyMesh(cgp::mesh* body_meshIn)
{
	body_mesh = body_meshIn;
}

void Boid::setBody(cgp::mesh_drawable* bodyIn)
{
	body = bodyIn;
}

void Boid::bounceWalls()
{
	if (position.x < walls[0]) {
		velocity.x = std::abs(velocity.x);
	}
	if (position.x > walls[1]) {
		velocity.x = -std::abs(velocity.x);
	}
	if (position.y < walls[2]) {
		velocity.y = std::abs(velocity.y);
	}
	if (position.y > walls[3]) {
		velocity.y = -std::abs(velocity.y);
	}
	if (position.z < walls[4]) {
		velocity.z = std::abs(velocity.z);
	}
	if (position.z > walls[5]) {
		velocity.z = -std::abs(velocity.z);
	}


	if (position.x < walls[0] -1) {
		velocity.x = 10;
	}
	if (position.x > walls[1] + 1) {
		velocity.x = -10;
	}
	if (position.y < walls[2] -1 ) {
		velocity.y = 10;
	}
	if (position.y > walls[3] + 1) {
		velocity.y = -10;
	}
	if (position.z < walls[4] -1) {
		velocity.z = 10;
	}
	if (position.z > walls[5] + 1) {
		velocity.z = -10;
	}
}

cgp::vec3 Boid::avoidWalls()
{
	float detDist = 3;

	cgp::vec3 res = { 0,0,0 };

	if (position.x < walls[0] + detDist) {
		float dist = (position.x - walls[0]);
		if (dist != 0) res.x = 1 / dist - 1/detDist;
	}
	if (position.x > walls[1] - detDist) {
		float dist = (walls[1] - position.x);
		if (dist != 0) res.x = -1 / dist + 1 / detDist;
	}
	if (position.y < walls[2] + detDist) {
		float dist = (position.y - walls[2]);
		if (dist != 0) res.y = 1 / dist - 1 / detDist;
	}
	if (position.y > walls[3] - detDist) {
		float dist = (walls[3] - position.y);
		if (dist != 0) res.y = -1 / dist + 1 / detDist;
	}
	if (position.z < walls[4] + detDist) {
		float dist = (position.z - walls[4]);
		if (dist != 0) res.z = 1 / dist - 1 / detDist;
	}
	if (position.z > walls[5] - detDist) {
		float dist = (walls[5] - position.z);
		if (dist != 0) res.z = -1 / dist + 1 / detDist;
	}
	return res;
}

cgp::vec3 Boid::avoidOthers()
{

	cgp::vec3 res = { 0,0,0 };

	for (int i = 0; i < nbOthers; i++) {
		float distance = dist(others[i]->position, position);
		if (distance < proximity[0] && distance != 0) {
			res += (1 / (distance * distance) - 1/(proximity[0]*proximity[0])) * (position - others[i]->position);
		}
	}
	return res;
}

cgp::vec3 Boid::flyTowardOthers()
{

	cgp::vec3 center = { 0,0,0 };
	int nb = 0;
	for (int i = 0; i < nbOthers; i++) {
		float distance = dist(others[i]->position, position);
		if (distance < proximity[2]) {
			center += (others[i]->position);
			nb++;
		}
	}
	cgp::vec3 res = { 0,0,0 };


	if (nb != 0) {
		center /= nb;
		res = center - position;
	}
	return res;
}

cgp::vec3 Boid::alignWithOthers() {
	cgp::vec3 v = { 0,0,0 };
	int nb = 0;
	for (int i = 0; i < nbOthers; i++) {
		float distance = dist(others[i]->position, position);
		if (distance < proximity[1]) {
			v += (others[i]->velocity);
			nb++;
		}
	}
	if (nb != 0) {
		v /= nb;
	}
	return v;
}

cgp::vec3 Boid::flyRandom()
{
	float x = rand() % 100;
	float y = rand() % 100;
	float z = rand() % 100;

	x = x/100 -0.5;
	y = y/100 -0.5;
	z = z/100 -0.5;

	return cgp::vec3({x,y,z});
}

void Boid::applyForces()
{
	velocity = velocity * factors[0] 
		+ avoidWalls() * factors[1] 
		+ avoidOthers() * factors[2] 
		+ alignWithOthers() * factors[3]
		+ flyTowardOthers() * factors[4]
		+ flyRandom() * factors[5];
	/*
	if (len(velocity) < vMin) {
		velocity *= vMin / len(velocity);
	}
	if (len(velocity) > vMax) {
		velocity *= vMax / len(velocity);
	}
	*/
	if (len(velocity) == 0) velocity = { 0,0,1 };
	velocity *= v / len(velocity);
	bounceWalls();
}



void Boid::update(float dt)
{
	position += dt * velocity;
}

void Boid::initialize()
{
	body_mesh->position = { {1, 0, 0}, {0,0.4,-0.3}, {0,-0.4,-0.3}, {0,0,0.3} };
	body_mesh->position *= 0.1;
	body_mesh->connectivity = { {0,1,2}, {0,2,3}, {0,3,1}, {1,3,2} };
	body_mesh->fill_empty_field();

	body->initialize(*body_mesh, "Body");
	body->shading.color = { 0.5f,0.5f,1.0f };
	std::cout << "ok";
}

void Boid::drawBoid(project_scene_environment environment, cgp::vec3 offset)
{
	body->transform.translation = position + offset;
	cgp::vec3 v = velocity;
	float lenV = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v /= lenV;
	body->transform.rotation = cgp::rotation_transform::between_vector({1,0,0}, v);
	cgp::draw(*body, environment);
}
