C_FLGS = -std=c++11 -Wall -g 
OBJECT_F = glut_handler.o shaders.o camera.o string+.o game.o tetromino.o cube.o  
LINKS = -lGL -lglut -lGLEW


all: main.o $(OBJECT_F)
	g++ -o hardertetris main.o $(OBJECT_F) $(C_FLGS) $(LINKS)

main.o: main.cc glut_handler.o game.o string+.o
	g++ -c $(C_FLGS) main.cc

game.o: game.cc game.h
	g++ -c $(C_FLGS) game.cc

tetromino.o: tetromino.cc tetromino.h
	g++ -c $(C_FLGS) tetromino.cc

cube.o: cube.cc cube.h
	g++ -c $(C_FLGS) cube.cc

shaders.o: shaders.cc shaders.h
	g++ -c $(C_FLGS) shaders.cc

glut_handler.o: glut_handler.cc glut_handler.h
	g++ -c $(C_FLGS) glut_handler.cc

camera.o: camera.cc camera.h
	g++ -c $(C_FLGS) camera.cc

string+.o: string+.cc string+.h
	g++ -c $(C_FLGS) string+.cc

gdb:
	g++ -g -o debug $(C_FLGS) *.cc $(LINKS)

tar:
	tar -cvzf harder_tetris.tar.gz ./*

clean:
	rm *~  *.o hardertetris harder_tetris.tar.gz

