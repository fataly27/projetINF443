#include "Player.hpp"

float Player::maxSpeed = 2;
float Player::acc = 2;
float Player::rotSpeed = 2;


Player::Player(cgp::vec3 pos, cgp::vec3 dir, float speed)
{
	this->pos = pos;
	this->dir = dir;
	this->speed = speed;

	footPos[0] = pos + cgp::vec3(0.2, 0, -0.7);
	footPos[1] = pos + cgp::vec3(-0.2, 0, -pos.z);
}

void Player::update(cgp::inputs_interaction_parameters inputs, float dt)
{
	cgp::inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	
	cgp::mat3 R = {
	1,0,0,
	0,1,0,
	0,0,1
	};

	if (keyboard.up) {
		if (speed < maxSpeed)
			speed += dt * acc;
	}

	if (keyboard.down) {
		if (speed > -maxSpeed)
			speed -= dt * acc;
	}

	if (!keyboard.up && !keyboard.down)
		speed = 0;
	
	if (keyboard.right)
	{
		R(0, 0) = cos(dt * rotSpeed);
		R(0, 1) = sin(dt * rotSpeed);
		R(1, 0) = -sin(dt * rotSpeed);
		R(1, 1) = cos(dt * rotSpeed);
	}
	if (keyboard.left)
	{
		R(0, 0) = cos(dt * rotSpeed);
		R(0, 1) = -sin(dt * rotSpeed);
		R(1, 0) = sin(dt * rotSpeed);
		R(1, 1) = cos(dt * rotSpeed);
	}


	dir = R * dir;
	hipPos[0] = R * hipPos[0];
	hipPos[1] = R * hipPos[1];

	if (footPos[foot].z < 0.05) {
		foot = 1 - foot;
		footPos[0] += 0.02 * (footPos[1] - footPos[0]);
		footPos[1] += 0.02 * (footPos[0] - footPos[1]);
 	}

	footPos[foot] += dt * speed * dir;
	footPos[foot] += dt * cgp::dot(cgp::cross(dir, cgp::vec3(0, 0, 1)), pos + hipPos[foot] - footPos[foot]) * cgp::cross(dir, cgp::vec3(0, 0, 1));
	footPos[foot].z = (0.7 - norm(footPos[1] - footPos[0]));


	pos = (footPos[0] + footPos[1]) / 2 + cgp::vec3(0, 0, 0.7);
}


void Player::drawPlayer(project_scene_environment environment)
{
	rightIK(footPos[0]);
	leftIK(footPos[1]);

	sphere.transform.translation = pos;
	cgp::draw(sphere, environment);

	bone.transform.translation = pos + hipPos[0];
	bone.transform.rotation = cgp::rotation_transform::between_vector({ 0,0,1 }, (rightKnee-hipPos[0])/lenBone);
	cgp::draw(bone, environment);
	bone.transform.translation = pos + rightKnee;
	bone.transform.rotation = cgp::rotation_transform::between_vector({ 0,0,1 }, (rightAnkle - rightKnee) / lenBone);
	cgp::draw(bone, environment);
	bone.transform.translation = pos + rightAnkle;
	bone.transform.rotation = cgp::rotation_transform::between_vector({ 0,0,1 }, dir);
	bone.transform.scaling = 0.3;
	cgp::draw(bone, environment);
	bone.transform.scaling = 1;
	bone.shading.color = { 0,0.3,0.8 };

	bone.transform.translation = pos + hipPos[1];
	bone.transform.rotation = cgp::rotation_transform::between_vector({ 0,0,1 }, (leftKnee - hipPos[1]) / lenBone);
	cgp::draw(bone, environment);
	bone.transform.translation = pos + leftKnee;
	bone.transform.rotation = cgp::rotation_transform::between_vector({ 0,0,1 }, (leftAnkle - leftKnee) / lenBone);
	cgp::draw(bone, environment);
	bone.transform.translation = pos + leftAnkle;
	bone.transform.rotation = cgp::rotation_transform::between_vector({ 0,0,1 }, dir);
	bone.transform.scaling = 0.3;
	cgp::draw(bone, environment);
	bone.transform.scaling = 1;
	bone.shading.color = { 0,0.3,0.8 };
	
}

void Player::moveCamera(project_scene_environment& environment, float upwards)
{
	environment.camera.look_at(pos - 3*dir + cgp::vec3(0,0,1) + cgp::cross(cgp::vec3(0, 0, 1), dir), cgp::vec3(0, 0, upwards) + pos + cgp::cross(cgp::vec3(0, 0, 1), dir), {0,0,1});
}

void Player::initializePlayer()
{
	sphere.initialize(cgp::mesh_primitive_sphere(0.2), "sphere");
	sphere.shading.color = { 1,0,0 };

	bone.initialize(cgp::mesh_primitive_cone(lenBone/10,lenBone), "bone");
	bone.shading.color = { 0,0.3,0.8 };

	rightIK(footPos[0]);
	leftIK(footPos[1]);

}

void Player::rightIK(cgp::vec3 target) 
{
	rightKnee = hipPos[0] -lenBone*dir;
	for (int i = 0; i < 5; i++) {
		rightAnkle = target - pos;
		cgp::vec3 dirBone = rightAnkle - rightKnee;
		rightKnee = rightAnkle - lenBone * cgp::normalize(dirBone);
		dirBone = rightKnee - hipPos[0];
		cgp::vec3 temp = hipPos[0];
		hipPos[0] = rightKnee - lenBone * cgp::normalize(dirBone);
		cgp::vec3 offset = temp - hipPos[0];
		hipPos[0] += offset;
		rightKnee += offset;
		rightAnkle += offset;
	}
}

void Player::leftIK(cgp::vec3 target)
{
	leftKnee = hipPos[1] - lenBone * dir;
	for (int i = 0; i < 5; i++) {
		leftAnkle = target - pos;
		cgp::vec3 dirBone = leftAnkle - leftKnee;
		leftKnee = leftAnkle - lenBone * cgp::normalize(dirBone);
		dirBone = leftKnee - hipPos[1];
		cgp::vec3 temp = hipPos[1];
		hipPos[1] = leftKnee - lenBone * cgp::normalize(dirBone);
		cgp::vec3 offset = temp - hipPos[1];
		hipPos[1] += offset;
		leftKnee += offset;
		leftAnkle += offset;
	}
}

cgp::vec3 Player::getPos()
{
	return pos;
}

cgp::vec3 Player::getDir()
{
	return dir;
}

float Player::getSpeed()
{
	return speed;
}
