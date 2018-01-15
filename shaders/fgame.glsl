varying vec4 color;

void main(){

	gl_FragColor.r   = color.r / 256.0;
	gl_FragColor.g   = color.g / 256.0;
	gl_FragColor.b   = color.b / 256.0;
	gl_FragColor.a   = 1.0;

}
