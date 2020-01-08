#include "tetromino.h"


#include "shaders.h"
#include "game.h"
extern shaders* shader_access;
extern game* game_access;

using namespace std;

//######################################################################################
tetromino::tetromino(){
	cube_status[0] = true;
	cube_status[1] = true;
	cube_status[2] = true;
	cube_status[3] = true;

	position = 0;
}
//######################################################################################

//######################################################################################
tetromino::tetromino(const tetromino& other){
	cube_status[0] = other.cube_status[0];
	cube_status[1] = other.cube_status[1];
	cube_status[2] = other.cube_status[2];
	cube_status[3] = other.cube_status[3];

	position = other.position;
}
//######################################################################################

//######################################################################################
void tetromino::init(const string& color, const tet_request* cube_locs){
	unsigned int column1, column2, column3, column4;

	column1 = cube_locs->cubes[0].column;
	column2 = cube_locs->cubes[1].column;
	column3 = cube_locs->cubes[2].column;
	column4 = cube_locs->cubes[3].column;

	unsigned int row1, row2, row3, row4;

	row1 = cube_locs->cubes[0].row;
	row2 = cube_locs->cubes[1].row;
	row3 = cube_locs->cubes[2].row;
	row4 = cube_locs->cubes[3].row;

	my_cubes[0].init(color,game_access->get_game_locs()[column1][row1],cube_locs->cubes[0]);
	my_cubes[1].init(color,game_access->get_game_locs()[column2][row2],cube_locs->cubes[1]);
	my_cubes[2].init(color,game_access->get_game_locs()[column3][row3],cube_locs->cubes[2]);
	my_cubes[3].init(color,game_access->get_game_locs()[column4][row4],cube_locs->cubes[3]);
}
//######################################################################################

//######################################################################################
void tetromino::draw_me(){
	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(cube_status[c]){
			my_cubes[c].draw_me();
			cout << "Center point:" << my_cubes[c].get_center_point() << endl;
		}
	}
}
//######################################################################################

//######################################################################################
void tetromino::kill_cubes(unsigned int row){
	if(game_access->get_const_board_state().size() == 0){
		cout << "Error! from tetromino::kill_cubes,"
			 << " board seemingly uninitialized." << endl;
		return;
	}
	if(row > game_access->get_const_board_state()[0].size()-1){
		cout << "Error! tetromino::kill_cubes given bad row index." << endl;
		return;
	}

	for(unsigned int c = 0; c < TET_SIZE;c++){
		if(cube_status[c] && my_cubes[c].my_indices.row == row){
			cube_status[c] = false;
		}
	}

}
//######################################################################################

//######################################################################################
bool tetromino::dead(){
	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(cube_status[c]){
			return false;
		}
	}
	return true;
}
//######################################################################################

//######################################################################################
void tetromino::up_pos(){
	position += 1;
	position = position % 4;
}
//######################################################################################

//######################################################################################
void tetromino::down_pos(){
	if(position == 0){
		position = 3;
	} else {
		position -= 1;
	}
}
//######################################################################################

//######################################################################################
//                   0
//                 2 1 3
bool t_block::rot_left(){
	deactivate_cubes();

	if(check_left()){
		switch(position){
			case 0: //0 to 3
				my_cubes[2].my_indices.column += 1;
				my_cubes[2].my_indices.row    += 1;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 1: //1 to 0
				my_cubes[2].my_indices.column -= 1;
				my_cubes[2].my_indices.row    += 1;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;

				break;
			case 2: // 2 to 1
				my_cubes[2].my_indices.column -= 1;
				my_cubes[2].my_indices.row    -= 1;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 3: // 3 to 2
				my_cubes[2].my_indices.column += 1;
				my_cubes[2].my_indices.row    -= 1;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		reactivate_cubes();
		down_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
bool t_block::rot_right(){
	deactivate_cubes();

	if(check_right()){
		//activate all of the booleans for the new location, and tell
		//the cubes their location
		switch(position){
			case 0: //0 to 1
				my_cubes[2].my_indices.column += 1;
				my_cubes[2].my_indices.row    -= 1;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 1: //1 to 2
				my_cubes[2].my_indices.column += 1;
				my_cubes[2].my_indices.row    += 1;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 2: //2 to 3
				my_cubes[2].my_indices.column -= 1;
				my_cubes[2].my_indices.row    += 1;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 3: //3 to 0
				my_cubes[2].my_indices.column -= 1;
				my_cubes[2].my_indices.row    -= 1;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;

			default:
				cout << "Illegal position in rot_right: " << position << endl;
				exit(-1);
		}
		reactivate_cubes();
		up_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
void t_block::deactivate_cubes(){
	unsigned int col;
	unsigned int row;


	//deactivate all the booleans for these cubes
	//except for the 'anchor' cube (doesn't move when rotated)
	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			col = my_cubes[c].my_indices.column;
			row = my_cubes[c].my_indices.row;
			game_access->get_board_state()[col][row] = false;
		}
	}
}
//######################################################################################

//######################################################################################
void t_block::reactivate_cubes(){
	unsigned int col;
	unsigned int row;


	//activate all booleans for this piece
	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			col = my_cubes[c].my_indices.column;
			row = my_cubes[c].my_indices.row;
			game_access->get_board_state()[col][row] = true;
			my_cubes[c].set_center_pt(game_access->get_game_locs()[col][row]);
		}
	}
}
//######################################################################################

//######################################################################################
bool t_block::check_left(){
		int cols[TET_SIZE];
		int rows[TET_SIZE];

		for(unsigned int c = 0; c < TET_SIZE; c++){
			if(c != 1){
				cols[c] = my_cubes[c].my_indices.column;
				rows[c] = my_cubes[c].my_indices.row;
			}
		}

		const vector<vector<bool>>& board = game_access->get_const_board_state();

		int num_cols = board.size() - 1;
		int num_rows = board[0].size() - 1;

		switch(position){
			case 0: //0 to 3 -> only need to check bottom
				if(rows[3] == num_rows) return false;
				break;
			case 1: //1 to 0 -> only need to check left
				if(cols[2] == 0) return false;
				break;
			case 2: //2 to 1 -> only need to check top
				if(rows[3] == 0) return false;
				break;
			case 3: //3 to 2 -> only need to check right
				if(cols[2] == num_cols) return false;
				break;
			default:
				break;
		}

		switch(position){
			case 0: //0 to 3
				if(board[cols[2]+1][rows[2]+1]) return false;
				if(board[cols[0]-1][rows[0]+1]) return false;
				if(board[cols[3]-1][rows[3]-1]) return false;
				break;
			case 1: //1 to 0
				if(board[cols[2]-1][rows[2]+1]) return false;
				if(board[cols[0]-1][rows[0]-1]) return false;
				if(board[cols[3]+1][cols[3]-1]) return false;
				break;
			case 2: // 2 to 1
				if(board[cols[2]-1][rows[2]-1]) return false;
				if(board[cols[0]+1][rows[0]-1]) return false;
				if(board[cols[3]+1][rows[3]+1]) return false;
				break;
			case 3: // 3 to 2
				if(board[cols[2]+1][rows[2]-1]) return false;
				if(board[cols[0]+1][rows[0]+1]) return false;
				if(board[cols[3]-1][rows[3]+1]) return false;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		return true;
}
//######################################################################################

//######################################################################################
bool t_block::check_right(){
	int cols[TET_SIZE];
	int rows[TET_SIZE];

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}


	const vector<vector<bool>>& board = game_access->get_const_board_state();

	int num_rows = board[0].size() - 1;
	int num_cols = board.size()    - 1;

	switch(position){
		case 0: //0 to 1 -> only need to check bottom
			if(rows[2] == num_rows) return false;
			break;
		case 1: //1 to 2 -> only need to check left
			if(cols[2] == 0) return false;
			break;
		case 2: //2 to 3 -> only need to check top
			if(rows[3] == 0) return false;
			break;
		case 3: //3 to 0 -> only need to check right
			if(cols[3] == num_cols) return false;
			break;
		default:
			break;
	}

	switch(position){
		case 0: //0 to 1
			if(board[cols[2]+1][rows[2]-1]) return false;
			if(board[cols[0]+1][rows[0]+1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 1: //1 to 2
			if(board[cols[2]+1][rows[2]+1]) return false;
			if(board[cols[0]-1][rows[0]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 2: //2 to 3
			if(board[cols[2]-1][rows[2]+1]) return false;
			if(board[cols[0]-1][rows[0]-1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 3: //3 to 0
			if(board[cols[2]-1][rows[2]-1]) return false;
			if(board[cols[0]+1][rows[0]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;

		default:
			cout << "Illegal position in rot_right: " << position << endl;
			exit(-1);
		}
	return true;
}
//######################################################################################


//######################################################################################
//            2 0
//              1 3
bool l_z_block::rot_left(){
	deactivate_cubes();
	if(check_left()){
		switch(position){
			case 0: //0 to 3
				my_cubes[2].my_indices.row    += 2;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 1: //1 to 0
				my_cubes[2].my_indices.column -= 2;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 2: //2 to 1
				my_cubes[2].my_indices.row    -= 2;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 3: //3 to 2
				my_cubes[2].my_indices.column += 2;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		reactivate_cubes();
		down_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
bool l_z_block::rot_right(){
	deactivate_cubes();
	if(check_right()){
		switch(position){
			case 0: //0 to 1
				my_cubes[2].my_indices.column += 2;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 1: //1 to 2
				my_cubes[2].my_indices.row    += 2;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 2: //2 to 3
				my_cubes[2].my_indices.column -= 2;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 3: //3 to 0
				my_cubes[2].my_indices.row    -= 2;
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			default:
				cout << "Illegal position in rot_right." << endl;
				exit(-1);
		}
		reactivate_cubes();
		up_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
void l_z_block::deactivate_cubes(){
	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			col = my_cubes[c].my_indices.column;
			row = my_cubes[c].my_indices.row;
			game_access->get_board_state()[col][row] = false;
		}
	}
}
//######################################################################################

//######################################################################################
void l_z_block::reactivate_cubes(){
	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			col = my_cubes[c].my_indices.column;
			row = my_cubes[c].my_indices.row;
			game_access->get_board_state()[col][row] = true;
			my_cubes[c].set_center_pt(game_access->get_game_locs()[col][row]);
		}
	}
}
//######################################################################################

//######################################################################################
bool l_z_block::check_left(){
	unsigned int cols[TET_SIZE];
	unsigned int rows[TET_SIZE];

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}


	const vector<vector<bool>>& board = game_access->get_const_board_state();

	switch(position){
		case 0: //0 to 3 -> only need to check bottom
			if(rows[3] + 1 > board[0].size() - 1) return false;
			break;
		case 1: //1 to 0 -> only need to check left
			if(cols[3] == 0) return false;
			break;
		case 2: //2 to 1 -> only need to check top
			if(rows[3] - 1 < 0) return false;
			break;
		case 3: //3 to 2 -> only need to check right
			if(cols[3] + 1 > board.size() - 1) return false;
			break;
		default:
			cout << "Illegal position in check_left()." << endl;
			exit(-1);
	}

	switch(position){
		case 0: //0 to 3
			if(board[cols[2]][rows[2]+2])   return false;
			if(board[cols[0]-1][rows[0]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 1: //1 to 0
			if(board[cols[2]-2][rows[2]])   return false;
			if(board[cols[0]-1][rows[0]-1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 2: //2 to 1
			if(board[cols[2]][rows[2]-2])   return false;
			if(board[cols[0]+1][rows[0]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		case 3: //3 to 2
			if(board[cols[2]+2][rows[2]])   return false;
			if(board[cols[0]+1][rows[0]+1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}
	return true;
}
//######################################################################################

//######################################################################################
bool l_z_block::check_right(){
	unsigned int cols[TET_SIZE];
	unsigned int rows[TET_SIZE];

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}

	const vector<vector<bool>>& board = game_access->get_const_board_state();

	switch(position){
		case 0: //0 to 1 -> only need to check bottom
			if(rows[3] == board[0].size() -1) return false;
			break;
		case 1: //1 to 2 -> only need to check left
			if(cols[3] == 0) return false;
			break;
		case 2: //2 to 3 -> only need to check top
			if(rows[3] == 0) return false;
			break;
		case 3: //3 to 0 -> only need to check right
			if(cols[3] == board.size() - 1) return false;
			break;

		default:
			cout << "Illegal positionin rot_right." << endl;

	}

	switch(position){
		case 0: //0 to 1
			if(board[cols[2]+2][rows[2]])   return false;
			if(board[cols[0]+1][rows[0]+1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 1: //1 to 2
			if(board[cols[2]][rows[2]+2])   return false;
			if(board[cols[0]-1][rows[0]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 2: //2 to 3
			if(board[cols[2]-2][rows[2]])   return false;
			if(board[cols[0]-1][rows[0]-1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 3: //3 to 0
			if(board[cols[2]][rows[2]-2])   return false;
			if(board[cols[0]+1][rows[0]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		default:
			cout << "Illegal position in rot_right." << endl;
			exit(-1);
	}


	return true;
}
//######################################################################################

//######################################################################################
//          0 2
//        3 1
bool r_z_block::rot_left(){
	deactivate_cubes();
	if(check_left()){
		switch(position){
			case 0: //0 to 3
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[2].my_indices.column -= 2;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 1: //1 to 0
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[2].my_indices.row    -= 2;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 2: //2 to 1
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[2].my_indices.column += 2;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 3: //3 to 2
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[2].my_indices.row    += 2;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		reactivate_cubes();
		down_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
bool r_z_block::rot_right(){
	deactivate_cubes();
	if(check_right()){
		switch(position){
			case 0: //0 to 1
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[2].my_indices.row    += 2;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 1: //1 to 2
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[2].my_indices.column -= 2;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 2: //2 to 3
				my_cubes[0].my_indices.column -= 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[2].my_indices.row    -= 2;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 3: //3 to 0
				my_cubes[0].my_indices.column += 1;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[2].my_indices.column += 2;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			default:
				cout << "Illegal position in rot_right." << endl;
				exit(-1);
		}
		reactivate_cubes();
		up_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
void r_z_block::deactivate_cubes(){
	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			col = my_cubes[c].my_indices.column;
			row = my_cubes[c].my_indices.row;
			game_access->get_board_state()[col][row] = false;
		}
	}
}
//######################################################################################

//######################################################################################
void r_z_block::reactivate_cubes(){
	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			col = my_cubes[c].my_indices.column;
			row = my_cubes[c].my_indices.row;
			game_access->get_board_state()[col][row] = true;
			my_cubes[c].set_center_pt(game_access->get_game_locs()[col][row]);
		}
	}
}
//######################################################################################

//######################################################################################
bool r_z_block::check_left(){

	unsigned int cols[TET_SIZE];
	unsigned int rows[TET_SIZE];

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}

	const vector<vector<bool>>& board = game_access->get_board_state();

	switch(position){
		case 0: //0 to 3 -> only need to check bottom
			if(rows[3] == board[0].size() - 1) return false;
			break;
		case 1: //1 to 0 -> only need to check left
			if(cols[3] == 0) return false;
			break;
		case 2: //2 to 1 -> only need to check top
			if(rows[3] == 0) return false;
			break;
		case 3: //3 to 2 -> only need to check right
			if(cols[3] == board.size() - 1) return false;
			break;
		default:
			cout << "Illegal position in check_left." << endl;
			exit(-1);
	}

	switch(position){
		case 0: //0 to 3
			if(board[cols[0]-1][rows[0]+1]) return false;
			if(board[cols[2]-2][rows[2]])   return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		case 1: //1 to 0
			if(board[cols[0]-1][rows[0]-1]) return false;
			if(board[cols[2]][rows[2]-2])   return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 2: //2 to 1
			if(board[cols[0]+1][rows[0]-1]) return false;
			if(board[cols[2]+2][rows[2]])   return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 3: //3 to 2
			if(board[cols[0]+1][rows[0]+1]) return false;
			if(board[cols[2]+2][rows[2]])   return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}



	return true;
}
//######################################################################################

//######################################################################################
bool r_z_block::check_right(){
	unsigned int cols[TET_SIZE];
	unsigned int rows[TET_SIZE];

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 1){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}

	const vector<vector<bool>>& board = game_access->get_board_state();

	switch(position){
		case 0: //0 to 1 -> only need to check bottom
			if(rows[3] == board[0].size() - 1) return false;
			break;
		case 1: //1 to 2 -> only need to check left
			if(cols[3] == 0) return false;
			break;
		case 2: //2 to 3 -> only need to check top
			if(rows[3] == 0) return false;
			break;
		case 3: //3 to 0 -> only need to check right
			if(cols[3] == board.size() - 1) return false;
			break;
		default:
			cout << "Illegal position in check_right." << endl;
			exit(-1);
	}

	switch(position){
		case 0: //0 to 1
			if(board[cols[0]+1][rows[0]+1]) return false;
			if(board[cols[2]][rows[2]+2])   return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 1: //1 to 2
			if(board[cols[0]-1][rows[0]+1]) return false;
			if(board[cols[2]-2][rows[2]])   return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		case 2: //2 to 3
			if(board[cols[0]-1][rows[0]-1]) return false;
			if(board[cols[2]][rows[2]-2])   return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 3: //3 to 0
			if(board[cols[0]+1][rows[0]-1]) return false;
			if(board[cols[2]+2][rows[2]])   return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
	}


	return true;
}
//######################################################################################

//######################################################################################
//       1 0 2 3
bool I_block::rot_left(){

	deactivate_cubes();
	if(check_left()){
		switch(position){
			case 0: //0 to 3
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    += 2;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[2].my_indices.column -= 1;
				my_cubes[3].my_indices.column -= 2;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 1: //1 to 0
				my_cubes[1].my_indices.column -= 2;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[2].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 2;
				break;
			case 2: //2 to 1
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    -= 2;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[2].my_indices.column += 1;
				my_cubes[3].my_indices.column += 2;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 3: //3 to 2
				my_cubes[1].my_indices.column += 2;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[0].my_indices.column += 1;
				my_cubes[2].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 2;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		reactivate_cubes();
		down_pos();
		return true;

	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
bool I_block::rot_right(){
	deactivate_cubes();
	if(check_right()){
		switch(position){
			case 0: //0 to 1
				my_cubes[1].my_indices.column += 2;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[0].my_indices.column += 1;
				my_cubes[2].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 2;
				break;
			case 1: //1 to 2
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    += 2;
				my_cubes[0].my_indices.row    += 1;
				my_cubes[2].my_indices.column -= 1;
				my_cubes[3].my_indices.column -= 2;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 2: //2 to 3
				my_cubes[1].my_indices.column -= 2;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[0].my_indices.column -= 1;
				my_cubes[2].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 2;
				break;
			case 3: //3 to 0
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    -= 2;
				my_cubes[0].my_indices.row    -= 1;
				my_cubes[2].my_indices.column += 1;
				my_cubes[3].my_indices.column += 2;
				my_cubes[3].my_indices.row    += 1;
				break;
			default:
				cout << "Illegal position in rot_right." << endl;
				exit(-1);
		}
		reactivate_cubes();
		up_pos();
		return true;
	}
	reactivate_cubes();
	return true;
}
//######################################################################################

//######################################################################################
void I_block::deactivate_cubes(){

	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		col = my_cubes[c].my_indices.column;
		row = my_cubes[c].my_indices.row;
		game_access->get_board_state()[col][row] = false;
	}
}
//######################################################################################

//######################################################################################
void I_block::reactivate_cubes(){

	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		col = my_cubes[c].my_indices.column;
		row = my_cubes[c].my_indices.row;
		game_access->get_board_state()[col][row] = true;
		my_cubes[c].set_center_pt(game_access->get_game_locs()[col][row]);
	}
}
//######################################################################################

//######################################################################################
bool I_block::check_left(){

	unsigned int cols[TET_SIZE];
	unsigned int rows[TET_SIZE];


	for(unsigned int c = 0; c < TET_SIZE; c++){
		cols[c] = my_cubes[c].my_indices.column;
		rows[c] = my_cubes[c].my_indices.row;
	}

	const vector<vector<bool>>& board = game_access->get_const_board_state();

	//check sides of board
	switch(position){
		case 0: //0 to 3 -> don't need to check sides of board
			if(rows[0] < 1 || rows[0] > board[0].size() - 3){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit top of board or bottom of board
			}
			break;
		case 1: //1 to 0 -> don't need to check height of board
			if(cols[0] < 2 || cols[0] > board.size() - 2){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit right or left sides of board
			}
			break;
		case 2: //2 to 1 -> don't need to check sides of board
			if(rows[0] < 2 || rows[0] > board[0].size() - 2){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit top or bottom of board
			}
			break;
		case 3: //3 to 2 -> don't need to check height of board
			if(rows[0] < 1 || rows[0] > board[0].size() - 3){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit right or left sides of board
			}
			break;

		default:
			cout << "Illegal position in check_left" << endl;

	}


	switch(position){
		case 0: //0 to 3
			if(board[cols[1]+1][rows[1]+2]) return false;
			if(board[cols[0]][rows[0]+1])   return false;
			if(board[cols[2]-1][rows[2]])   return false;
			if(board[cols[3]-2][rows[3]-1]) return false;
			break;
		case 1: //1 to 0
			if(board[cols[1]-2][rows[1]+1]) return false;
			if(board[cols[0]-1][rows[0]])   return false;
			if(board[cols[2]][rows[2]-1])   return false;
			if(board[cols[3]+1][rows[3]-2]) return false;
			break;
		case 2: //2 to 1
			if(board[cols[1]-1][rows[1]-2]) return false;
			if(board[cols[0]][rows[0]-1])   return false;
			if(board[cols[2]+1][rows[2]])   return false;
			if(board[cols[3]+2][rows[3]+1]) return false;
			break;
		case 3: //3 to 2
			if(board[cols[1]+2][rows[1]-1]) return false;
			if(board[cols[0]+1][rows[0]])   return false;
			if(board[cols[2]][rows[2]+1])   return false;
			if(board[cols[3]-1][rows[3]+2]) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}
	return true;
}
//######################################################################################

//######################################################################################
bool I_block::check_right(){
	int cols[TET_SIZE];
	int rows[TET_SIZE];

	for(unsigned int c = 0; c < TET_SIZE; c++){
		cols[c] = my_cubes[c].my_indices.column;
		rows[c] = my_cubes[c].my_indices.row;

	}

	const vector<vector<bool>>& board = game_access->get_const_board_state();

	//check sides of board
	switch(position){
		case 0: //0 to 1 -> don't need to check sides of board
			if(rows[0] < 2 || rows[0] > (int) board[0].size() - 3){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit top of board or bottom of board
			}
			break;
		case 1: //1 to 2 -> don't need to check height of board
			if(cols[0] < 3 || cols[0] > (int) board.size() - 2){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit right or left sides of board
			}
			break;
		case 2:  //2 to 3 -> don't need to check sides of board
			if(rows[0] < 2 || rows[0] > (int) board[0].size() - 2){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit top or bottom of board
			}
			break;
		case 3:  //1 to 2 -> don't need to check height of board
			if(rows[0] < 1 || rows[0] > (int)board[0].size() - 2){
				//cout << "\n\n\n\n\n" << endl;
				return false; //will hit right or left sides of board
			}
			break;

		default:
			cout << "Illegal position in check_left" << endl;

	}



	switch(position){
		case 0: //0 to 1
			if(board[cols[1]+2][rows[1]-1]) return false;
			if(board[cols[0]+1][rows[0]])   return false;
			if(board[cols[2]][rows[2]+1])   return false;
			if(board[cols[3]-1][rows[3]+2]) return false;
			break;
		case 1: //1 to 2
			if(board[cols[1]+1][rows[1]+2]) return false;
			if(board[cols[0]][rows[0]+1])   return false;
			if(board[cols[2]-1][rows[2]])   return false;
			if(board[cols[3]-2][rows[3]-1]) return false;
			break;
		case 2: //2 to 3
			if(board[cols[1]-2][rows[1]+1]) return false;
			if(board[cols[0]-1][rows[0]])   return false;
			if(board[cols[2]+1][rows[2]-1]) return false;
			if(board[cols[3]+1][rows[3]-2]) return false;
			break;
		case 3: //3 to 0
			if(board[cols[1]-1][rows[1]-2]) return false;
			if(board[cols[0]-1][rows[0]])    return false;
			if(board[cols[2]+1][rows[2]])    return false;
			if(board[cols[3]+2][rows[3]+1]) return false;
			break;
		default:
			cout << "Illegal position in rot_right." << endl;
			exit(-1);
	}
	return true;
}
//######################################################################################

//######################################################################################
//      0
//      1 2 3
bool l_l_block::rot_left(){

	deactivate_cubes();
	if(check_left()){
		switch(position){
			case 0: //0 to 3
				my_cubes[0].my_indices.row    += 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 1: //1 to 0
				my_cubes[0].my_indices.column -= 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 2: //2 to 1
				my_cubes[0].my_indices.row    -= 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 3: //3 to 2
				my_cubes[0].my_indices.column += 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		reactivate_cubes();
		down_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
bool l_l_block::rot_right(){


	deactivate_cubes();
	if(check_right()){
		switch(position){
			case 0: //0 to 1
				my_cubes[0].my_indices.column += 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 1: //1 to 2
				my_cubes[0].my_indices.row    += 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 2: //2 to 3
				my_cubes[0].my_indices.column -= 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 3: //3 to 0
				my_cubes[0].my_indices.row    -= 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			default:
				cout << "Illegal position in rot_right." << endl;
				exit(-1);
		}
		reactivate_cubes();
		up_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
void l_l_block::deactivate_cubes(){

	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		col = my_cubes[c].my_indices.column;
		row = my_cubes[c].my_indices.row;
		game_access->get_board_state()[col][row] = false;
	}
}
//######################################################################################

//######################################################################################
void l_l_block::reactivate_cubes(){

	unsigned int col;
	unsigned int row;

	for(unsigned int c = 0; c < TET_SIZE; c++){
		col = my_cubes[c].my_indices.column;
		row = my_cubes[c].my_indices.row;
		game_access->get_board_state()[col][row] = true;
		my_cubes[c].set_center_pt(game_access->get_game_locs()[col][row]);
	}
}
//######################################################################################

//######################################################################################
bool l_l_block::check_left(){

	int cols[TET_SIZE];
	int rows[TET_SIZE];

	const vector<vector<bool>>& board = game_access->get_const_board_state();

	for(unsigned int c = 0; c < TET_SIZE;c++){
		if(c != 2){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}
	//check board bounds
	switch(position){
		case 0: //0 to 3 -> only need to check bottom
			if(rows[1]+1 > (int)board[0].size() - 2) return false;
			break;
		case 1: //1 to 0 -> only need to check left side
			if(cols[1]-1 < 0) return false;
			break;
		case 2: //2 to 1 -> only need to check top
			if(rows[1]-1 < 0) return false;
			break;
		case 3: //3 to 2 -> only need to check right side
			if(cols[1]+1 > (int)board.size()-1) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}


	switch(position){
		case 0: //0 to 3
			if(board[cols[0]][rows[0]+2])   return false;
			if(board[cols[1]+1][rows[1]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 1: //1 to 0
			if(board[cols[0]-2][rows[0]])   return false;
			if(board[cols[1]-1][rows[1]+1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 2: //2 to 1
			if(board[cols[0]][rows[0]-2])   return false;
			if(board[cols[1]-1][rows[1]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		case 3: //3 to 2
			if(board[cols[0]+2][rows[0]+1]) return false;
			if(board[cols[1]+1][rows[1]-1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}

	return true;
}
//######################################################################################

//######################################################################################
bool l_l_block::check_right(){

	int cols[TET_SIZE];
	int rows[TET_SIZE];

	const vector<vector<bool>>& board = game_access->get_const_board_state();

	for(unsigned int c = 0; c < TET_SIZE;c++){
		if(c != 2){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}

	switch(position){
		case 0:  //0 to 1 -> only need to check bottom
			if(rows[1]+1 > (int)board[0].size() - 1) return false;
			break;
		case 1:  //1 to 2 -> only need to check left
			if(cols[2]-1 < 0) return false;
			break;
		case 2:  //2 to 3 -> only need to check top
			if(rows[3]-1 < 0) return false;
			break;
		case 3:  //3 to 0 -> only need to check right
			if(cols[3]+1  > (int)board.size() - 1) return false;
			break;

		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}


	switch(position){
		case 0: //0 to 1
			if(board[cols[0]+2][rows[0]])   return false;
			if(board[cols[1]+1][rows[1]-1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 1: //1 to 2
			if(board[cols[0]][rows[2]+2])   return false;
			if(board[cols[1]+1][rows[1]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 2: //2 to 3
			if(board[cols[0]-2][rows[0]])   return false;
			if(board[cols[1]-1][rows[1]+1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 3: //3 to 0
			if(board[cols[0]][rows[0]-2])   return false;
			if(board[cols[1]-1][rows[1]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		default:
			cout << "Illegal position in rot_right." << endl;
			exit(-1);
	}

	return true;
}
//######################################################################################

//######################################################################################
//            0
//        3 2 1
bool r_l_block::rot_left(){
	deactivate_cubes();
	if(check_left()){
		switch(position){
			case 0: //0 to 3
				my_cubes[0].my_indices.column -= 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 1: //1 to 0
				my_cubes[0].my_indices.row    -= 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 2: //2 to 1
				my_cubes[0].my_indices.column += 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 3: //3 to 2
				my_cubes[0].my_indices.row    += 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			default:
				cout << "Illegal position in rot_left." << endl;
				exit(-1);
		}
		reactivate_cubes();
		down_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
bool r_l_block::rot_right(){
	deactivate_cubes();
	if(check_right()){
		switch(position){
			case 0: //0 to 1
				my_cubes[0].my_indices.row    += 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			case 1: //1 to 2
				my_cubes[0].my_indices.column -= 2;
				my_cubes[1].my_indices.column -= 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column += 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 2: //2 to 3
				my_cubes[0].my_indices.row    -= 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    -= 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    += 1;
				break;
			case 3: //3 to 0
				my_cubes[0].my_indices.column += 2;
				my_cubes[1].my_indices.column += 1;
				my_cubes[1].my_indices.row    += 1;
				my_cubes[3].my_indices.column -= 1;
				my_cubes[3].my_indices.row    -= 1;
				break;
			default:
				cout << "Illegal position in rot_right." << endl;
				exit(-1);
		}
		reactivate_cubes();
		up_pos();
		return true;
	}
	reactivate_cubes();
	return false;
}
//######################################################################################

//######################################################################################
void r_l_block::deactivate_cubes(){

	int col;
	int row;


	for(unsigned int c = 0; c < TET_SIZE;c++){
		col = my_cubes[c].my_indices.column;
		row = my_cubes[c].my_indices.row;
		game_access->get_board_state()[col][row] = false;
	}
}
//######################################################################################

//######################################################################################
void r_l_block::reactivate_cubes(){

	int col;
	int row;


	for(unsigned int c = 0; c < TET_SIZE;c++){
		col = my_cubes[c].my_indices.column;
		row = my_cubes[c].my_indices.row;
		game_access->get_board_state()[col][row] = true;
		my_cubes[c].set_center_pt(game_access->get_game_locs()[col][row]);
	}
}
//######################################################################################

//######################################################################################
bool r_l_block::check_left(){

	int cols[TET_SIZE];
	int rows[TET_SIZE];


	const vector<vector<bool>>& board = game_access->get_const_board_state();

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 2){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}

	switch(position){
		case 0: //0 to 3 -> need only check bottom
			if(rows[1]+1 > (int)board[0].size() - 1) return false;
			break;
		case 1: //1 to 0 -> need only check left edge
			if(cols[1]-1 < 0) return false;
			break;
		case 2: //2 to 1 -> need only check top
			if(rows[1]-1 < 0) return false;
			break;
		case 3: //3 to 2 -> need only check right
			if(cols[1]+1 > (int)board.size() - 1) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}


	switch(position){
		case 0: //0 to 3
			if(board[cols[0]-2][rows[0]])   return false;
			if(board[cols[1]-1][rows[1]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		case 1: //1 to 0
			if(board[cols[0]][rows[0]-2])   return false;
			if(board[cols[1]+1][rows[1]-1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 2: //2 to 1
			if(board[cols[0]+2][rows[0]])   return false;
			if(board[cols[1]+1][rows[1]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		case 3: //3 to 2
			if(board[cols[0]][rows[0]+2])   return false;
			if(board[cols[1]-1][rows[1]+1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}

	return true;
}
//######################################################################################

//######################################################################################
bool r_l_block::check_right(){

	int cols[TET_SIZE];
	int rows[TET_SIZE];

	const vector<vector<bool>>& board = game_access->get_const_board_state();

	for(unsigned int c = 0; c < TET_SIZE; c++){
		if(c != 2){
			cols[c] = my_cubes[c].my_indices.column;
			rows[c] = my_cubes[c].my_indices.row;
		}
	}

	switch(position){
		case 0: //0 to 1 -> need only check bottom
			if(rows[1]+1 > (int)board[0].size() - 1) return false;
			break;
		case 1: //1 to 2 -> need only check left edge
			if(cols[1]-1 < 0) return false;
			break;
		case 2: //2 to 3 -> need only check top
			if(rows[1]-1 < 0) return false;
			break;
		case 3: //3 to 0 -> need only check right
			if(cols[1]+1 > (int)board.size() - 1) return false;
			break;
		default:
			cout << "Illegal position in rot_left." << endl;
			exit(-1);
	}

	switch(position){
		case 0: //0 to 1
			if(board[cols[0]][rows[0]+2])   return false;
			if(board[cols[1]-1][rows[1]+1]) return false;
			if(board[cols[3]+1][rows[3]-1]) return false;
			break;
		case 1: //1 to 2
			if(board[cols[0]-2][rows[0]])   return false;
			if(board[cols[1]-1][rows[1]-1]) return false;
			if(board[cols[3]+1][rows[3]+1]) return false;
			break;
		case 2: //2 to 3
			if(board[cols[0]][rows[0]-2])   return false;
			if(board[cols[1]+1][rows[1]-1]) return false;
			if(board[cols[3]-1][rows[3]+1]) return false;
			break;
		case 3: //3 to 0
			if(board[cols[0]+2][rows[0]])   return false;
			if(board[cols[1]+1][rows[1]+1]) return false;
			if(board[cols[3]-1][rows[3]-1]) return false;
			break;
		default:
			cout << "Illegal position in rot_right." << endl;
			exit(-1);
	}
	return true;
}
//######################################################################################

//######################################################################################
bool square_block::rot_left(){
	down_pos();
	return true;
}
//######################################################################################

//######################################################################################
bool square_block::rot_right(){
	up_pos();
	return true;
}
//######################################################################################

//######################################################################################
void square_block::deactivate_cubes(){

}
//######################################################################################

//######################################################################################
void square_block::reactivate_cubes(){

}
//######################################################################################

//######################################################################################
bool square_block::check_left(){
	return true;
}
//######################################################################################

//######################################################################################
bool square_block::check_right(){
	return true;
}
//######################################################################################








