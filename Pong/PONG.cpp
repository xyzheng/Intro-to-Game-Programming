//Nick Zheng
//Assignment 2 - Pong

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"

SDL_Window* displayWindow;

Entity* ball;
Entity* paddleRed;
Entity* paddleBlu;
Entity* medal;
Entity* blockTop; 
Entity* blockBot;

float lastFrameTicks;

const Uint8 *keys = SDL_GetKeyboardState(NULL);

//Loads texture
GLuint loadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

//create entities and initialize
void setUp(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	GLuint greyBall = loadTexture("ballGrey.png");
	GLuint redPaddle = loadTexture("paddleRed.png");
	GLuint bluePaddle = loadTexture("paddleBlu.png");
	GLuint greyMedal = loadTexture("shaded_medal8.png");
	GLuint blocks = loadTexture("element_grey_rectangle.png");

	ball = new Entity(greyBall, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f);
	paddleRed = new Entity(redPaddle, 1.0f, 0.0f, 0.0f, 0.1f, 0.3f);
	paddleBlu = new Entity(bluePaddle, -1.0f, 0.0f, 0.0f, 0.1f, 0.3f);
	blockTop = new Entity(blocks, 0.0f, 1.0f, 0.0f, 3.0f, 0.2f);
	blockBot = new Entity(blocks, 0.0f, -1.0f, 0.0f, 3.0f, 0.2f);
	medal = new Entity(greyMedal, 0.0f, 0.0f, 0.0f, 0.2f, 0.2f);

	ball->speed = 1.0f;
	ball->xDirect = 1.0f;
	ball->yDirect = 1.0f;
}

//space to start over
bool processEvents(){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		else if(keys[SDL_SCANCODE_SPACE]){
			ball->draw();
			ball->x = 0.0f;
			ball->y = 0.0f;
		}
	}
	return true;
}


void update(){
	
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ball->x += ball->speed*ball->xDirect*elapsed;
	ball->y += ball->speed*ball->yDirect*elapsed;

	if(keys[SDL_SCANCODE_UP] && paddleRed->y < 0.8f){
		paddleRed->y += 2.0f*elapsed;
	}
	else if(keys[SDL_SCANCODE_DOWN] && paddleRed->y > -0.8){
		paddleRed->y -= 2.0f*elapsed;
	}	
	if(keys[SDL_SCANCODE_W] && paddleBlu->y < 0.8){
		paddleBlu->y += 2.0f*elapsed;
	}
	else if(keys[SDL_SCANCODE_S] && paddleBlu->y > -0.8){
		paddleBlu->y -= 2.0f*elapsed;
	}
	
	//wall collision
	if(ball->y > 0.85f && ball->yDirect > 0.0f){
		ball->yDirect = -ball->yDirect; 
	}
	else if(ball->y < -0.85f && ball->yDirect < 0.0f)
	{
		ball->yDirect = -ball->yDirect; 
	}

	//red paddle
	if(!((ball->x - 0.05f)>(paddleRed->x + 0.05f)) && !((ball->x + 0.05f)<(paddleRed->x - 0.05f)) && 
		!((ball->y - 0.05f)>(paddleRed->y + 0.05f)) && !((ball->y + 0.05f)<(paddleRed->y -0.05f))){
		//reverse directions
		ball->xDirect = -1.0f;
		}
	//blue paddle
	else if(!((ball->x - 0.05f)>(paddleBlu->x + 0.05f)) && !((ball->x + 0.05f)<(paddleBlu->x - 0.05f)) &&
		!((ball->y - 0.05f)>(paddleBlu->y + 0.05f)) && !((ball->y + 0.05f)<(paddleBlu->y - 0.05f))){
		//reverse directions
		ball->xDirect = 1.0f;
	}


}

void render(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	paddleRed->draw();
	paddleBlu->draw();
	blockTop->draw();
	blockBot->draw();

	//win conditions
	if(ball->x > 1.33){
		medal->draw();
		medal->x = -0.5f;
	}
	else if(ball->x < -1.33){
		medal->draw();
		medal->x = 0.5f;
	}
	else{
		ball->draw();
	}

	SDL_GL_SwapWindow(displayWindow);

}

void cleanUp(){
	delete ball, paddleRed, paddleBlu, medal, blockTop, blockBot;
	SDL_Quit();
}

int main(int argc, char *argv[]){
	setUp();
	while(processEvents()){
		update();
		render();
	}
	cleanUp();
	return 0;

}
