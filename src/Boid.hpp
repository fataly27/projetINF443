#pragma once

#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"

class Boid
{
public :
	static constexpr int max = 200;


	static float factors[6];
	static float proximity[3];
	static float v;

	static cgp::mesh* body_mesh;
	static cgp::mesh_drawable* body;

private:

	cgp::vec3 position;
	cgp::vec3 velocity;


	static void setBodyMesh(cgp::mesh* body_mesh);
	static void setBody(cgp::mesh_drawable* body);

	std::array<Boid *, max> others;
	int nbOthers = 0;
	float walls[6];

	void bounceWalls();
	cgp::vec3 avoidWalls();
	cgp::vec3 avoidOthers();
	cgp::vec3 flyTowardOthers();
	cgp::vec3 alignWithOthers();
	cgp::vec3 flyRandom();
	
	

public:
	Boid(int);
	Boid(cgp::vec3 position = {0,0,0});
	

	void setWalls(float[6]);
	void setPosition(cgp::vec3);
	void setVelocity(cgp::vec3);
	void setOthers(std::array<Boid *, max>, int);

	cgp::vec3 getPosition();
	

	void applyForces();
	void update(float dt);
	void initialize();
	void drawBoid(project_scene_environment environment, cgp::vec3 offset = { 0,0,0 });
};