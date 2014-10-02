#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "App.h"


enum GameState {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

int state;

using namespace std;

//loads texture
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

//text drawing
/*void drawText(int fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y){
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	float texture_size = 1.0/16.0f;

	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (unsigned int i = 0; i < text.size(); i++){
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), {r, g, b, a, 
										   r, g, b, a, 
										   r, g, b, a, 
										   r, g, b, a });
		vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, 
											 ((size+spacing) * i) + (-0.5f * size), -0.5f * size, 
											 ((size+spacing) * i) + (0.5f * size), -0.5f * size, 
											 ((size+spacing) * i) + (0.5f * size), 0.5f * size}); 
		texCoordData.insert(texCoordData.end(), {texture_x, texture_y, 
												 texture_x, texture_y + texture_size, 
												 texture_x + texture_size, texture_y + texture_size, 
												 texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}
*/

App::App(){
	init();
	done = false;
	lastFrameTicks = 0.0f;
}

//destructor
App::~App(){
	SDL_Quit();
}

//initialize
void App::init() { 
	SDL_Init(SDL_INIT_VIDEO); 
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); 
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow); 
	SDL_GL_MakeCurrent(displayWindow, context); 

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	state = STATE_MAIN_MENU;
	keys = SDL_GetKeyboardState(NULL);

	glMatrixMode(GL_MODELVIEW);
}

//renders the states
void App::render(){

	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

/*	switch(state){
		case STATE_MAIN_MENU:
			renderMainMenu();
		break;
		case STATE_GAME_LEVEL:
			renderGameLevel();
		break;
		case STATE_GAME_OVER:
			renderGameOver();
		break;
	}
*/
	SDL_GL_SwapWindow(displayWindow);
}

//renders the main menu
/*void App::renderMainMenu(){
	GLuint text = loadTexture("spritesheet_font.png");
	drawText(text, "SPACE INVADERS", 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	drawText(text, "Press Enter to start", 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	drawText(text, "Left and right arrows to move", 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	drawText(text, "Spacebar to shoot", 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

//renders the game over screen
void App::renderGameOver(){
	GLuint text = loadTexture("spritesheet_font.png");
	drawText(text, "Final Score:", 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	drawText(text, to_string(score), 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	drawText(text, "Press Spacebar to start over", 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
*/

void App::renderGameLevel(){
	for(int i=0; i<10; i++){
		enemiesRowOne[i].render();
	}
}


//update and renders
bool App::updateAndRender(){
	float ticks = (float)SDL_GetTicks()/1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	update(elapsed);
	render();
	return done;
}

//restarts the game
void App::restart(){
	player->aliveStatus = true;
	score = 0;

	GLuint sheetTexture = loadTexture("sheet.png");
	for(int i=0; i<10; i++){
		Entity temp = Entity(sheetTexture, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.5f);
		enemiesRowOne.push_back(temp);
	}

	float initialX = 0.0f;
	float initialY = 0.0f;

	for(int i=0; i<10; i++){
		enemiesRowOne[i].x = 0.0f;
		enemiesRowOne[i].y = 0.0f;
		enemiesRowOne[i].aliveStatus = true;
	}
}

void App::update(float elapsed) {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		}
}















































































