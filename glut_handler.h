#pragma once

//usual libraries
#include<iostream>
#include<string>

//gl libraries
#include<GL/glew.h>
#include<GL/freeglut.h>


#include "camera.h"
#include "shaders.h"
#include "game.h"

#define GLUT_FLAGS GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH

#define WINDOW_DEFAULT {0,0,1000,1000}
//#define WINDOW_DEFAULT {0,0,1920,1080}

#define DEFAULT_CONTEXT_SWITCH 12


#define ORIG_WIDTH 1000
#define ORIG_HEIGHT 1000
#define WINDOW_CAPTION "Harder Tetris"
#define TIME_PER_FRAME 16  //animate.cc's time keeping statements used here - about 60 fps
#define TICKS_PER_SEC 60

#define ESCAPE 27   //macro to make ESCAPE case in switch statement more readable


//! struct holds uints that track the window's information
struct rect{
	GLuint x;  //!< store the window's horizontal location
	GLuint y;  //!< store the window's vertical location
	GLuint w;  //!< store the window's width for the window
	GLuint h;  //!< store the window's height for the window
};

//! \class glut_handler abstracts away alot of the GLUT stuff like windowing
class glut_handler{
  public:
	//! default constructor calls glutInit and inits the display
	/*! uses the macro GLUT_FLAGS for arguments */
	glut_handler(int& argc, char* argv[]);

	//! This is not in use yet 10/1
	//~glut_handler();

	//! display call back function
	/*! odd function types are to make it play well with glut */
	static void display();

	//! processes keyboard events
	static void handle_key(unsigned char key, int x, int y);

	//! processes keyboard events like the arrow keys
	/*! it actuall just calls handle_key with the letter
	 *corresponding to what needs to be done */
	static void handle_other_keys(int key, int x, int y);

	//! processes mouse events
	static void handle_mouse(int button, int state, int x, int y);

	//! processes window resize events
	static void handle_resize(int width_in, int height_in);

	//! ensures that the display is updated at the same frame rate on all machines that are capable
	/*! from Dr. Chelber's animate.cc file */
	static void timerfunc(int value);

	//! update the class upon resize
	void update_class_window_info(int width_in, int height_in);

	const rect& get_win_info(){ return window_info;}     //!< getter for window info
	const rect& get_screen_info(){ return screen_info;}  //!< getter for screen info

	GLuint ticks;//!< keep track of time, other objects will need this info as well


  private:
	rect window_info;  //!< contain window information
	rect screen_info;  //!< contain screen information
};
