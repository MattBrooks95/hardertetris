//! \file camera.h describes the class that stores the camera's info
#pragma once

#include <glm/glm.hpp>

#define CAM_START  -5.0, 10.0, 30.0

//#define CAM_START  0.0, 0.0, 1.0
#define LOOK_START -5.0, 10.0,  -1.0
//#define LOOK_START 0.0, 0.0,  0.0

#define ZNEAR   0.1
#define ZFAR   90.0

//! \class camera handles the states necessary for the lookat function, and does the camera tricks
class camera{

  public:
	//! camera() sets fields to defaults
	camera();
	//this is a placeholder in case it is needed
	//~camera();

	//! getter for where the camera is located
	const glm::vec3& get_eye(){ return eye;}
	//! getter for where the eye is looking
	const glm::vec3& get_looking(){ return looking;}
	//! getter for what the current up vector is
	const glm::vec3& get_up(){ return up;}

	//! revert to the default tetris view
	void normal_view();
	//! change to looking at the board from the right
	void right_side_view();
	//! change to looking at the board from the left
	void left_side_view();
	//! change to looking at the back side of the board
	void behind_view();
	//! change to looking at the board on your head
	void upside_down_view();
	//! return the integer that indicates the current camera state
	unsigned int get_view(){ return true;}

  private:

	//! keep track of which view the user is currently experiencing
	/*! 1 = right side view
	 *  2 = left side view
	 *  3 = behind view
	 *  4 = upside down view
	 *************************************************************/
	unsigned int view;

	//! store the camera's location
	glm::vec3 eye;

	//! store where the camera is looking
	glm::vec3 looking;

    //! store what direction is 'up' relative to where the camera is looking
	glm::vec3 up;
};









