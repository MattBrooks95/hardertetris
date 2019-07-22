//! \file main.cc implements the structure of a game of HarderTetris

//useful includes
#include <iostream>
#include <chrono>
#include <cstdlib>

//my includes
#include "glut_handler.h"
#include "shaders.h"
#include "game.h"
#include "camera.h"

using namespace std;

//make sure the glut_handler can be accessed by all objects that need it
//this is a pointer so that nothing happens pre-main in constructors
glut_handler* glut_access;

//make sure that the shaders and GL global variables can be found by
//all objects that need them
shaders* shader_access;

//global access to the game state
game* game_access;

//global access to the camera class
camera* gaze_access;

int main(int argc, char *argv[]){

	//set up the glut handling class
	glut_handler my_glut(argc,argv);

	//make the global pointer point to the now constructed object
	glut_access = &my_glut;

	//shader initialization ######################################
	shaders shader_manager;
	shader_access = &shader_manager;

	vector<shader_request> shader_list;
	shader_list.push_back(shader_request("vgame.glsl","vshader"));
	shader_list.push_back(shader_request("fgame.glsl","fshader"));
	shader_manager.init_shaders(shader_list);
	shader_manager.print_shaders_source();
	//############################################################

	//game state initialization###################################
	game tetris;
	game_access = &tetris;
	game_access->make_scene();

	// tetris.print_game_bools();
	//tetris.print_positions();
	//############################################################

	//camera initialization#######################################
	camera gaze;
	gaze_access = & gaze;
	//############################################################

	//get the random number generator set up######################
	std::time_t toki = std::time(NULL);  //get time from the system, to be used as a seed
	std::cout << "The time is: " << std::asctime(std::localtime(&toki)) << std::endl;
	srand(toki);//seed the random number generator
	//############################################################

	//enter glut's infinite loop
	glutMainLoop();

    return EXIT_SUCCESS;//exit with success
}
