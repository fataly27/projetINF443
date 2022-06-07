#pragma once
#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"

class Player
{
public:
	static float maxSpeed;
	static float acc;
	static float rotSpeed;

	Player(cgp::vec3 pos = cgp::vec3(0, 0, 0.7), cgp::vec3 dir = cgp::vec3(0, 1, 0), float speed = 0);

	void initializePlayer();
	void update(cgp::inputs_interaction_parameters inputs, float dt);
	void drawPlayer(project_scene_environment environment);
	void moveCamera(project_scene_environment& environment, float upwards);


	cgp::vec3 getPos();
	cgp::vec3 getDir();
	float getSpeed();


private:
	cgp::vec3 pos;
	cgp::vec3 footPos[2];

	cgp::vec3 rightPos;
	cgp::vec3 leftPos;

	cgp::vec3 dir;
	float speed;

	int foot = 0;

	float lenBone = 0.5;
	
	cgp::vec3 hipPos[2] = { {0.2, 0, 0}, {-0.2, 0, 0 } };
	cgp::vec3 rightKnee = {0.2,lenBone,0};
	cgp::vec3 rightAnkle = {0.2,2*lenBone,0};
	void rightIK(cgp::vec3 target);


	cgp::vec3 leftKnee = { -0.2,lenBone,0 };
	cgp::vec3 leftAnkle = { -0.2,2 * lenBone,0 };
	void leftIK(cgp::vec3 target);

	cgp::mesh_drawable bone;

	cgp::mesh_drawable sphere;
};
