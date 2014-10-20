//Nick Zheng
//Assignment 4 - Single Screen Platformer WITH SOUND
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include "App.h"
#include <vector>
#include <string>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
float timeLeftOver = 0.0f;
float lastFrameTicks;

using namespace std;

enum GameState {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

GLuint loadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	if (!surface){return -1;}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);

	return textureID;
}

//draw texts
void drawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y){
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		for(int j=0; j < 4; j++) {
			colorData.push_back(r);
			colorData.push_back(g);
			colorData.push_back(b);
			colorData.push_back(a);
		}

		vertexData.push_back(((size + spacing) * i) + (-0.5f* size)); 
		vertexData.push_back(0.5f* size);
		vertexData.push_back(((size + spacing) * i) + (-0.5f* size)); 
		vertexData.push_back(-0.5f* size);
		vertexData.push_back(((size + spacing) * i) + (0.5f* size));
		vertexData.push_back(-0.5f* size);
		vertexData.push_back(((size + spacing) * i) + (0.5f* size));
		vertexData.push_back(0.5f* size);

		texCoordData.push_back(texture_x);
		texCoordData.push_back(texture_y);
		texCoordData.push_back(texture_x);
		texCoordData.push_back(texture_y + texture_size);
		texCoordData.push_back(texture_x + texture_size);
		texCoordData.push_back(texture_y + texture_size);
		texCoordData.push_back(texture_x + texture_size);
		texCoordData.push_back(texture_y);
	}
	
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}

//constructor
App::App() {
	init();

	keys = SDL_GetKeyboardState(NULL);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	state = STATE_MAIN_MENU;

	font = loadTexture("spritesheet_font.png");

	done = false;
	gravity = -9.8f;

	glMatrixMode(GL_MODELVIEW);
}

//destructor
App::~App(){
	Mix_FreeChunk(gemSound);
	Mix_FreeChunk(jumpSound);
	Mix_FreeChunk(dieSound);
	Mix_FreeChunk(winSound);
	Mix_FreeMusic(music);

	SDL_Quit();
}

//initializer
void App::init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
}

void App::reset(){
	//music/sounds
	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
	gemSound = Mix_LoadWAV("gem.wav");
	jumpSound = Mix_LoadWAV("jump.wav");
	winSound = Mix_LoadWAV("win.wav");
	dieSound = Mix_LoadWAV("die.wav");
	music = Mix_LoadMUS("bmg.mp3");

	Mix_PlayMusic(music, -1);

	for(size_t i=0; i<enemies.size(); i++){
		delete enemies[i];
	}
	enemies.clear();
	for(size_t i=0; i<blocks.size(); i++){
		delete blocks[i];
	}
	blocks.clear();

	//load player sprite
	unsigned int sheetSprite = loadTexture("spritesheet_aliens.png");
	//	textureID(spritesheet), x(x), y(y), xD(xD), yD(yD), rotation(rotate), u(u), v(v), width(width), height(height),
	//	mass(mass), dynamic(true), collide(false)
	player = Entity(sheetSprite, -1.0f, -0.7f, 0.0f, 0.0f, 0.0f, 70.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, true, true);

	//load map sprite
	unsigned int mapSprite = loadTexture("spritesheet_tiles.png");
	endPoint = Entity(mapSprite, 1.2f, 0.6f, 0.0f, 0.0f, 0.0f, 350.0f/512.0f, 70.0/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true);
	//push them into the vector
	for(float i=-1.4f; i<-0.9f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, -0.9f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}
	for(float i=-1.3f; i<-1.1f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, -0.5f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}
	for(float i=-0.7f; i<-0.2f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, -0.7f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}
	for(float i=0.0f; i<0.2f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, -0.5f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}
	for(float i=-0.8f; i<-0.3f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, -0.2f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}
	for(float i=-0.1f; i<0.5f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, 0.0f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}
	blocks.push_back(new Entity(mapSprite, 0.6f, 0.2f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	for(float i=0.8f; i<1.4f; i+=0.1f){
		blocks.push_back(new Entity(mapSprite, i, 0.4f, 0.0f, 0.0f, 0.0f, 280.0f/512.0f, 70.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	}

	//stars
	stars.push_back(new Entity(mapSprite, -0.7f, -0.5f, 0.0f, 0.0f, 0.0f, 140.0f/512.0f, 140.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	stars.push_back(new Entity(mapSprite, -0.0f, -0.3f, 0.0f, 0.0f, 0.0f, 140.0f/512.0f, 140.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));
	stars.push_back(new Entity(mapSprite, 0.4f, 0.2f, 0.0f, 0.0f, 0.0f, 140.0f/512.0f, 140.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, false, true));

	//enemies
	enemies.push_back(new Entity(sheetSprite, -0.4f, -0.5f, 0.0f, 0.0f, 0.0f, 140.0f/512.0f, 70.0/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, true, true));
	enemies.push_back(new Entity(sheetSprite, 0.4f, 0.2f, 0.0f, 0.0f, 0.0f, 210.0f/512.0f, 140.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, true, true));
	enemies.push_back(new Entity(sheetSprite, 1.2f, 0.6f, 0.0f, 0.0f, 0.0f, 70.0f/512.0f, 140.0f/256.0f, 70.0f/512.0f, 70.0f/256.0f, 1.0f, true, true));

	//reset velocity, collision, etc
	player.collidedTop = false;
	player.collidedBot = false;
	player.collidedRight = false;
	player.collidedLeft = false;
	player.xVelo = 0.0f;
	player.yVelo = 0.0f;
//	player.xAcc = 0.0f;
	player.yAcc = 0.0f;
	player.yFric = 1.0f;
	player.xFric = 1.0f;
	for(size_t i=0; i<enemies.size(); i++){
		enemies[i]->xFric = 0.0f;
		enemies[i]->yFric = 0.0f;
		enemies[i]->xAcc = 0.0f;
		enemies[i]->collidedBot = false;
		enemies[i]->xVelo = 0.3f;
	}

}

//renders the states
void App::render(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	switch(state){
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
	SDL_GL_SwapWindow(displayWindow);
}

//renders main menu
//GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y
void App::renderMainMenu(){
	drawText(font, "GEMS", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.3f, 0.7f);
	drawText(font, "Collect all gems", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, 0.5f);	
	drawText(font, "Arrow keys to move", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.9f, 0.3f);
	drawText(font, "Space to jump", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, 0.1f);
	drawText(font, "Reach the end", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.1f);
	drawText(font, "Press SPACE to start", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.9f, -0.3f);
}

//renders game level
void App::renderGameLevel(){
	player.drawSprite(0.4f);
	for(size_t i=0; i<blocks.size(); i++){
		blocks[i]->drawSprite(0.4f);
	}
	for(size_t i=0; i<enemies.size(); i++){
		enemies[i]->drawSprite(0.4f);
	}
	for(size_t i=0; i<stars.size(); i++){
		if(stars[i]->visible == true){
			stars[i]->drawSprite(0.4f);
		}
	}
	endPoint.drawSprite(0.4f);
}

void App::updateGameLevel(){
	if(keys[SDL_SCANCODE_RIGHT]){
		player.xVelo += 2.0f * FIXED_TIMESTEP;
		player.xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_LEFT]){
		player.xVelo -= 2.0f * FIXED_TIMESTEP;
		player.xAcc = -0.5f;
	}
	else{
		player.xAcc = 0.0f;
	}

	//borders
	if(player.x - player.width*0.4f < -1.3f){
		player.x = -1.3f + player.width*0.4f;
	}
	if(player.x + player.width*0.4f > 1.3f){
		player.x = 1.3f - player.width*0.4f;
	}

	//player time steps
	player.yVelo = lerp(player.yVelo, 0.0f, FIXED_TIMESTEP * player.yFric);
	player.yVelo += player.yAcc * FIXED_TIMESTEP;
	player.y += player.yVelo * FIXED_TIMESTEP;
	
	player.xVelo = lerp(player.xVelo, 0.0f, FIXED_TIMESTEP * player.xFric);
	player.xVelo += player.xAcc * FIXED_TIMESTEP;
	player.x += player.xVelo * FIXED_TIMESTEP;
	player.yVelo += gravity * FIXED_TIMESTEP;
	
	//enemy time steps
	for(size_t i=0; i<enemies.size(); i++){
			enemies[i]->xVelo += enemies[i]->xAcc * FIXED_TIMESTEP;
			enemies[i]->x += enemies[i]->xVelo * FIXED_TIMESTEP;

			enemies[i]->xVelo = lerp(enemies[i]->xVelo, 0.0f, FIXED_TIMESTEP * enemies[i]->xFric);
			enemies[i]->xVelo += enemies[i]->xAcc * FIXED_TIMESTEP;
			enemies[i]->x += enemies[i]->xVelo * FIXED_TIMESTEP;
	}
	
	//enemies move back and forth
	for(size_t i=0; i<blocks.size(); i++){
		for(size_t j=0; j<enemies.size(); j++){
			if(enemies[j]->collideY(blocks[i])){
				if(enemies[0]->x < -0.7f){
					enemies[0]->xVelo = -enemies[0]->xVelo;
				}
				else if(enemies[0]->x > -0.35f){
					enemies[0]->xVelo = -enemies[0]->xVelo;
				}
				if(enemies[1]->x < -0.1f){
					enemies[1]->xVelo = -enemies[1]->xVelo;
				}
				else if(enemies[1]->x > 0.4f){
					enemies[1]->xVelo = -enemies[1]->xVelo;
				}
				if(enemies[2]->x < 0.8f){
					enemies[2]->xVelo = -enemies[2]->xVelo;
				}
				else if(enemies[2]->x > 1.2f){
					enemies[2]->xVelo = -enemies[2]->xVelo;
				}
				if(enemies[j]->collidedBot){
					 enemies[j]->y = blocks[i]->y + blocks[i]->height*0.4f + enemies[j]->height*0.3f; 
					 enemies[j]->yVelo = 0.0f; 
					 enemies[j]->collidedBot = false;
				}
			}
		}
	}
	
	//player map collision
	for(size_t i=0; i<blocks.size(); i++){
		if(player.collideY(blocks[i])){
			if(player.collidedTop){
				player.y = blocks[i]->y - blocks[i]->height*0.4f - player.height*0.4f; 
				player.yVelo = 0.0f;
				player.collidedTop = false;
			}
			else if(player.collidedBot){
				player.y = blocks[i]->y + blocks[i]->height*0.4f + player.height*0.4f;
				player.yVelo = 0.0f;
			}
		}
		if(player.collideX(blocks[i])){
			if(player.collidedLeft){
				player.x = blocks[i]->x + blocks[i]->width*0.4f + player.width*0.4f;
				player.xVelo = 0.0f;
				player.collidedLeft = false;
			}
			else if(player.collidedRight){
				player.x = blocks[i]->x - blocks[i]->width*0.4f - player.width*0.4f;
				player.xVelo = 0.0f;
				player.collidedRight = false;
			}
		}
	}

	//player star collision
	for(size_t i=0; i<stars.size(); i++){
		if(player.collideY(stars[i]) || player.collideX(stars[i])){
			if(stars[i]->visible){
				Mix_PlayChannel(-1, gemSound, 0);
			}
			stars[i]->visible = false;
		}
	}

	//dead conditions
	for(size_t i=0; i<enemies.size(); i++){
		if(enemies[i]->collideY(&player) || enemies[i]->collideX(&player) || player.y < -1.0f){
			Mix_PlayChannel(-1, dieSound, 0);
			state = STATE_GAME_OVER;
		}
	}

	//win condition
	if(!stars[0]->visible && !stars[1]->visible && !stars[2]->visible  && (player.collideX(&endPoint) || player.collideY(&endPoint))){
		Mix_PlayChannel(-1, winSound, 0);
		state = STATE_GAME_OVER;
	}
}

//renders game over
void App::renderGameOver(){
	drawText(font, "GAME OVER", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
	drawText(font, "Press Enter for main menu", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.2f, 0.3f);
}

void App::update(){
	SDL_Event event;

	if(state == STATE_GAME_LEVEL){
		updateGameLevel();
	}

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		//jump
		if(state == STATE_GAME_LEVEL && event.key.keysym.scancode == SDL_SCANCODE_SPACE && player.collidedBot == true){
			player.yVelo = 3.0f;
			player.collidedBot = false;
			Mix_PlayChannel(-1, jumpSound, 0);
		}
		if(state == STATE_GAME_LEVEL){
			if(event.key.keysym.scancode == SDL_SCANCODE_R){
				state = STATE_GAME_OVER;
			}
		}
		if(state == STATE_MAIN_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
				state = STATE_GAME_LEVEL;
				reset();
			}
		}
		if(state == STATE_GAME_OVER){
			if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = STATE_MAIN_MENU;
			}
		}
	}
}

bool App::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}

	while (fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTEP;
		update();
		render();
		elapsed -= FIXED_TIMESTEP;
	}
	timeLeftOver = fixedElapsed;

	return done;
}