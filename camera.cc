//! \file camera.cc describes the functions declared in camera.h

#include "camera.h"

//######################################################################################
camera::camera(){

	eye     = glm::vec3(CAM_START);
	up      = glm::vec3(0.0, 1.0, 0.0);
	looking = glm::vec3(LOOK_START);

	//start off with the default view
	view    = 0;
}
/*
camera::~camera(){

}
*/
//######################################################################################
void camera::normal_view(){
	//move camera to the normal position
	eye     = glm::vec3(CAM_START);
	up      = glm::vec3(0.0,1.0,0.0);
	looking = glm::vec3(LOOK_START);
}

//######################################################################################
void camera::right_side_view(){
	normal_view();
	view   = 1;
	//move camera to the right side
	eye.x +=  35.0;
	eye.y +=   0.0;
	eye.z -=  31.0;
}
//######################################################################################

//######################################################################################
void camera::left_side_view(){
	normal_view();
	view   = 2;
	//move camera to the left side
	eye.x -= 35.0;
	eye.y +=  0.0;
	eye.z -= 31.0;
}
//######################################################################################

//######################################################################################
void camera::behind_view(){
	normal_view();
	view   = 3;
	//move camera to be behind the board
	eye.x +=  0.0;
	eye.y +=  0.0;
	eye.z -= 60.0;
}
//######################################################################################

//######################################################################################
void camera::upside_down_view(){
	normal_view();
	view = 4;
	//play like you're standing on your head
	up.x =  0.0;
	up.y = -1.0;
	up.z =  0.0;
}
//######################################################################################






