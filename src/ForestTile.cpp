#include "ForestTile.hpp"

ForestTile::ForestTile(int index) : TreeTile()
{
	Aretes[Up] = 2;
	Aretes[Down] = 2;
	Aretes[Left] = 2;
	Aretes[Right] = 2;

    terrain_mesh = create_terrain_mesh(70, 10.f);

	//mesh.fill_empty_field();
	n_trees = 70;
}

void ForestTile::drawTile(cgp::vec3 position, project_scene_environment environment, int width, int height)
{
    terrain_shape.transform.translation = position;
	cgp::draw(terrain_shape, environment);

	TreeTile::drawTile(position, environment);
}

void ForestTile::initialiseTile()
{
	TreeTile::initialiseTile();

    terrain_shape.initialize(terrain_mesh, "ForestTerrainShape");
    terrain_shape.shading.color = { 0.3f,0.8f,0.4f };
    terrain_shape.shading.phong.specular = 0.0f; // non-specular terrain material
}

void ForestTile::updateTile(float dt)
{
	
}

bool ForestTile::canPlaceTree(float x, float y)
{
	return true;
}
float ForestTile::terrainHeight(float x, float y)
{
    float epsilon = 0.05f;

    float z = 0.f;

    // Compute the Perlin noise
    float const noise = cgp::noise_perlin({ x, y }, 8, 0.40f, 2.1f);

    // use the noise as displacment height value
    z += 3.4f * noise;

    float d = cgp::norm(cgp::vec2(x, y) - cgp::vec2(0.5f, 0.5f)) / 0.3f;
    z *= std::exp(-d * d);

    if (x < epsilon)
        return z * x / epsilon;
    else if (x > 1.f - epsilon)
        return z * (1.f - x) / epsilon;
    else if (y < epsilon)
        return z * y / epsilon;
    else if (y > 1.f - epsilon)
        return z * (1.f - y) / epsilon;

	return z;
}

cgp::mesh ForestTile::create_terrain_mesh(int n, float terrain_length)
{
    cgp::mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(n * n);
    terrain.uv.resize(n * n);

    // Fill terrain geometry and generate UVs
    for (int ku = 0; ku < n; ++ku)
    {
        for (int kv = 0; kv < n; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (n - 1.0f);
            float v = kv / (n - 1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = terrainHeight(u, v);

            // Store vertex coordinates
            terrain.position[kv + n * ku] = { x,y,z };
            terrain.uv[kv + n * ku] = { float(kv) / 5.f, float(ku) / 5.f };
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < n - 1; ++ku)
    {
        for (int kv = 0; kv < n - 1; ++kv)
        {
            unsigned int idx = kv + n * ku; // current vertex offset

            cgp::uint3 triangle_1 = { idx, idx + 1 + n, idx + 1 };
            cgp::uint3 triangle_2 = { idx, idx + n, idx + 1 + n };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    terrain.fill_empty_field();

    return terrain;
}