//! \file shaders.cc implements the functions declared in shaders.h

#include "glut_handler.h"

#include "shaders.h"

using namespace std;

//################ STRUCT TO STORE A SINGLE SHADER ###################
shader::shader(string file_name_in, string type_in, string source_in){
	file_name    = file_name_in;
	type_string  = type_in;
	source       = source_in;
	shader_index = 0;

}
//####################################################################

//################ SHADER MANAGER ####################################
//######################################################################################
shaders::shaders(){
	program   =  0;
	v_pos_loc = -1;
	gpu_color = -1;
}
//######################################################################################

//######################################################################################
void shaders::init_shaders(const std::vector<shader_request>& requests){

	//yank the shader sources from the given list of file names
	for(unsigned int c = 0; c < requests.size(); c++){
		init_shaders_source(requests[c]);
	}

	//compile the shaders now in shader_list
	compile_shaders();

	//link the shaders in shader_list using their indices
	link_shaders();

	//check that the attribute locations are found
	check_attribs();

	//check that the uniform locations are found
	check_uniforms();

}
//######################################################################################

//######################################################################################
void shaders::init_shaders_source(const shader_request& request_in){

	//cout << "Making shader: " << request_in.file_name << endl;

	//put a shader struct in the vector
	//to be filled in later
	shader push_me;
	shader_list.push_back(push_me);

	//open up the file
	ifstream ins;
	string shader_path = SHADER_PATH;
	string open_me = shader_path + request_in.file_name;

	ins.open(open_me.c_str(),ios::binary);	
	if(ins.fail()){
		cout << "Failure to open shader file with path: " << open_me << endl;
		exit(-1);
	}

	//thanks to www.cplusplus.com/reference/istream/istream/read/

	//figure out how large file is
	ins.seekg(0, ins.end);
	unsigned int length = ins.tellg();
	//cout << "It has " << length << " characters!" << endl;
	ins.seekg(0, ins.beg);

	//read that file into a c character array
	char* file_contents = new char[length+1];
	ins.read(file_contents,length);

	//put a newline in to cap off the array, ins.read does not do this
	//automatically, idk why \n doesn't work here
	file_contents[length] = '\0';
	//cout << file_contents;
	//set the shader's file name variable
	shader_list.back().file_name = request_in.file_name;

	//save the string representation of what this kind of shader is
	//supposed to do
	shader_list.back().type_string = request_in.type;

	//depending on argument type, set the GLenum type for this shader
	if(request_in.type.compare("vshader") == 0){

		shader_list.back().type = GL_VERTEX_SHADER; 

	} else if(request_in.type.compare("fshader") == 0){

		shader_list.back().type = GL_FRAGMENT_SHADER;
	}

	//put the source in the shader struct
	shader_list.back().source   = file_contents;

	delete[] file_contents;

	ins.close();
}
//######################################################################################

//######################################################################################
void shaders::compile_shaders(){
	//thanks to 
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/#shaders
	//cout << "SHADERS VEC SIZE: " << shader_list.size() << endl;
	for(unsigned int c = 0; c < shader_list.size(); c++){

		//cout << "Compiling shader: " << shader_list[c].file_name << endl;
		GLuint shader_id;

		if(shader_list[c].type_string.compare("vshader") == 0){
			shader_id = glCreateShader(GL_VERTEX_SHADER);
		} else if(shader_list[c].type_string.compare("fshader") == 0){
			shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		}

		GLint result = GL_FALSE;
		int info_length;

		char const * source_ptr = shader_list[c].source.c_str();
		glShaderSource(shader_id,1,&source_ptr,NULL);
		glCompileShader(shader_id);
		glGetShaderiv(shader_id,GL_COMPILE_STATUS,&result);
		glGetShaderiv(shader_id,GL_INFO_LOG_LENGTH,&info_length);

		if(result == GL_TRUE){

			//cout << "Shader succesful." << endl;
			shader_list[c].shader_index = shader_id;

		} else {
			vector<char> shader_error(info_length+1);			
			//cout << "Shader compilation unsuccessful." << endl;
			glGetShaderInfoLog(shader_id,info_length,NULL,&shader_error[0]);
			printf("%s\n",&shader_error[0]);
		}
	}
}
//######################################################################################

//######################################################################################
void shaders::link_shaders(){
	//cout << "in link shaders" << endl;

	//create a program on the gpu and save it's id in this class'
	//program variable
	program = glCreateProgram();

	//go through and attach all the shaders
	for(unsigned int c = 0; c < shader_list.size(); c++){
		glAttachShader(program,shader_list[c].shader_index);
	}

	//link the program
	glLinkProgram(program);

	//check for errors
	GLint result = GL_FALSE;
	int info_length;

	glGetProgramiv(program,GL_LINK_STATUS,&result);
	glGetProgramiv(program,GL_INFO_LOG_LENGTH,&info_length);

	if(result == GL_TRUE){

		//cout << "Successfully linked." << endl;

	} else {
		vector<char> link_error(info_length+1);
		glGetProgramInfoLog(program,info_length,NULL,&link_error[0]);
		printf("Linking errors: %s\n",&link_error[0]);
	}


	glUseProgram(program);
	//cout << "Program: " << program << endl;

	//I don't understand why we detach and delete the shaders
	//here, but that's what the guides say
	/*for(unsigned int c = 0; c < shader_list.size(); c++){
		glDetachShader(program,shader_list[c].shader_index);
		glDeleteShader(shader_list[c].shader_index);
	}*/

}
//######################################################################################

//######################################################################################
void shaders::check_attribs(){

	v_pos_loc = glGetAttribLocation(program,"vPosition");
	if(v_pos_loc == -1){
		cout << "Error grabbing vPosition location on the gpu." << endl;
	}

	gpu_color = glGetAttribLocation(program,"vColor");
	if(gpu_color == -1){
		cout << "Error grabbing vColor location on the gpu." << endl;
	}

}
//######################################################################################

//######################################################################################
void shaders::check_uniforms(){

	camera_view = glGetUniformLocation(program,"camera_view");
	if(camera_view == -1){
		cout << "Error, could not find uniform camera_view in the shader." << endl;
	}

	model_view = glGetUniformLocation(program,"model_view");
	if(model_view == -1){
		cout << "Error, could not find uniform model_view in the shader." << endl;
	}

	perspective = glGetUniformLocation(program,"perspective");
	if(perspective == -1){
		cout << "Error, could not find uniform perspective in the shader." << endl;
	}

}
//######################################################################################

//######################################################################################
void shaders::print_shaders(){

//	for(unsigned int c = 0; c < shader_list.size();c++){
//		cout << "File name: "   << shader_list[c].file_name << endl;
//		cout << "Type: "        << shader_list[c].type << endl;
//		cout << "Type string: " << shader_list[c].type_string << endl;
//		cout << "GL index: "    << shader_list[c].shader_index << endl;
//	}

}
//######################################################################################

void shaders::print_shaders_source(){

//	for(unsigned int c = 0; c < shader_list.size();c++){
//		cout << "File name: " << shader_list[c].file_name << endl;
//		cout << "Type: "      << shader_list[c].type << endl;
//		cout << "Type string: " << shader_list[c].type_string << endl;
//		cout << "GL index: "    << shader_list[c].shader_index << endl;
//		cout << "Source: \n|"    << shader_list[c].source << "|" << endl;
//	}


}
//######################################################################################



