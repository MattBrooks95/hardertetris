attribute vec4 vPosition;
attribute vec4 vColor;

varying vec4 color; //gets passed through to fragment shader

uniform mat4 model_view;
uniform mat4 camera_view;
uniform mat4 perspective;

void main(){

	//######### POINTS & PERSPECTIVE #########
	gl_Position = perspective  * camera_view * model_view * vPosition;
	//########################################

	//######### COLORS #######################
	color   = vColor;
	//########################################

}
