#include "building.hpp"

House::House()
{
	height = rand() % 3;
	doorPos = cgp::rand_interval(1, 3);
}

void House::initializeHouse()
{
	house_mesh.position.push_back({ 0,0,0 });
	house_mesh.position.push_back({ 5,0,0 });
	house_mesh.position.push_back({ 5,3,0 });
	house_mesh.position.push_back({ 0,3,0 });

	house_mesh.position.push_back({ 0,0,2 * height + 3 });
	house_mesh.position.push_back({ 5,0,2 * height + 3 });
	house_mesh.position.push_back({ 5,3,2 * height + 3 });
	house_mesh.position.push_back({ 0,3,2 * height + 3 });


	house_mesh.connectivity.push_back({ 0,1,5});
	house_mesh.connectivity.push_back({ 0,5,4 });

	house_mesh.connectivity.push_back({ 1,2,6 });
	house_mesh.connectivity.push_back({ 1,6,5 });

	house_mesh.connectivity.push_back({ 2,3,7 });
	house_mesh.connectivity.push_back({ 2,7,6 });

	house_mesh.connectivity.push_back({ 3,0,4 });
	house_mesh.connectivity.push_back({ 3,4,7 });


	roof_mesh.position.push_back({ 0,0,2 * height + 3 });
	roof_mesh.position.push_back({ 5,0,2 * height + 3 });
	roof_mesh.position.push_back({ 5,3,2 * height + 3 });
	roof_mesh.position.push_back({ 0,3,2 * height + 3 });

	roof_mesh.position.push_back({ 0.5 ,1.5 , 2 * height + 3 + 2 });
	roof_mesh.position.push_back({ 4.5 ,1.5 , 2 * height + 3 + 2 });

	roof_mesh.connectivity.push_back({ 0,1,5 });
	roof_mesh.connectivity.push_back({ 0,5,4 });

	roof_mesh.connectivity.push_back({ 1,2,5 });

	roof_mesh.connectivity.push_back({ 2,3,4 });
	roof_mesh.connectivity.push_back({ 2,4,5 });

	roof_mesh.connectivity.push_back({ 3,0,4 });


	house_mesh.fill_empty_field();
	roof_mesh.fill_empty_field();
	house.initialize(house_mesh, "House");
	roof.initialize(roof_mesh, "Roof");

	house.shading.color = { 0.5,0.5,0.5 };
	roof.shading.color = { 1,0,0 };

	window_mesh.position.push_back({ -0.4, -0.3, -0.75 });
	window_mesh.position.push_back({ 0.4, -0.3, -0.75 });
	window_mesh.position.push_back({ 0.4, 0.3, -0.75 });
	window_mesh.position.push_back({ -0.4, 0.3, -0.75 });

	window_mesh.position.push_back({ -0.4, -0.3, 0.75 });
	window_mesh.position.push_back({ 0.4, -0.3, 0.75 });
	window_mesh.position.push_back({ 0.4, 0.3, 0.75 });
	window_mesh.position.push_back({ -0.4, 0.3, 0.75 });

	window_mesh.connectivity.push_back({ 2,1,0 });
	window_mesh.connectivity.push_back({ 3,2,0 });

	window_mesh.connectivity.push_back({ 0,1,5 });	
	window_mesh.connectivity.push_back({ 0,5,4 });

	window_mesh.connectivity.push_back({ 4,5,6 });
	window_mesh.connectivity.push_back({ 4,6,7 });

	window_mesh.connectivity.push_back({ 3,0,4 });
	window_mesh.connectivity.push_back({ 3,4,7 });

	window_mesh.connectivity.push_back({ 1,2,6 });
	window_mesh.connectivity.push_back({ 1,6,5 });

	window_mesh.fill_empty_field();
	window.initialize(window_mesh, "Window");
	window.shading.color = { 0.2,0.2,0.2 };

	door_mesh.position.push_back({ -0.5, -0.3, 0 });
	door_mesh.position.push_back({ 0.5, -0.3, 0 });
	door_mesh.position.push_back({ 0.5, 0.3, 0 });
	door_mesh.position.push_back({ -0.5, 0.3, 0 });

	door_mesh.position.push_back({ -0.5, -0.3, 2.3 });
	door_mesh.position.push_back({ 0.5, -0.3, 2.3 });
	door_mesh.position.push_back({ 0.5, 0.3, 2.3 });
	door_mesh.position.push_back({ -0.5, 0.3, 2.3 });

	door_mesh.connectivity.push_back({ 2,1,0 });
	door_mesh.connectivity.push_back({ 3,2,0 });

	door_mesh.connectivity.push_back({ 0,1,5 });
	door_mesh.connectivity.push_back({ 0,5,4 });

	door_mesh.connectivity.push_back({ 4,5,6 });
	door_mesh.connectivity.push_back({ 4,6,7 });

	door_mesh.connectivity.push_back({ 3,0,4 });
	door_mesh.connectivity.push_back({ 3,4,7 });

	door_mesh.connectivity.push_back({ 1,2,6 });
	door_mesh.connectivity.push_back({ 1,6,5 });

	door_mesh.fill_empty_field();
	door.initialize(door_mesh, "Door");
	door.shading.color = { 0.60, 0.40, 0.20 };
}

void House::drawHouse(scene_environment_with_multiple_lights environment, cgp::vec3 position, cgp::vec3 dir)
{

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < 4; j++) {
			window.transform.translation = cgp::vec3(1 + j, 0, 4 + 2 * i) + position;
			cgp::draw(window, environment);
		}
	}

	door.transform.translation = cgp::vec3(doorPos, 0, 0) + position;
	cgp::draw(door, environment);

	house.transform.translation = position;
	roof.transform.translation = position;
	cgp::draw(house, environment);
	cgp::draw(roof, environment);

}
