#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>
#include<string>


#include "colors.h"
#include "c_tuples.h"

//! cube handles the geometry and drawing of the cubes that make up game pieces
class cube{
  public:
	cube();
	//~cube();

	//! initialize this cube
	void init(const std::string& color,const glm::vec3& my_location,
			  const array_indices& indices_in);

	//! this function sets up the center point for this cube
	void set_center_pt(const glm::vec3& location);

	//! set up the vertices for a cube at the origin
	void build_vertices();

	//! set up the colors for this cube
	void make_colors(const std::string& color);

	//! draw this cube when commanded
	void draw_me();

	std::string get_center_point();

	//! keep track of there this cube's boolean is in the game state boolean array
	array_indices my_indices;

  private:
	//! store index to access vertex array on gpu
	GLuint my_vao;

	//! store index to buffer that can send data to gpu
	GLuint my_buffer;

	//! keep track of where this thing's center point is
	glm::vec3 center_pt;

	//! model view matrix made up of any scaling and translation to center point
	glm::mat4 my_model_view;

	//! store array of vertex locations
	std::vector<glm::vec4> my_points;

	//! store array of colors
	std::vector<glm::vec4> my_colors;
};



