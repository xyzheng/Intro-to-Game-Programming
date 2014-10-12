//Nick Zheng
//Assignment 4 - Single Screen Platformer
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "App.h"
#include <string>
#include <vector>

int main(int argc, char *argv[]){

	App app;
	while(!app.updateAndRender()){};
	return 0;

}