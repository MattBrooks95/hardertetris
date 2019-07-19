
#include "glut_handler.h"

using namespace std;


//#define CLEAR_COLOR 0.0,0.0,0.0,1.0
#define CLEAR_COLOR 0.7, 0.7, 0.7, 1.0

//access globals from main
extern glut_handler* glut_access;
extern shaders*      shader_access;
extern game*         game_access;
extern camera*       gaze_access;

//######################################################################################
glut_handler::glut_handler(int& argc, char* argv[]){
	//call the glut init function
	glutInit(&argc,argv);

	//double buffer makes animation look better
	glutInitDisplayMode(GLUT_FLAGS);
	window_info = WINDOW_DEFAULT;

	//tell glut where to put the window, and how big it should be
	glutInitWindowPosition(window_info.x, window_info.y);
	glutInitWindowSize(window_info.w, window_info.h);

	string window_arg = WINDOW_CAPTION;
	glutCreateWindow(window_arg.c_str());

	//set up the display callback
	glutDisplayFunc(display);

	//set up the keyboard handling callback
	glutKeyboardFunc(handle_key);

	//set up the special keyboard handling callback
	glutSpecialFunc(handle_other_keys);

	//set up the mouse handling callback
	glutMouseFunc(handle_mouse);

	//set up the resize handling callback
	glutReshapeFunc(handle_resize);

	//set up the timer func to allow consistent FPS
	glutTimerFunc(TIME_PER_FRAME, timerfunc, 0);   //Dr. Chelberg's animate.cc used as reference

	//why was this required to run? It would cause InitShader or any function
	//that had remotely anything to do with the gpu to seg fault w/o an error code or anything
	glewInit();

	ticks = 0;//start the timer off at zero

}
//######################################################################################

//######################################################################################
void glut_handler::display(){

	glClearColor(CLEAR_COLOR);
	//clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	game_access->draw_scene();
	game_access->draw_cubes();
	game_access->print_game_bools();
	//swap the buffers
	glutSwapBuffers();
	glFlush();
}
//######################################################################################

//######################################################################################
void glut_handler::handle_key(unsigned char key, int x, int y){

	switch(key){

	  case ESCAPE:
		exit(EXIT_SUCCESS);  //exit the program
		break;

	  case 'w':
	  case 'W':
		game_access->move_list.push(fast_drop);
		break;

	  case 's':
	  case 'S':
		game_access->move_list.push(medium_drop);
		break;

	  case 'a':
	  case 'A':

		game_access->move_list.push(move_left);
		break;

	  case 'd':
	  case 'D':
		game_access->move_list.push(move_right);
		break;

      case 'e':
      case 'E':
		game_access->move_list.push(rot_right);
        break;

	  case 'q':
	  case 'Q':
		game_access->move_list.push(rot_left);
		break;

	  case 'r':
	  case 'R':

		handle_resize(ORIG_WIDTH,ORIG_HEIGHT);
		glutReshapeWindow(ORIG_WIDTH,ORIG_HEIGHT);
		break;

	  case 'p':
	  case 'P':
		std::cout << "Pausing." << endl;
		game_access->paused = !game_access->paused;
		game_access->print_game_bools();
		break;
	  default:
		std::cout << "Key: " << key << " unrecognized." << std::endl;

	}

}
//######################################################################################

//######################################################################################
void glut_handler::handle_other_keys(int key, int x, int y){
	switch(key){

		case GLUT_KEY_UP:
			handle_key('w',x,y);
			break;

		case GLUT_KEY_DOWN:
			handle_key('s',x,y);
			break;

		case GLUT_KEY_LEFT:
			handle_key('a',x,y);
			break;

		case GLUT_KEY_RIGHT:
			handle_key('d',x,y);
			break;

		default:
			cout << "Unrecognized key press: " << key << endl;
			break;
	}


}
//######################################################################################

//######################################################################################
void glut_handler::handle_mouse(int button, int state, int x, int y){

	//this logic switch allows us to only process mouse events in
	//button down & button up pairs
	static bool do_we_care = true;

	if(state == 0 && do_we_care){

		if(button == 0){
		} else if(button == 2){
		}


		do_we_care = false;

	} else if(state == 1 && !do_we_care){

		do_we_care = true;
	}

}
//######################################################################################

//######################################################################################
void glut_handler::handle_resize(int width_in, int height_in){

	//use the global access pointer to update the class's pointers
	glut_access->update_class_window_info(width_in,height_in);

	glViewport(0,0,width_in,height_in);

	//tell GLUT it should redraw the window at some point
	glutPostRedisplay();
}
//######################################################################################

//######################################################################################
void glut_handler::timerfunc(int value){

	glut_access->ticks++;

	static int context_switch_seconds = DEFAULT_CONTEXT_SWITCH;
	static bool strange_view = false;

	//log passage of time in seconds
	if(glut_access->ticks % TICKS_PER_SEC == 0 ){
		int seconds = glut_access->ticks / TICKS_PER_SEC;
		cout << "Second:" << seconds << endl;
		//decrement the time until next context switch
		if(!game_access->paused) context_switch_seconds--;

		if(context_switch_seconds == 0){
			if(!strange_view){
				int random_number = rand() % 4;
				//int random_number = 0;
				//cout << "CONTEXT SWITCH RAND: " << random_number << endl;
				switch(random_number){

					case 0:
						gaze_access->right_side_view();
						break;

					case 1:
						gaze_access->left_side_view();
						break;
					case 2:
						gaze_access->behind_view();
						break;
					case 3:
						gaze_access->upside_down_view();
						break;

					default:
						cout << "Bad camera position!" << endl;
						exit(-1);
				}
				strange_view = true;
				context_switch_seconds = 3;//go back to normal after 5 seconds
				//context_switch_seconds = 20;//to test contexts
			} else {
				strange_view = false;
				gaze_access->normal_view();
				context_switch_seconds = 20;
			}
		}
	}


	//update the object's locations
	//game_access->calc_game_state();

	//update the object's locations
	if(glut_access->ticks % 8 == 0 && glut_access->ticks != 0){
		game_access->calc_game_state();
	}

	glutPostRedisplay();
	glutTimerFunc(TIME_PER_FRAME, timerfunc,0);
}
//######################################################################################

//######################################################################################
void glut_handler::update_class_window_info(int width_in,int height_in){
	window_info.w = width_in;
	window_info.h = height_in;
}
//######################################################################################

/*
glut_handler::~glut_handler(){





}
*/





