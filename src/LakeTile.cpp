#include "LakeTile.hpp"
#include <cmath>

LakeTile::LakeTile(int index, int rotation) : TreeTile()
{
	Rotation = rotation;

	time = 0.f;

	Aretes[Up] = 2;
	Aretes[Down] = 1;
	Aretes[Left] = 2;
	Aretes[Right] = 2;
	mesh = cgp::mesh_load_file_obj("assets/tiles/lake/lac.obj");
	texture_file = "assets/tiles/lake/lake_texture.png";

	lantern_mesh = cgp::mesh_load_file_obj("assets/tiles/lake/lantern.obj");
	lantern_texture_file = "assets/tiles/lake/lantern_textureb.png";

	lantern_billboard_texture_file = "assets/tiles/lake/lantern_billboard.png";

	mesh.fill_empty_field();
	n_trees = 60;
	n_lantern = 200;
}

LakeTile::~LakeTile()
{
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
}

void LakeTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
	if (height == 0 || width == 0 || height > 10000 || width > 10000)
		return;

	window_width = width;
	window_height = height;

	if (cgp::norm(position - environment.camera.position()) < 30.f && (cgp::dot(environment.camera.front(), position - environment.camera.position()) > 0.f || cgp::norm(position - environment.camera.position()) < 10.f))
	{
		cgp::camera_around_center temp_camera = environment.camera;
		cgp::vec3 camera_front = cgp::normalize(environment.camera.front());
		cgp::vec3 camera_ground_front = camera_front;
		camera_ground_front.z = 0.f;
		if (cgp::norm(camera_ground_front) < 0.01f)
			camera_ground_front = cgp::vec3(1.f, 0.f, 0.f);
		else
			camera_ground_front = cgp::normalize(camera_ground_front);
		float angle = 2 * std::acos(cgp::dot(camera_front, camera_ground_front));

		if (camera_front.z < 0.f)
			angle *= -1.f;

		environment.camera.manipulator_rotate_roll_pitch_yaw(0, angle, 0);

		float center_distance = environment.camera.center_of_rotation.z - lake_deepness;
		environment.camera.center_of_rotation.z = lake_deepness - center_distance;

		glEnable(GL_CLIP_DISTANCE0);
		environment.plane = cgp::vec4(0, 0, 1, -lake_deepness);
		bindReflectionFrameBuffer();
		glClearColor(0.75f, 0.82f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		drawWithoutLake(position, environment);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDepthMask(false);
		drawTileTransparent(position, environment);
		glDepthMask(true);
		glDisable(GL_BLEND);
		unbindCurrentFrameBuffer();
		glDisable(GL_CLIP_DISTANCE0);
		environment.plane = cgp::vec4(0, 0, 1, -100);
		environment.camera = temp_camera;

	}

	drawWithoutLake(position, environment);
	lake_shape.transform.translation = position;
	lake_shape.moveFactor = moveFactor;
	draw(lake_shape, environment);
}

void LakeTile::drawWithoutLake(cgp::vec3 position, project_scene_environment environment)
{
	shape.transform.translation = position;

	if (Rotation == Right)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, -cgp::Pi / 2);
	else if (Rotation == Down)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi);
	else if (Rotation == Left)
		shape.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, cgp::Pi / 2);

	cgp::draw(shape, environment);

	TreeTile::drawTile(position, environment);

	if (cgp::norm(position - environment.camera.position()) < 50.f && (cgp::dot(environment.camera.front(), position - environment.camera.position()) > 0.f || cgp::norm(position - environment.camera.position()) < 10.f))
	{
		for (int i = 0; i < n_lantern; i++)
		{
			lantern_shape.transform.translation = position + LanternPositions[i] + LanternDisplacements[i];
			lantern_shape.transform.scaling = lantern_scale / 1.23f;
			cgp::draw(lantern_shape, environment);
		}
	}
}

void LakeTile::drawTileTransparent(cgp::vec3 position, project_scene_environment environment)
{
	if (cgp::norm(position - environment.camera.position()) < 40.f && (cgp::dot(environment.camera.front(), position - environment.camera.position()) > 0.f || cgp::norm(position - environment.camera.position()) < 10.f))
	{
		// Re-orient the grass shape to always face the camera direction
		cgp::vec3 const front = cgp::normalize(environment.camera.front() * cgp::vec3{ 1, 1, 1 }); // front-vector of the camera with z-component
		// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
		cgp::rotation_transform R = cgp::rotation_transform::between_vector({ 0,1,0 }, front);
		lantern_billboard.transform.rotation = R;

		for (int i = 0; i < n_lantern; i++)
		{
			lantern_billboard.transform.translation = position + LanternPositions[i] + LanternDisplacements[i];
			draw(lantern_billboard, environment);
		}
	}
}

void LakeTile::initialiseTile()
{
	TreeTile::initialiseTile();

	initialiseReflectionFrameBuffer();


	texture_image_id = cgp::opengl_load_texture_image(texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	shape.initialize(mesh, "LakeShape");
	shape.texture = texture_image_id;

	lake_shape.initialize(cgp::mesh_primitive_quadrangle({ -3.f, -3.f, lake_deepness }, { -3.f, 3.f, lake_deepness }, { 3.f, 3.f, lake_deepness }, { 3.f, -3.f, lake_deepness }), "Lake");
	lake_shape.shader = cgp::opengl_load_shader("shaders/project_lake/vert.glsl", "shaders/project_lake/frag.glsl");
	lake_shape.texture = reflectionTexture;
	lake_shape.dudv_texture = cgp::opengl_load_texture_image("assets/tiles/lake/waterDUDV.png", GL_REPEAT, GL_REPEAT);

	lantern_texture_image_id = cgp::opengl_load_texture_image(lantern_texture_file, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	lantern_shape.initialize(lantern_mesh, "Lantern");
	lantern_shape.texture = lantern_texture_image_id;

	lantern_billboard.initialize(cgp::mesh_primitive_quadrangle({ -0.5f, -lantern_scale / lantern_billboard_scale, -0.5f }, { 0.5f, -lantern_scale / lantern_billboard_scale, -0.5f }, { 0.5f, -lantern_scale / lantern_billboard_scale, 0.5f }, { -0.5f, -lantern_scale / lantern_billboard_scale, 0.5f }), "LanternBillboard");
	lantern_billboard.texture = cgp::opengl_load_texture_image(lantern_billboard_texture_file);
	lantern_billboard.transform.scaling = lantern_billboard_scale;
	lantern_billboard.shader = cgp::opengl_load_shader("shaders/project_billboards/vert.glsl", "shaders/project_billboards/frag.glsl");


	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float c = 0.f;

	for (int i = 0; i < n_lantern; i++)
	{
		LanternDisplacements.push_back(cgp::vec3(0.f, 0.f, 0.f));

		bool not_okay = true;
		while (not_okay)
		{
			x = cgp::rand_interval();
			y = cgp::rand_interval();
			z = cgp::rand_interval();
			c = cgp::rand_interval(0.f, std::pow(1.8f, 8));

			float d = cgp::norm(cgp::vec2(x, y) - cgp::vec2(0.5f, 0.5f));

			not_okay = std::exp(-d * d) * std::pow(cgp::noise_perlin(cgp::vec3(x, y, z), 2, 0.8f, 3.f), 8) < c;
		}

		if (Rotation == Up)
			LanternPositions.push_back(cgp::vec3(10 * x - 5.f, 10 * y - 5.f, 5.f * z + 3.f));
		else if (Rotation == Right)
			LanternPositions.push_back(cgp::vec3(10 * y - 5.f, -(10 * x - 5.f), 5.f * z + 3.f));
		else if (Rotation == Down)
			LanternPositions.push_back(cgp::vec3(-(10 * x - 5.f), -(10 * y - 5.f), 5.f * z + 3.f));
		else if (Rotation == Left)
			LanternPositions.push_back(cgp::vec3(-(10 * y - 5.f), 10 * x - 5.f, 5.f * z + 3.f));
	}
}

void LakeTile::updateTile(float dt)
{
	moveFactor += 0.03f * dt;
	while (moveFactor >= 1.f)
		moveFactor -= 1.f;


	time += dt / 5.f;

	for (int i = 0; i < n_lantern; i++)
	{
		float x = std::cos(time + i);
		float y = std::sin(time + i);
		float z = std::cos(time + i);
		float a = std::pow(cgp::noise_perlin(LanternPositions[i], 2, 0.6f, 2.4f), 2) / 2.f;

		LanternDisplacements[i] = a * cgp::vec3(x, y, z);
	}
}

bool LakeTile::canPlaceTree(float x, float y)
{
	bool farFromLake = std::pow((x - 0.5f), 2) + std::pow((y - 0.5f), 2) > std::pow(0.3f, 2); // On n'affiche des arbres que suffisamment loin du centre (où il y a le lac)
	bool notOnRoad = std::abs(y - 0.5f) > 0.1f || x < 0.8f; //On n'affiche que des arbres qui ne sont pas sur la route

	return farFromLake && notOnRoad;
}
float LakeTile::terrainHeight(float x, float y)
{
	return 0.f;
}

//About the reflection
void LakeTile::initialiseReflectionFrameBuffer()
{
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindCurrentFrameBuffer();
}
int LakeTile::createFrameBuffer()
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//create the framebuffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return frameBuffer;
}
int LakeTile::createTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}
int LakeTile::createDepthBufferAttachment(int width, int height)
{
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}
void LakeTile::bindReflectionFrameBuffer() //call before rendering to this FBO
{
	bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}
void LakeTile::unbindCurrentFrameBuffer() //call to switch to default frame buffer
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window_width, window_height);
}
void LakeTile::bindFrameBuffer(int frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0); //To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}