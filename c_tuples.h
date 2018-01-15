//! \file c_tuples.h creates some simple structs for convenience

#pragma once

#include<string>

//should always be 4 or it ain't Tetris
#define TET_SIZE 4

//! argument for asking the shader class to load & use a shader
struct shader_request{
	shader_request(const std::string& file_name_in = "no_name",
				   const std::string& type_in = "bad_type"){
		file_name = file_name_in;
		type      = type_in;
	}
	std::string file_name;
	std::string type;
};

//! argument for functions that have to do with creating game pieces
struct array_indices{
	array_indices(unsigned int column_index = 0, unsigned int row_index = 0){
		column = column_index;
		row    =    row_index;

	}


	unsigned int column;
	unsigned int row;

};

//! bag of 4 array_indices args, used to create pieces
struct tet_request{
	tet_request(const array_indices& cube1, const array_indices& cube2,
				const array_indices& cube3, const array_indices& cube4){
		cubes[0] = cube1;
		cubes[1] = cube2;
		cubes[2] = cube3;
		cubes[3] = cube4;		
	}
	
	array_indices cubes[TET_SIZE];
};



