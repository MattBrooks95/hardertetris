Matthew Brooks

This directory contains my final project for
CS4250 interactive computer graphics with Dr. Chelberg.

12/12/2017

I've implemented a simplified ( and in many ways lacking) game of Tetris,
but in 3D.

The 3D comes in handy for a 2d game because it allows me to do really
	annoying, but hopefully humorous, camera tricks.
The game view alternates between being normal and
	modified every 20 seconds and 3 seconds.

controls are:
	right arrow key: move piece to the right
	left arrow key: move piece to the left
	'q' : rotate current piece to the left
	'e' : rotate current piece to the right

Cool points:
	1.) Implementing Tetris took much longer than I thought it would
	2.) Moves are buffered in a queue from Glut interrupts, and read
			only during 'physics' steps
	3.) Controls make use of both WASD and the arrow keys
	4.) wrote own shader loading class, albeit with the help of
			reference: http://www.opengl-tutorial.org/beginners-tutorials/
							tutorial-2-the-first-triangle/#shaders
	5.) Used the modern-day rotating algorithm SRS
			reference: http://tetris.wikia.com/wiki/SRS 
			with the exception of wall kicks
	6.) Use of parent class Tetromino, and it's derivative pieces
			that overload the rotating functions to implement
			that piece special logic
	7.) Used the GLM library for matrix logic instead of the books code
			-> I believe this is typically what is used in industry
