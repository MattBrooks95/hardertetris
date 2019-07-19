#pragma once

#include<vector>
#include<string>
#include<fstream>

#include "c_tuples.h"

#define SHADER_PATH "./shaders/"

//! \struct shader will hold the variables associated with a loaded shader
struct shader{
	shader(std::string file_name_in = "no name", std::string type_in = "no type",
		   std::string source_in = "");

	std::string file_name;
	std::string type_string;
	GLenum type;
	std::string source;
	GLint shader_index;
};

//! \class shaders handles the loading and compiling of shaders
/*! It will also save the global variable indices used to index
 *shader arguments */
class shaders{
  public:
	shaders();
	//~shaders();

	//! this function takes a list of shader file names and fills shader_list
	void init_shaders(const std::vector<shader_request>& file_names);

	//! this function is a helper for the init_shaders member
	void init_shaders_source(const shader_request& file_name);

	//! compile the shaders on the gpu
	void compile_shaders();

	//! link the shaders
	void link_shaders();

	//! find the attribute arrays on the gpu
	void check_attribs();

	//! find the indices for the uniform arguments to shader
	void check_uniforms();

	//! this function prints the shader's info without their source
	void print_shaders();
	//! this function prints all of the shader's infos
	void print_shaders_source();

	//! getter for the GL program
	GLuint get_program(){   return program;}
	//! getter for the vertex position attrib index
	GLint get_v_pos(){     return v_pos_loc;}
	//! getter for the gpu color attrib index
	GLint get_gpu_color(){ return gpu_color;}

	//! returns the uniform reference index for model_view
	GLint get_model_view(){  return model_view;}
	//! returns the uniform reference index for camera_view
	GLint get_cam_view(){    return camera_view;}
	//! returns the uniform reference index for perspective
	GLint get_perspective(){ return perspective;}

  private:

	//! save the index reference to this program
	GLuint program;

	//! save index reference to location vertices
	GLint v_pos_loc;

	//! save index reference to color vertices
	GLint gpu_color;

	//! save the index for shader argument model view
	GLint model_view;

	//! save the index for shader argument camera view
	GLint camera_view;

	//! save the index for shader argument perspective
	GLint perspective;


	//! vector of the info used to make shaders
	std::vector<shader> shader_list;
};



