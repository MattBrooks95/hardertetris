//! \file cube.cc contains the implementations of the functions declared in cube.h

#include "glut_handler.h"
#include "cube.h"
#include "shaders.h"
#include "game.h"
#include "camera.h"

using namespace std;

extern glut_handler* glut_access;
extern shaders* shader_access;
extern game* game_access;
extern camera* gaze_access;

//######################################################################################
cube::cube(){
	my_vao    = 0;
	my_buffer = 0;
}
//######################################################################################

//######################################################################################
void cube::init(const string& color,const glm::vec3& my_location,
				const array_indices& indices_in){
	//save the logical location for this cube in the game
	my_indices = indices_in;


	//make the vertex array
	glGenVertexArrays(1,&my_vao);

	//bind it
	glBindVertexArray(my_vao);

	//create the vertex buffer
	glGenBuffers(1,&my_buffer);

	//bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER,my_buffer);

	//set the class's variables for center point
	set_center_pt(my_location);

	//make data on cpu by building vertices about the center point
	build_vertices();

	//make color array of passed color
	make_colors(color);

	//send data to gpu
    int total_size = sizeof(glm::vec4) * (my_points.size() + my_colors.size());
	glBufferData(GL_ARRAY_BUFFER,total_size,NULL, GL_STATIC_DRAW);

    //send over the points
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec4) * my_points.size(),my_points.data());

    //send over the point's colors
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(glm::vec4) * my_points.size(),
									sizeof(glm::vec4) * my_colors.size(),my_colors.data());

	glEnableVertexAttribArray(shader_access->get_v_pos());
	//it is very important that arg 2 is 4 here instead of 2, b/c
	//now using vec4's for point location
	glVertexAttribPointer(shader_access->get_v_pos(),4,GL_FLOAT,GL_FALSE,0,0);

	glEnableVertexAttribArray(shader_access->get_gpu_color());
	glVertexAttribPointer(shader_access->get_gpu_color(),4,GL_FLOAT,GL_FALSE,0,
						(GLvoid*)(sizeof(glm::vec4) * my_colors.size())  );
}
//######################################################################################

//######################################################################################
void cube::set_center_pt(const glm::vec3& location){
	center_pt = location;

	//I guess the OpenGL spec changed, so this code just caused the matrix
	// to be full of 0's, because the default mat4 is an all 0 matrix and not
	// the identity as it was when I made this project
	//THANK YOU https://community.khronos.org/t/glm-translate/75326

	// my_model_view = glm::translate(glm::mat4(),center_pt);
	my_model_view = glm::translate(glm::mat4(1),center_pt);
}
//######################################################################################

string cube::get_center_point(){
	return to_string(center_pt.x) + " " + to_string(center_pt.y) + " " + to_string(center_pt.z);
}

//######################################################################################
void cube::build_vertices(){
	//front face
	my_points.push_back(glm::vec4(    0.5,     0.5, 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5,     0.5, 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5, 0.5 , 1.0));
	// triangle divider
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5, 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5,     0.5, 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5, 0.5 , 1.0));


	//back face - this is front face with - z
	my_points.push_back(glm::vec4(    0.5,     0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5,     0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5, 0 - 0.5 , 1.0));
	// triangle divider
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5,     0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5, 0 - 0.5 , 1.0));


	//left face
	my_points.push_back(glm::vec4(0 - 0.5,     0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5,     0.5, 0 - 0.5 , 1.0));
	//triangle divider
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5,     0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5, 0 - 0.5 , 1.0));


	//right face - left face with + x
	my_points.push_back(glm::vec4( 0.5,     0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4( 0.5, 0 - 0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4( 0.5,     0.5, 0 - 0.5 , 1.0));
	//triangle divider
	my_points.push_back(glm::vec4( 0.5, 0 - 0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4( 0.5,     0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4( 0.5, 0 - 0.5, 0 - 0.5 , 1.0));


	//top face
	my_points.push_back(glm::vec4(0 - 0.5,  0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5,  0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5,  0.5,     0.5 , 1.0));
	//triangle divider
	my_points.push_back(glm::vec4(0 - 0.5, 0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5, 0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0.5,     0.5 , 1.0));


	//bottom face - top face with neg y values
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5,     0.5 , 1.0));
	//triangle divider
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5, 0 - 0.5 , 1.0));
	my_points.push_back(glm::vec4(0 - 0.5, 0 - 0.5,     0.5 , 1.0));
	my_points.push_back(glm::vec4(    0.5, 0 - 0.5,     0.5 , 1.0));

}
//######################################################################################

//######################################################################################
void cube::make_colors(const string& color){
	glm::vec4 base_color;

    //note these colors are defined in colors.h
	//cout << "Make colors arg: " << color << endl;
	if(color.compare("blue_green")     == 0){
		base_color = BLUE_GREEN;
	} else if(color.compare("purple")  == 0){
		base_color = PURPLE;
	} else if(color.compare("yellow")  == 0){
		base_color = YELLOW;
	} else if(color.compare("white")   == 0){
		base_color = WHITE;
	} else if(color.compare("pink")    == 0){
		base_color = PINK;
	} else if(color.compare("blue")    == 0){
		base_color = BLUE;
	} else if(color.compare("red")     == 0){
		base_color = RED;
	} else if(color.compare("green")   == 0){
		base_color = GREEN;
	} else if(color.compare("orange")  == 0){
        base_color = ORANGE;
    } else {
        cout << "Unrecognized color specifier: " << color << endl;
        exit(-1);
    }

	for(unsigned int c = 0; c < my_points.size();c++){
		my_colors.push_back(base_color);
	}
}
//######################################################################################

//######################################################################################
void cube::draw_me(){
	//bind this cube's vertex array object
	glBindVertexArray(my_vao);

	//move vertices to simulate the camera being at eye, and looking at looking, with respect to current up
	glm::mat4 pass_cam_view = glm::lookAt(gaze_access->get_eye(),gaze_access->get_looking(),gaze_access->get_up());
	glUniformMatrix4fv(shader_access->get_cam_view(),1,GL_FALSE,&pass_cam_view[0][0]);

	// std::cout << glm::to_string(my_model_view) << std::endl;

	//move vertices relative to the model view
	glUniformMatrix4fv(shader_access->get_model_view(),1,GL_FALSE,&my_model_view[0][0]);

	//calculate aspect ratio for perspective
	GLfloat width  = GLfloat(glut_access->get_win_info().w);
	GLfloat height = GLfloat(glut_access->get_win_info().h);

	//create perspective matrix
	GLfloat pass_aspect = width / height;
	glm::mat4 pass_perspective = glm::perspective((GLfloat)glm::radians(45.0),pass_aspect,(GLfloat) ZNEAR,(GLfloat) ZFAR);
	glUniformMatrix4fv(shader_access->get_perspective(),1,GL_FALSE,&pass_perspective[0][0]);

	glEnable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLES,0,my_points.size());
	// glDrawArrays(GL_POINTS,0,my_points.size());
}
//######################################################################################
