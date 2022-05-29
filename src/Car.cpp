#include "Car.hpp"


Car::Car()
{
}

Car::Car(Case* casePos)
{
	this->casePos = casePos;
	mesh_car = cgp::mesh_load_file_obj("assets/car/CAR.obj");
	mesh_tires = cgp::mesh_load_file_obj("assets/car/TIRES.obj");
	mesh_car.position *= 0.3;
	mesh_tires.position *= 0.3;
}

void Car::initializeCar()
{
	shape_car.initialize(mesh_car, "Car");
	shape_tires.initialize(mesh_tires, "Tires");
	shape_car.shading.color = { 0,1,0 };
	shape_tires.shading.color = { 0,0,0 };
}

cgp::vec3 vecDir[4] = { {-5,0,0}, {0,5,0}, {5,0,0}, {0,-5,0} };

void Car::updateCar(float dt)
{
	timeCount += dt;
	if (timeCount < waitTime / 2) {
		p = vecDir[dirIn] *2*(waitTime/2-timeCount)/waitTime;
		v = -vecDir[dirIn] / 5;
		return;
	}
	if (timeCount < waitTime) {
		p = vecDir[dirOut]* (- 2) * ((waitTime - timeCount) / waitTime - 0.5);
		v = vecDir[dirOut] / 5;
		return;
	}
	timeCount = 0;
	casePos = casePos->getNeighbors()[dirOut];
	
	dirIn = (dirOut+2) %4;
	dirOut = nextTile();
	p = vecDir[dirIn];
}

int Car::nextTile()
{
	std::array<Case*, 4> Neighbors = casePos->getNeighbors();
	Tile* tilePos = Case::Tiles[casePos->getFixedTile()];
	int dir;
	do {
		dir = rand() % 4;
	} while (tilePos->getProximity(dir) != 1 || Neighbors[dir] == NULL);
	return dir;
}



void Car::drawCar(scene_environment_with_multiple_lights environment)
{
	shape_car.transform.rotation = cgp::rotation_transform::between_vector({ 0,1,0 }, v);
	shape_tires.transform.rotation = cgp::rotation_transform::between_vector({ 0,1,0 }, v);
	shape_car.transform.translation = casePos->getPosition() + p;
	shape_tires.transform.translation = casePos->getPosition() + p;
	cgp::draw(shape_tires, environment);
	cgp::draw(shape_car, environment);
}
