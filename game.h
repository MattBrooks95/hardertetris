//! \file game.h

#pragma once

#include<glm/glm.hpp>   //matrix and vector types
#include<glm/gtx/string_cast.hpp>  //easy printing of vectors and matrices
#include<vector>
#include<list>
#include<queue>
#include<iostream>

#include<cstdlib>

#include "tetromino.h"
#include "c_tuples.h"
//################## DEFAULT STATE DEFINITIONS ###################
#define DEFAULT_MATRIX_COLS 10
#define DEFAULT_MATRIX_ROWS 20
#define DEFAULT_CUBE_RADIUS 0.5
#define DEFAULT_BOARD_HOME {-10.0,0.0,-1.0}
#define BOUNDING_COLOR 111.0f, 93.0f, 216.0f, 1.0f
#define BORDER_VERTICES 16
#define GUIDE_VERTICES 56
//################################################################

/*! this enum makes the switch statement for piece generation
 *easier to read */
enum tet_type{
	t_tet,
	l_z_tet,
	r_z_tet,
	I_tet,
	l_l_tet,
	r_l_tet,
	square_tet
};

//! this enum is stored in the user input queue, and tells the game what to do
enum move_type{
	move_left,
	move_right,
	rot_right,
	rot_left,
	medium_drop,
	fast_drop
};

//! \class game handles the game state, and manipulates the pieces
/*! it also draws the box that contains the 'game area' */
class game{
  public:
	//! sets up the start state for the game
	game();
	//~game();

	//! (re)sizes the board
	/*! it defaults to 10 cols x 20 rows from constructor, and will
	 *likely not be changed unless I have ample time to experiment
	 *with different sized boards */
	void size_board(unsigned int columns,unsigned int rows);

	//! calculate the center points for the positions on the board
	void calculate_positions();

	//! draws the floor of the game world
	void make_scene();

	//! helper for make_scene, puts in vertices for terrain
	void build_scene_vertices();

	//! helper for make_scene, puts in line vertices for illustrating the rows and columns
	/*! these vertices are bussed over with the scene vertices, and will be at the end of
	 *the vertex array */
	void build_board_vertices();

	//! print the 2d array of game logic booleans
	void print_game_bools();

	//! print the 2d array of center point locations
	void print_positions();

	//! this function generates a new tetromino at the top center of the board
	/* it places it into the active_game_pieces.list */
	bool new_active_piece();

	//! this function is a helper for new_active_piece
	/*! it prepares the tet_request argument for instantiating a new piece
	 *according to a string flag that describes what kind of piece it is*/
	void set_tet_loc(tet_request*& fill_me,std::string type);

	//! this helper for new_active_piece makes sure the piece can be created
	/*! if this returns false, the game should end */
	bool can_make_piece(tet_request* check_me);
	//! this function changes the game state bools when a new piece is created
	void new_piece_bools(tet_request* new_tet_loc);

	//! this function draws the "scene", like the floor or any walls
	void draw_scene();

	//! this function draws all active cubes on the board
	void draw_cubes();

	//! return the vector of center point locations for the game positions
	const std::vector<std::vector<glm::vec3>>& get_game_locs(){
		return center_points;
	}

	//! getter for boolean array
	const std::vector<std::vector<bool>>& get_const_board_state(){
		return board_state;
	}

	//! non-const getter for boolean array
	std::vector<std::vector<bool>>& get_board_state(){
		return board_state;
	}

	//############################################################################
	//! this is the function that implements game logic
	/*! it is called from the glut handler's timer func */
	void calc_game_state();
	//! this function moves the active tetromino per user input
	void move_tet(tetromino* move_me);
	//! this function deletes cubes off of the board when rows are completed
	bool clear_rows(std::vector<int>& clear_indices);
	//! this function cleans out tetrominos whose cubes have all been killed
	void clean_list();
	//! this function moves all of the pieces above the removed down rows down
	/*! this is necessary because the usual falling mechanics don't work in
	 *this case. In tetris, they don't fall down as far as they can, they
	 *only fall down as many rows as was clears below them, to preserve gaps*/
	void move_down_after_clear(std::vector<int>& clear_indices);
	//! helper for move_down_after_clear
	/*! moves all cubes in a tet down one, if that cube is above the index
	 *of a culled row that is provided as an argument */
	void shift_down_one(tetromino* shift_me,int row_index);
	//! this function implements the "falling" of the game pieces
	bool gravity();
	//! this function is a helper for gravity(), that moves a given tetromino
	bool fall(tetromino* move_me);

	//! move active piece left, if possible
	void move_piece_left(tetromino* move_me);
	//! move active piece right, if possible
	void move_piece_right(tetromino* move_me);
	//! move active piece down twice
	void drop_piece(tetromino* move_me);
	//! drop active piece to very bottom
	void fast_drop_piece(tetromino* move_me);
	//############################################################################

	//! keep track of user inputs, note that queues are first-in-first-out
	std::queue<move_type> move_list;

	//! toggle to true to pause the game, and false to unpause the game
	bool paused;

  private:

	//! keep track of the vertices for the scene geometry
	GLuint my_vao;
	//! buffer to send scene vertices to gpu
	GLuint my_buffer;
	//! vector of the vertices that the game itself uses to draw things
	std::vector<glm::vec4> scene_points;
	//! vector of the vertex colors that the game itself uses to color the scene
	std::vector<glm::vec4> scene_colors;

	//! board_home is the location of the very left corner of the board
	glm::vec3 board_home;

	//! the size of the cubes defines the board cell sizes
	GLfloat cube_radius;

	//! this array of booleans keeps track of which cells are 'occupied'
	std::vector<std::vector<bool>> board_state;

	//! this array is a lookup table of center positions for cubes to use
	std::vector<std::vector<glm::vec3>> center_points;

	//! list of active tetrominos
	std::list<tetromino*> active_game_pieces;

	//! this bool keeps track of when the game should generate
	/*! a new game piece. For instance, when no game piece can fall
	 *further */
	bool make_piece;

	//! toggle to true when the player loses the game
	bool lost;
};




