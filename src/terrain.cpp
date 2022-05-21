#include "terrain.hpp"

using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_height(float x, float y)
{
    const int nb_gauss = 4;
    vec2 p_0[nb_gauss] = { { 0.3f, 0.25f }, { 0.52f, 0.67f }, { 0.15f, 0.64f }, { 0.81f, 0.32f } };
    float h_0[nb_gauss] = { 5.0f, 3.5f, 7.f, 5.f };
    float sigma_0[nb_gauss] = { 0.08f, 0.15f, 0.09f, 0.07f };

    float z = 0.f;

    for (int i = 0; i < nb_gauss; i++)
    {
        float d = norm(vec2(x, y) - p_0[i]) / sigma_0[i];
        z += h_0[i] * std::exp(-d * d);
    }

    // Compute the Perlin noise
    float const noise = noise_perlin({ x, y }, 8, 0.40f, 2.1f);

    // use the noise as displacment height value
    z += 7.4f * noise;

    return z - 7.f;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.uv.resize(N * N);

    // Fill terrain geometry and generate UVs
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(u, v);

            // Store vertex coordinates
            terrain.position[kv + N * ku] = { x,y,z };
            terrain.uv[kv + N * ku] = { float(kv) / 5.f, float(ku) / 5.f };
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; // current vertex offset

            uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    terrain.fill_empty_field();

    return terrain;
}