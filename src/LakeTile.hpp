#pragma once

#include "TreeTile.hpp"
#include "cgp/cgp.hpp"
#include <string>
#include "ProjectEnvironment.hpp"
#include "mesh_drawable_lake.hpp"


class LakeTile : public TreeTile
{
	private:
		float time;
		float moveFactor;

		int window_width;
		int window_height;

		cgp::mesh mesh;
		cgp::mesh_drawable shape;
		GLuint texture_image_id;
		std::string texture_file;

		mesh_drawable_lake lake_shape;
		float lake_deepness = -0.3f;

		//Lanterns
		cgp::mesh lantern_mesh;
		cgp::mesh_drawable lantern_shape;
		GLuint lantern_texture_image_id;
		std::string lantern_texture_file;
		float lantern_scale = 0.123f;

		cgp::mesh_drawable lantern_billboard;
		std::string lantern_billboard_texture_file;
		float lantern_billboard_scale = 1.f;

		int n_lantern;
		std::vector<cgp::vec3> LanternPositions;
		std::vector<cgp::vec3> LanternDisplacements;

	public:
		LakeTile(int index, int rotation = Up);
		~LakeTile();
		virtual void drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height) override;
		virtual void drawTileTransparent(cgp::vec3 position, project_scene_environment environment) override;
		virtual void initialiseTile() override;
		virtual void updateTile(float dt) override;

		virtual bool canPlaceTree(float x, float y) override;
		virtual float terrainHeight(float x, float y) override;

	private:
		//For the lake reflection
		GLuint reflectionFrameBuffer;
		GLuint reflectionTexture;
		GLuint reflectionDepthBuffer;
		const int REFLECTION_WIDTH = 640;
		const int REFLECTION_HEIGHT = 360;

		void drawWithoutLake(cgp::vec3 position, project_scene_environment environment);

		void initialiseReflectionFrameBuffer();
		int createFrameBuffer();
		int createTextureAttachment(int width, int height);
		int createDepthBufferAttachment(int width, int height);
		void bindReflectionFrameBuffer();
		void unbindCurrentFrameBuffer();
		void bindFrameBuffer(int frameBuffer, int width, int height);
};