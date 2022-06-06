#pragma once

#include "case.hpp"
#include "ProjectEnvironment.hpp"

class Car
{
public:
	float waitTime = 4;

private:
	cgp::mesh mesh_car;
	cgp::mesh mesh_tires;
	cgp::mesh_drawable shape_car;
	cgp::mesh_drawable shape_tires;

	Case* casePos;

	float timeCount = 0;

	int dirIn = 1;
	int dirOut = 1;

	cgp::vec3 p;
	cgp::vec3 v;

	int nextTile(int in);
public:
	Car();
	Car(Case*);
	void initializeCar();
	void updateCar(float);
	void drawCar(project_scene_environment environment);

	void changeColor(cgp::vec3 color);

	cgp::vec3 getPos();
	cgp::vec3 getV();
};
