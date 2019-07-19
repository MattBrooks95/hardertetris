//! \file game.cc houses the implementations for the class found in game.h


#include "game.h"
#include "camera.h"
#include "shaders.h"
#include "glut_handler.h"


extern glut_handler* glut_access;
extern shaders* shader_access;
extern camera* gaze_access;

using namespace std;
using glm::vec4;
//######################################################################################
game::game(){

	//place the board in the world, by specifying it's left
	//corner's location
	board_home = DEFAULT_BOARD_HOME;

	//set up the 2d array of game positions
	size_board(DEFAULT_MATRIX_COLS,DEFAULT_MATRIX_ROWS);

	//set up the radius for all the cubes
	cube_radius = DEFAULT_CUBE_RADIUS;

	//calculate the positions for the center points of the game
	calculate_positions();

	my_vao    = 0;
	my_buffer = 0;
	lost      = false;
	paused    = false;
}
//######################################################################################

/*
game::~game(){

}
*/

//######################################################################################
void game::size_board(unsigned int columns, unsigned int rows){

	//set up the columns of the board state
	board_state.resize(columns);

	//set up the width of the board state
	for(unsigned int c = 0; c < board_state.size(); c++){
		board_state[c].resize(rows);
	}

	//make the vector of center points whose indices
	//line up with the boolean vector's
	center_points.resize(columns);

	for(unsigned int c = 0; c < center_points.size(); c++){
		center_points[c].resize(rows);
	}

}
//######################################################################################

//######################################################################################
void game::calculate_positions(){

	for(unsigned int c = 0; c < center_points.size(); c++){

		GLfloat this_col_x = c * (2 * cube_radius) + cube_radius + board_home.x;

		for(unsigned int d = 0; d < center_points[c].size(); d++){
			center_points[c][d].x = this_col_x;
			center_points[c][d].y = board_home.y + cube_radius +
									(2 * cube_radius) * ((center_points[c].size() - 1) - d);
			center_points[c][d].z = board_home.z - cube_radius;
		}

	}

}
//######################################################################################

//######################################################################################
void game::make_scene(){

	//make the vertex array
	glGenVertexArrays(1,&my_vao);

	//bind it
	glBindVertexArray(my_vao);

	//create the vertex buffer
	glGenBuffers(1,&my_buffer);

	//bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER,my_buffer);

	//make data on cpu by building vertices about the center point
	build_scene_vertices();

	//make the lines for the game board
	build_board_vertices();

	//send data to gpu
    int total_size = sizeof(vec4) * (scene_points.size() + scene_colors.size());
	glBufferData(GL_ARRAY_BUFFER,total_size,NULL, GL_STATIC_DRAW);

    //send over the points
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vec4) * scene_points.size(),scene_points.data());

    //send over the point's colors
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec4) * scene_points.size(),
									sizeof(vec4) * scene_colors.size(),scene_colors.data());

	glEnableVertexAttribArray(shader_access->get_v_pos());
	//it is very important that arg 2 is 4 here instead of 2, b/c
	//now using vec4's for point location
	glVertexAttribPointer(shader_access->get_v_pos(),4,GL_FLOAT,GL_FALSE,0,0);

	glEnableVertexAttribArray(shader_access->get_gpu_color());
	glVertexAttribPointer(shader_access->get_gpu_color(),4,GL_FLOAT,GL_FALSE,0,
						(GLvoid*)(sizeof(vec4) * scene_colors.size()));

}
//######################################################################################

//######################################################################################
void game::build_scene_vertices(){
	//triangles to draw the 'ground'
	scene_points.push_back(vec4( 40.0f, -1.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, -1.0f, -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f, -40.0f, 1.0f));

	scene_points.push_back(vec4(-40.0f, -1.0f, -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, -1.0f,  40.0f, 1.0f));

	//triangles for the top
	scene_points.push_back(vec4( 40.0f, 39.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, 39.0f, -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, 39.0f, -40.0f, 1.0f));

	scene_points.push_back(vec4(-40.0f, 39.0f, -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, 39.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, 39.0f,  40.0f, 1.0f));

	//triangles for the back
	scene_points.push_back(vec4( 40.0f, 39.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, 39.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  -40.0f, 1.0f));

	scene_points.push_back(vec4(-40.0f, 39.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, -1.0f,  -40.0f, 1.0f));

	//triangles for the front
	scene_points.push_back(vec4( 40.0f, 39.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, 39.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  40.0f, 1.0f));

	scene_points.push_back(vec4(-40.0f, 39.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  40.0f, 1.0f));
	scene_points.push_back(vec4(-40.0f, -1.0f,  40.0f, 1.0f));


	//triangles for right
	scene_points.push_back(vec4( 40.0f, 39.0f,   40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,   40.0f, 1.0f));

	scene_points.push_back(vec4( 40.0f, 39.0f,   40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, -1.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4( 40.0f, 39.0f,  -40.0f, 1.0f));

	//triangles for left
	scene_points.push_back(vec4( -40.0f, 39.0f,   40.0f, 1.0f));
	scene_points.push_back(vec4( -40.0f, -1.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4( -40.0f, -1.0f,   40.0f, 1.0f));

	scene_points.push_back(vec4( -40.0f, 39.0f,   40.0f, 1.0f));
	scene_points.push_back(vec4( -40.0f, -1.0f,  -40.0f, 1.0f));
	scene_points.push_back(vec4( -40.0f, 39.0f,  -40.0f, 1.0f));


	//ground's color
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));

	//cieling's color
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));

	//back color
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));

	//front color
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));

	//right color
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));

	//left color
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
	scene_colors.push_back(vec4(BOUNDING_COLOR));
}
//######################################################################################

//######################################################################################
void game::build_board_vertices(){

	GLfloat cube = cube_radius * 2;

	//lines for the board's perimeter
	scene_points.push_back(vec4(board_home.x, board_home.y,                board_home.z-cube,1.0f));//1
	scene_points.push_back(vec4(board_home.x, board_home.y+40*cube_radius, board_home.z-cube,1.0f));//2
	scene_points.push_back(vec4(board_home.x, board_home.y,                board_home.z-cube,1.0f));//3
	scene_points.push_back(vec4(0.0,          board_home.y,                board_home.z-cube,1.0f));//4
	scene_points.push_back(vec4(0.0,          board_home.y+40*cube_radius, board_home.z-cube,1.0f));//5

	scene_points.push_back(vec4(board_home.x, board_home.y+40*cube_radius, board_home.z-cube,1.0f));//6
	scene_points.push_back(vec4(board_home.x, board_home.y+40*cube_radius, board_home.z,     1.0f));//7
	scene_points.push_back(vec4(board_home.x, board_home.y,                board_home.z,     1.0f));//8
	scene_points.push_back(vec4(board_home.x, board_home.y+40*cube_radius, board_home.z,     1.0f));//9
	scene_points.push_back(vec4(0.0,          board_home.y+40*cube_radius, board_home.z,     1.0f));//10

	scene_points.push_back(vec4(0.0,          board_home.y+40*cube_radius, board_home.z-cube,1.0f));//11
	scene_points.push_back(vec4(0.0,          board_home.y+40*cube_radius, board_home.z,     1.0f));//12
	scene_points.push_back(vec4(0.0,          board_home.y,                board_home.z,     1.0f));//13
	scene_points.push_back(vec4(0.0,          board_home.y,                board_home.z-cube,1.0f));//14
	scene_points.push_back(vec4(0.0,          board_home.y,                board_home.z,     1.0f));//15

	scene_points.push_back(vec4(board_home.x, board_home.y,                board_home.z,1.0f));//16
	//##################################################################################

	//lines for the board's guides

	//10 columns
	GLfloat x_val = board_home.x+cube;
	for(unsigned int c = 0; c < 9; c++){
		scene_points.push_back(vec4(x_val, 0.0f,           board_home.z - cube_radius,  1.0f));
		scene_points.push_back(vec4(x_val, 40*cube_radius, board_home.z - cube_radius,  1.0f));
		x_val += cube;
	}

	//20 rows
	GLfloat y_val = cube;
	for(unsigned int c = 0; c < 19; c++){
		scene_points.push_back(vec4(0.0,           y_val,   board_home.z - cube_radius, 1.0f));
		scene_points.push_back(vec4(board_home.x,  y_val,   board_home.z - cube_radius, 1.0f));
		y_val += cube;
	}

	//black lines for the board
	for(unsigned int c = 0; c < BORDER_VERTICES+GUIDE_VERTICES; c++){
		scene_colors.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

}

//######################################################################################
void game::draw_scene(){
	//bind this cube's vertex array object
	glBindVertexArray(my_vao);

	//move vertices to simulate the camera being at eye, and looking at looking, with respect to current up
	glm::mat4 pass_cam_view = glm::lookAt(gaze_access->get_eye(),gaze_access->get_looking(),gaze_access->get_up());
	glUniformMatrix4fv(shader_access->get_cam_view(),1,GL_FALSE,&pass_cam_view[0][0]);

	//move vertices relative to the model view
	glm::mat4 identity(1.0);
	glUniformMatrix4fv(shader_access->get_model_view(),1,GL_FALSE,&identity[0][0]);

	//calculate aspect ratio for perspective
	GLfloat width  = GLfloat(glut_access->get_win_info().w);
	GLfloat height = GLfloat(glut_access->get_win_info().h);

	//create perspective matrix
	GLfloat pass_aspect = width / height;
	glm::mat4 pass_perspective = glm::perspective((GLfloat)glm::radians(45.0),pass_aspect,(GLfloat) ZNEAR,(GLfloat) ZFAR);
	glUniformMatrix4fv(shader_access->get_perspective(),1,GL_FALSE,&pass_perspective[0][0]);

	glEnable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLES,0,scene_points.size()-BORDER_VERTICES-GUIDE_VERTICES);
	glLineWidth(3.0);
	glDrawArrays(GL_LINE_LOOP,scene_points.size()-BORDER_VERTICES-GUIDE_VERTICES,BORDER_VERTICES);
	glLineWidth(1.0);
	glDrawArrays(GL_LINES,scene_points.size()-GUIDE_VERTICES,GUIDE_VERTICES);
}
//######################################################################################

//######################################################################################
void game::draw_cubes(){
	for(std::list<tetromino*>::iterator it = active_game_pieces.begin();
		it != active_game_pieces.end(); ++it){
		(*it)->draw_me();
	}
}
//######################################################################################

//######################################################################################
void game::calc_game_state(){
	if(paused){
		return;
	}

	if(lost){
		exit(EXIT_SUCCESS);
	}

	if(active_game_pieces.empty()){
		new_active_piece();
	}

	//the active game piece (the one that can still fall - last in list)
	//is moved according to recent user input
	//having this function be first should mean that the piece
	//can move after it has touched a surface that could stop it,
	//so long as the user keeps moving it
	move_tet(active_game_pieces.back());

	return;//remove - trying to make piece freeze until i get it drawing

	//have the pieces fall until they can fall no more
	//this function returns true if any falling was done,
	//and this function ends
	if(gravity()){
		return;
	}

	//this function clears rows off the board if they have been completed
	//if any rows are removed, end this function early
	vector<int> clear_indices;
	if(clear_rows(clear_indices)){
		print_game_bools();
		clean_list();
		move_down_after_clear(clear_indices);
	}

	//at this point, no cubes can fall, and no rows can be removed,
	//so a new piece needs created
	//if a piece can't be created, because there is no room, the
	//player loses the game
	if(!new_active_piece()){
		lost = true;
	}

	// print_game_bools();

}
//######################################################################################

//######################################################################################
void game::move_tet(tetromino* move_me){

	//leave if there is no input in the queue
	if(move_list.empty()){
		return;
	}

	//leave if there is no active piece
	if(active_game_pieces.empty()){
		return;
	}

	move_type curr_move = move_list.front();
	move_list.pop();
	switch(curr_move){

		case move_left:
			move_piece_left(move_me);
			break;

		case move_right:
			move_piece_right(move_me);
			break;

		case rot_right:
			move_me->rot_right();
			break;

		case rot_left:
			move_me->rot_left();
			break;

		case medium_drop:
			drop_piece(move_me);
			break;

		case fast_drop:
			fast_drop_piece(move_me);
			break;

		default:
			cout << "I don't know how this happened, but bad enum move_type in move_tet." << endl;
	}
}
//######################################################################################

//######################################################################################
bool game::clear_rows(vector<int>& clear_indices){
	for(unsigned int row = 0; row < board_state[0].size(); row++){
		bool delete_row = true;
		for(unsigned int col = 0; col < board_state.size(); col++){

			//if ANY boolean in this row is false, immediately stop
			//inspecting this row and move to the next
			if(!board_state[col][row]){

				//let the outer loop know to not cull this row
				delete_row = false;
				//leave this loop, start inspecting other rows
				break;
			}
		}
		if(delete_row){
			clear_indices.push_back(row);
		}
	}

	//leave and let calc_game_state know that no rows were removed
	if(clear_indices.size() == 0){
		return false;
	}

	//cout << "BEFORE CLEAR####################################" << endl;
	print_game_bools();
	//loop over the rows that need culled
	for(unsigned int c = 0; c < clear_indices.size(); c++){
		//cout << "CLEARING ROW: " << clear_indices[c] << endl;
		//make each cell in that row false
		for(unsigned int d = 0; d < board_state.size(); d++){
			board_state[d][clear_indices[c]] = false;
		}

		for(list<tetromino*>::iterator it = active_game_pieces.begin();
			it != active_game_pieces.end();
			++it){
			(*it)->kill_cubes(clear_indices[c]);
			//cout << "How do pieces know when they are culled?" << endl;
		}


	}
	//print_game_bools();

	return true;
}
//######################################################################################

//######################################################################################
void game::clean_list(){
	//bool killed = false;
	//cout << "LIST SIZE BEFORE: " << active_game_pieces.size();
	list<tetromino*>::iterator it = active_game_pieces.begin();
	while(it != active_game_pieces.end()){
		if((*it)->dead()){
			//cout << "KILLING: " << *it << endl;
			//killed = true;
			it= active_game_pieces.erase(it);
		} else {
			//cout << "Not killed: " << *it << endl;
			++it;
		}
	}
	//cout << "LIST SIZE AFTER: " << active_game_pieces.size();
	//if(killed){
	//	char junk; cin >> junk;
	//}

}
//######################################################################################

//######################################################################################
void game::move_down_after_clear(vector<int>& clear_indices){

	//note that the function that fills in clear_indices
	//(clear rows) walks the game board top to bottom,
	//this is handy because at each cleared row, we can make all the cubes above that
	//row fall by one. These cubes will also be caught by the rows deleted later,
	//and moved down by 1 again. This way, cubes are only moved down by the exact
	//number of row clears below them. So some cubes will fall more than others
	//depending on how convoluted the board state is
	for(unsigned int c = 0; c < clear_indices.size(); c++){

		//cout << "Clear indices: " << clear_indices[c] << endl;
		for(list<tetromino*>::iterator it = active_game_pieces.begin();
			it != active_game_pieces.end();
			++it){
			shift_down_one((*it),clear_indices[c]);
		}

	}

}
//######################################################################################

//######################################################################################
void game::shift_down_one(tetromino* move_me, int shift_index){

	int rows[TET_SIZE];
	int cols[TET_SIZE];

	//get handy access to the locatins of this piece's cubes
	for(unsigned int c = 0; c < TET_SIZE; c++){
		cols[c] = move_me->get_cubes()[c].my_indices.column;
		rows[c] = move_me->get_cubes()[c].my_indices.row;
	}

	//deactivate all cubes for this piece
	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(move_me->cube_status[c]){
			board_state[cols[c]][rows[c]] = false;
		}
	}

	//move all of the cubes that should be moved
	for(unsigned int c = 0; c < TET_SIZE; c++){
		//note that rows being less than the cleared row's index
		//means that is higher up in the game board
		if(move_me->cube_status[c] && rows[c] < shift_index){
			//cout << "ROW: " << rows[c] << endl;
			//cout << "COL: " << cols[c] << endl;
			move_me->get_cubes()[c].my_indices.row += 1;
			move_me->get_cubes()[c].set_center_pt(center_points[cols[c]][rows[c]+1]);
		}
	}

	//reactivate all of the cubes
	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(move_me->cube_status[c]){
			int new_col = move_me->get_cubes()[c].my_indices.column;
			int new_row = move_me->get_cubes()[c].my_indices.row;
			board_state[new_col][new_row] = true;
		}
	}
}
//######################################################################################

//######################################################################################
bool game::gravity(){
	bool return_me = false;

	if(fall(active_game_pieces.back())){
		return_me = true;
	}
	return return_me;
}
//######################################################################################

//######################################################################################
bool game::fall(tetromino* move_me){

	if(move_me == NULL){
		cout << "game::fall recieved null pointer as argument, "
			 << "something is broken." << endl;
		return true;
	}

	if(board_state.size() == 0){
		cout << "game::fall error, game boolean array has size 0." << endl;
		return true;
	}


	//unsigned int board_cols = board_state.size();
	unsigned int board_rows = board_state[0].size();

	//all of these flags need to be made true in order for this piece to fall
	bool box_good[TET_SIZE] = {false,false,false,false};

	for(unsigned int c = 0; c < TET_SIZE;c++){

		//if this cube is dead, make its bool true immediately, but
		//do not use it to set cubes above it to be true
		if(!move_me->cube_status[c]){
			box_good[c] = true;
			continue;
		}

		//this element has been made to be true, so don't bother checking it
		if(box_good[c]){
			continue;
		}

		//if this box is on the bottom of the board, it can't fall, and neither can any of the other
		//cubes in this piece
		if( (move_me->get_cubes()[c].my_indices.row) == board_rows-1){
			return false;
		}

		unsigned int this_cube_col = move_me->get_cubes()[c].my_indices.column;
		unsigned int this_cube_row = move_me->get_cubes()[c].my_indices.row;

		//if the box directly below this cube is occupied, this cube seemingly (for now)
		//can't fall
		if(board_state[this_cube_col][this_cube_row+1]){

			//go to next phase of outer loop
			continue;
		}

		//elsewise, this cube can fall, which means that every cube directly above it in this piece
		//can also fall
		for(unsigned int d = 0; d < TET_SIZE;d++){
			//no point in comparing this cube against itself
			if(d == c) continue;


			if(this_cube_col == move_me->get_cubes()[d].my_indices.column &&
				   this_cube_row >  move_me->get_cubes()[d].my_indices.row){

					box_good[d] = true;
			}
		}
		box_good[c] = true;
	}

	//if all the boxes can fall, make them move down one
	//cout << box_good[0] << box_good[1] << box_good[2] << box_good[3] << endl;
	if(box_good[0] && box_good[1] && box_good[2] && box_good[3]){

		//make all of their old positions false - and move them down a row
		if(move_me->cube_status[0]){
			board_state[move_me->get_cubes()[0].my_indices.column][move_me->get_cubes()[0].my_indices.row] = false;
			move_me->get_cubes()[0].my_indices.row += 1;
		}
		if(move_me->cube_status[1]){
			board_state[move_me->get_cubes()[1].my_indices.column][move_me->get_cubes()[1].my_indices.row] = false;
			move_me->get_cubes()[1].my_indices.row += 1;
		}
		if(move_me->cube_status[2]){
			board_state[move_me->get_cubes()[2].my_indices.column][move_me->get_cubes()[2].my_indices.row] = false;
			move_me->get_cubes()[2].my_indices.row += 1;
		}
		if(move_me->cube_status[3]){
			board_state[move_me->get_cubes()[3].my_indices.column][move_me->get_cubes()[3].my_indices.row] = false;
			move_me->get_cubes()[3].my_indices.row += 1;
		}

		//make all of their new positions true
		if(move_me->cube_status[0]){
			board_state[move_me->get_cubes()[0].my_indices.column][move_me->get_cubes()[0].my_indices.row] = true;
		}
		if(move_me->cube_status[1]){
			board_state[move_me->get_cubes()[1].my_indices.column][move_me->get_cubes()[1].my_indices.row] = true;
		}
		if(move_me->cube_status[2]){
			board_state[move_me->get_cubes()[2].my_indices.column][move_me->get_cubes()[2].my_indices.row] = true;
		}
		if(move_me->cube_status[3]){
			board_state[move_me->get_cubes()[3].my_indices.column][move_me->get_cubes()[3].my_indices.row] = true;
		}

		//let all of the cubes know their new position
		unsigned int col;
		unsigned int row;
		if(move_me->cube_status[0]){
			col = move_me->get_cubes()[0].my_indices.column;
			row = move_me->get_cubes()[0].my_indices.row;
			move_me->get_cubes()[0].set_center_pt(center_points[col][row]);
		}
		if(move_me->cube_status[1]){
			col = move_me->get_cubes()[1].my_indices.column;
			row = move_me->get_cubes()[1].my_indices.row;
			move_me->get_cubes()[1].set_center_pt(center_points[col][row]);
		}
		if(move_me->cube_status[2]){
			col = move_me->get_cubes()[2].my_indices.column;
			row = move_me->get_cubes()[2].my_indices.row;
			move_me->get_cubes()[2].set_center_pt(center_points[col][row]);
		}
		if(move_me->cube_status[3]){
			col = move_me->get_cubes()[3].my_indices.column;
			row = move_me->get_cubes()[3].my_indices.row;
			move_me->get_cubes()[3].set_center_pt(center_points[col][row]);
		}
		return true;
	}//end of 'moving the pieces' if block

	//if we make it here, the piece couldn't fall, so we should return false
	return false;

}
//######################################################################################

//######################################################################################
void game::print_game_bools(){

	if(board_state.size() == 0) return;

	for(unsigned int row = 0; row < board_state[0].size(); row++){
		for(unsigned int col = 0; col < board_state.size(); col++){
			cout << board_state[col][row] << " ";
		}
		cout << endl;
	}

	string separator = "###########################################";

	cout << separator << endl;

}
//######################################################################################

//######################################################################################
void game::print_positions(){

	if(center_points.size() == 0) return;

	for(unsigned int col = 0; col < center_points.size(); col++){
		for(unsigned int row = 0; row < center_points[0].size(); row++){
			cout << col << ":" << row << " ";
			cout << "|" << to_string(center_points[col][row]) << "|" << endl;

		}
	}

}
//######################################################################################

//######################################################################################
bool game::new_active_piece(){
	//need to be able to randomly select between 7 different pieces
	// int random_number = rand() % 7;
	int random_number = 6; //line for testing only a particular piece

	//color to make the next piece, fill in switch case
	string color;

	tetromino* new_tetromino = NULL;
	tet_request* new_tet_loc = NULL;

	//locations to give that piece, fill in switch case
	switch(random_number){
		case t_tet:
			color = "purple";
			set_tet_loc(new_tet_loc,"t_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new t_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;

			} else {
				return false;
			}

			break;

		case l_z_tet:
			color = "red";
			set_tet_loc(new_tet_loc,"l_z_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new l_z_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;

			} else {

				return false;

			}
			break;

		case r_z_tet:

			color = "green";
			set_tet_loc(new_tet_loc,"r_z_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new r_z_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;

			} else {
				return false;
			}
			break;

		case I_tet:

			color = "blue_green";
			set_tet_loc(new_tet_loc,"I_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new I_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;
			} else {
				return false;

			}
			break;

		case l_l_tet:
			//cout << "Making a left L block." << endl;
			color = "blue";
			set_tet_loc(new_tet_loc,"l_l_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new l_l_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;

			} else {

				return false;

			}
			break;

		case r_l_tet:
			//cout << "Making a right L block." << endl;
			color = "orange";
			set_tet_loc(new_tet_loc,"r_l_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new r_l_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;

			} else {

				return false;

			}
			break;

		case square_tet:
			//cout << "Making a square block." << endl;
			color = "yellow";
			set_tet_loc(new_tet_loc,"square_block");
			if(can_make_piece(new_tet_loc)){
				new_piece_bools(new_tet_loc);
				new_tetromino = new square_block;
				active_game_pieces.push_back(new_tetromino);
				active_game_pieces.back()->init(color,new_tet_loc);
				delete new_tet_loc;
				new_tetromino = NULL;
				return true;
			} else {
				return false;
			}
			break;

		default:
			cout << "Error! Illegal value given for random tetromino creation" << endl;
	}
	cout << "Shouldn't reach this point in new_active_piece, something is wrong." << endl;
	exit(-1);
}
//######################################################################################

//######################################################################################
void game::set_tet_loc(tet_request*& fill_me, string type){
	array_indices cube0;
	array_indices cube1;
	array_indices cube2;
	array_indices cube3;

	if(board_state.size() == 0){
		cout << "Error from game::set_tet_loc, game board not "
			 << "initialized yet." << endl;
	}
	unsigned int start_col = (board_state.size() / 2) - 1;


	if(type.compare("t_block") == 0){

		cube0.column = start_col;     cube0.row = 0;
		cube1.column = start_col;     cube1.row = 1;
		cube2.column = start_col - 1; cube2.row = 1;
		cube3.column = start_col + 1; cube3.row = 1;

	} else if(type.compare("l_z_block") == 0){

		cube0.column = start_col;     cube0.row = 0;
		cube1.column = start_col;     cube1.row = 1;
		cube2.column = start_col - 1; cube2.row = 0;
		cube3.column = start_col + 1; cube3.row = 1;

	} else if(type.compare("r_z_block") == 0){

		cube0.column = start_col;     cube0.row = 0;
		cube1.column = start_col;     cube1.row = 1;
		cube2.column = start_col + 1; cube2.row = 0;
		cube3.column = start_col - 1; cube3.row = 1;

	} else if(type.compare("I_block")   == 0){

		cube0.column = start_col;     cube0.row = 0;
		cube1.column = start_col - 1; cube1.row = 0;
		cube2.column = start_col + 1; cube2.row = 0;
		cube3.column = start_col + 2; cube3.row = 0;

	} else if(type.compare("l_l_block") == 0){

		cube0.column = start_col - 1;     cube0.row = 0;
		cube1.column = start_col - 1;     cube1.row = 1;
		cube2.column = start_col;         cube2.row = 1;
		cube3.column = start_col + 1;     cube3.row = 1;

	} else if(type.compare("r_l_block") == 0){

		cube0.column = start_col + 1; cube0.row = 0;
		cube1.column = start_col + 1; cube1.row = 1;
		cube2.column = start_col;     cube2.row = 1;
		cube3.column = start_col - 1; cube3.row = 1;

	} else if(type.compare("square_block") == 0){

		cube0.column = start_col;     cube0.row = 0;
		cube1.column = start_col;     cube1.row = 1;
		cube2.column = start_col + 1; cube2.row = 0;
		cube3.column = start_col + 1; cube3.row = 1;


	} else {
		cout << "Error! game::set_tet_loc given bad tetromino type argument."
			 << endl;
		return;
	}

	fill_me = new tet_request(cube0,cube1,cube2,cube3);
}
//######################################################################################

//######################################################################################
bool game::can_make_piece(tet_request* check_me){

	array_indices box0 = check_me->cubes[0];
	array_indices box1 = check_me->cubes[1];
	array_indices box2 = check_me->cubes[2];
	array_indices box3 = check_me->cubes[3];

	if(board_state[box0.column][box0.row]){
		return false;
	}
	if(board_state[box1.column][box1.row]){
		return false;
	}
	if(board_state[box2.column][box2.row]){
		return false;
	}
	if(board_state[box3.column][box3.row]){
		return false;
	}
	//if we make it here, its okay to return true
	return true;
}
//######################################################################################

//######################################################################################
void game::new_piece_bools(tet_request* new_tet_loc){
		for(unsigned int c = 0; c < TET_SIZE; c++){
			board_state[new_tet_loc->cubes[c].column][new_tet_loc->cubes[c].row] = true;
		}
}
//######################################################################################

//######################################################################################
void game::move_piece_left(tetromino* move_me){

	if(board_state.size() == 0){
		cout << "Error in move_piece_left, board_state seemingly uninitialized." << endl;
		return;
	}

	bool box_good[TET_SIZE] = {false,false,false,false};

	for(unsigned int c = 0; c < TET_SIZE;c++){

		unsigned int col = move_me->get_cubes()[c].my_indices.column;
		if(col == 0){
			return;
		}
		unsigned int row = move_me->get_cubes()[c].my_indices.row;

		if(box_good[c]){
			continue;
		}

		if(!move_me->cube_status[c]){
			box_good[c] = true;
			continue;
		}

		if(!board_state[col-1][row]){
			box_good[c] = true;
			for(unsigned int d = 0; d < TET_SIZE; d++){

				//move on if this cube is about be compared against itself, or if
				//the other cube has already been toggled to true
				if(d == c || box_good[d]){
					continue;
				}

				//if this cube can move left, and is directly to the left
				//of another cube, than that cube can also move left
				if(move_me->get_cubes()[d].my_indices.row == row &&
				   move_me->get_cubes()[d].my_indices.column > col){
					box_good[d] = true;
				}

			}
		}
	}
	//cout << box_good[0] << box_good[1] << box_good[2] << box_good[3] << endl;
	if(box_good[0] && box_good[1] && box_good[2] && box_good[3]){

		for(unsigned int c = 0; c < TET_SIZE; c++){
			if(move_me->cube_status[c]){
				unsigned int col = move_me->get_cubes()[c].my_indices.column;
				unsigned int row = move_me->get_cubes()[c].my_indices.row;

				board_state[col][row]   = false;
				move_me->get_cubes()[c].my_indices.column -= 1;
				move_me->get_cubes()[c].set_center_pt(center_points[col-1][row]);

			}
		}
		//set all of their current positions true at once
		//that way they don't mess with each other's logic
		for(unsigned int c = 0; c < TET_SIZE; c++){
			unsigned int col = move_me->get_cubes()[c].my_indices.column;
			unsigned int row = move_me->get_cubes()[c].my_indices.row;
			board_state[col][row] = true;
		}
	}

}
//######################################################################################

//######################################################################################
void game::move_piece_right(tetromino* move_me){

	if(board_state.size() == 0){
		cout << "Error in move_piece_left, board_state seemingly uninitialized." << endl;
		return;
	}

	bool box_good[TET_SIZE] = {false,false,false,false};

	for(unsigned int c = 0; c < TET_SIZE;c++){
		unsigned int col = move_me->get_cubes()[c].my_indices.column;
		if(col == board_state.size()-1){
			return;
		}
		unsigned int row = move_me->get_cubes()[c].my_indices.row;

		if(box_good[c]){
			continue;
		}

		if(!move_me->cube_status[c]){
			box_good[c] = true;
			continue;
		}

		if(!board_state[col+1][row]){
			box_good[c] = true;
			for(unsigned int d = 0; d < TET_SIZE; d++){

				//move on if this cube is about be compared against itself, or if
				//the other cube has already been toggled to true
				if(d == c || box_good[d]){
					continue;
				}

				//if this cube can move left, and is directly to the left
				//of another cube, than that cube can also move left
				if(move_me->get_cubes()[d].my_indices.row == row &&
				   move_me->get_cubes()[d].my_indices.column < col){
					box_good[d] = true;
				}

			}
		}
	}
	//cout << box_good[0] << box_good[1] << box_good[2] << box_good[3] << endl;
	if(box_good[0] && box_good[1] && box_good[2] && box_good[3]){

		for(unsigned int c = 0; c < TET_SIZE; c++){
			if(move_me->cube_status[c]){
				unsigned int col = move_me->get_cubes()[c].my_indices.column;
				unsigned int row = move_me->get_cubes()[c].my_indices.row;

				board_state[col][row]   = false;
				board_state[col+1][row] = true;
				move_me->get_cubes()[c].my_indices.column += 1;
				move_me->get_cubes()[c].set_center_pt(center_points[col+1][row]);
			}
		}

	}

}
//######################################################################################

//######################################################################################
void game::drop_piece(tetromino* move_me){

}
//######################################################################################

//######################################################################################
void game::fast_drop_piece(tetromino* move_me){

}
//######################################################################################

















