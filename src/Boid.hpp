#pragma once

#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"

class Boid
{
public :
	static constexpr int max = 100;

	bool debug = false;
	cgp::vec3 debugCenter;
	cgp::mesh_drawable debugSphere;

	static float factors[6];
	static float proximity[3];
	static float v;


private:

	cgp::vec3 position;
	cgp::vec3 velocity;
	cgp::mesh body_mesh;
	cgp::mesh_drawable body;

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
	void drawBoid(scene_environment_with_multiple_lights, cgp::vec3 offset = { 0,0,0 });
};