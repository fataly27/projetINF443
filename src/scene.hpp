#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the multiple_lights  */

#include <array>
#include "cgp/cgp.hpp"
#include "ProjectEnvironment.hpp"
#include "terrain.hpp"
#include "case.hpp"
#include "Car.hpp"
#include "BoidTile.hpp"
#include "FountainTile.hpp"
#include "LakeTile.hpp"
#include "Player.hpp"
#include "building.hpp"
#include "BuildingTile.hpp"

#define NCases 10


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
};


// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	project_scene_environment environment; // The specific scene environment
	
	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::skybox_drawable skybox;	// The sky box

	gui_parameters gui;     // The standard GUI element storage
	
	std::array<Case*, NCases * NCases> Cases;

	cgp::timer_event_periodic timer;
	cgp::inputs_interaction_parameters sceneInputs;

	House H;
	Car C;
	Player P;
	
	BoidTile* BT;
	FountainTile* FT;
	LakeTile* TestLakeTile;
	BuildingTile* BuildT;
	
	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
};





