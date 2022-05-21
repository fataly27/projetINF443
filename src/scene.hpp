#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the multiple_lights  */

#include <array>
#include "cgp/cgp.hpp"
#include "multiple_lights/multiple_lights.hpp"
#include "terrain.hpp"
#include "case.hpp"

#define NCases 30


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
};


// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_with_multiple_lights environment; // The specific scene environment with multiple lights (*)
	
	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::mesh_drawable cube;            // A cube displayed in the center of the scene
	cgp::mesh_drawable ground;          // The ground
	cgp::mesh_drawable ocean;          // The ocean

	light_shape_drawable light_drawable; // Helper structure used to display the lights as spheres (*)

	cgp::timer_basic timer; // A basic timer for the animation of the lights
	gui_parameters gui;     // The standard GUI element storage
	
	std::array<Case*, NCases * NCases> Cases;
	
	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
};





