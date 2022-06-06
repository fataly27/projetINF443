#include "Car.hpp"



cgp::vec3 cardinalSplineInterpolation(float t, float t0, float t1, float t2, float t3, cgp::vec3 const& p0, cgp::vec3 const& p1, cgp::vec3 const& p2, cgp::vec3 const& p3, float K = 1) {
	float s = (t - t1) / (t2 - t1);
	cgp::vec3 d1 = 2 * K * (p2 - p0) / (t2 - t0);
	cgp::vec3 d2 = 2 * K * (p3 - p1) / (t3 - t1);
	cgp::vec3 p = (2 * s * s * s - 3 * s * s + 0 * s + 1) * p1
		+ (1 * s * s * s - 2 * s * s + 1 * s + 0) * d1
		+ (-2 * s * s * s + 3 * s * s + 0 * s + 0) * p2
		+ (1 * s * s * s - 1 * s * s + 0 * s + 0) * d2;
	return p;
}


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
	float K = 1;
	timeCount += dt;
	if (timeCount < waitTime / 2) {
		cgp::vec3 pp = p;
		p = cardinalSplineInterpolation(timeCount, -waitTime/2, 0, waitTime / 2, waitTime, 2*vecDir[dirIn], vecDir[dirIn], { 0,0,0 }, vecDir[dirOut], K);
		v = cgp::normalize(p-pp);
		return;
	}
	if (timeCount < waitTime) {

		cgp::vec3 pp = p;
		p = cardinalSplineInterpolation(timeCount, 0, waitTime / 2, waitTime, waitTime *3/2, vecDir[dirIn], { 0,0,0 }, vecDir[dirOut], 2*vecDir[dirOut], K);
		v = cgp::normalize(p - pp);
		return;
	}
	timeCount = 0;
	casePos = casePos->getNeighbors()[dirOut];
	
	dirIn = (dirOut+2) %4;
	dirOut = nextTile(dirIn);
	p = vecDir[dirIn];
}

int Car::nextTile(int in)
{
	std::array<Case*, 4> Neighbors = casePos->getNeighbors();
	Tile* tilePos = Case::Tiles[casePos->getFixedTile()];
	int dir;
	int test = 0;
	do {
		dir = rand() % 4;
		test++;
	} while (test<20 && (dir == in || tilePos->getProximity(dir) != 1 || Neighbors[dir] == NULL));
	if (test >= 20) dir = in;
	return dir;
}



void Car::drawCar(project_scene_environment environment)
{
	shape_car.transform.rotation = cgp::rotation_transform::between_vector({ 0,1,0 }, v);
	shape_tires.transform.rotation = cgp::rotation_transform::between_vector({ 0,1,0 }, v);
	shape_car.transform.translation = casePos->getPosition() + p;
	shape_tires.transform.translation = casePos->getPosition() + p;
	cgp::draw(shape_tires, environment);
	cgp::draw(shape_car, environment);
}

void Car::changeColor(cgp::vec3 color)
{
	shape_car.shading.color = color;
}

cgp::vec3 Car::getPos()
{
	return p + casePos->getPosition();
}

cgp::vec3 Car::getV()
{
	return v;
}

