#pragma once

#include "cube.h"
#include "c_tuples.h"

//! bag for 4 cubes, that is inherited from to implement rotations and bounds checking
class tetromino{
  public:
	//! constructor just zeros out most of the fields for this class
	tetromino();
	//! copy constructor copies boolean arrays and most fields for this class
	tetromino(const tetromino& other);
	//~tetromino();

	//! this function sets up the tetrimono according to the arguments
	void init(const std::string& color,const tet_request* cube_locs);

	//! this function draws all of this tetrimino's remaining cubes
	void draw_me();

	//! this function can be overloaded and rotates this tetromino clockwise
	/*! it should be overloaded, keeps track of the current rotation state*/
	virtual bool rot_left() = 0;
	//! this function can to be overloaded and rotates this tetrominio counterclockwise
	/*! it should be overloaded, keeps track of the current rotation state*/
	virtual bool rot_right() = 0;
	//! helper to see if left rotation can be done
	virtual bool check_left() = 0;
	//helper to see if right rotation can be done
	virtual bool check_right() = 0;
	//! this function turns the current location's values in the game state to false
	virtual void deactivate_cubes() = 0;
	//! this function turns the current location's values in the game state to true
	virtual void reactivate_cubes() = 0;

	//! this function increments the position variable
	void up_pos();
	//! this function decrements the positition variable
	void down_pos();

	//! this function deactives all cubes that match the passed row parameter
	void kill_cubes(unsigned int row);
	//! return true if all of this piece's cubes have been killed off
	bool dead();
	//! const getter for array of cube objects
	const cube* get_const_cubes(){ return my_cubes;}

	//! getter for array of cube objects
	cube* get_cubes(){ return my_cubes;}

	//! keep track of which cubes have been deleted or not
	bool cube_status[TET_SIZE];

	//! starts at 0, and indicates the current rotation mode
	/*! these positions indicate which position the cube is in
	 *per
	 *http://tetris.wikia.com/wiki/SRS */
	unsigned int position;

  protected:
	//! store array of booleans to keep track of which cubes have been removed
	cube my_cubes[TET_SIZE];
};

//! inherit from tetromino, implement rotations for this piece
class t_block : public tetromino{

  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};

//! inherit from tetromino, implement rotations for this piece
class l_z_block : public tetromino{

  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};

//! inherit from tetromino, implement rotations for this piece
class r_z_block : public tetromino{

  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};

//! inherit from tetromino, implement rotations for this piece
class I_block : public tetromino{
  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};

//! inherit from tetromino, implement rotations for this piece
class l_l_block : public tetromino{

  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};

//! inherit from tetromino, implement rotations for this piece
class r_l_block : public tetromino{

  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};

//! inherit from tetromino, implement rotations for this piece
class square_block : public tetromino{

  public:
	//! overload rot_left() for this piece
	bool rot_left();
	//! overload rot_right() for this piece
	bool rot_right();
	//! overload function to turn old location's boolean value to false
	void deactivate_cubes();
	//! overload function to turn new location's boolean value to true
	void reactivate_cubes();
	//! helper function to check to see if a left rotation can be done
	bool check_left();
	//! helper function to theck to see if a right rotation can be done
	bool check_right();
  private:

};








