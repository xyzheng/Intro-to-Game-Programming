//Nick Zheng
//Assignment 3 - Space Invaders
#include "App.h"

enum GameState{STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

//loads texture
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
	lastFrameTicks = 0.0f;

	glMatrixMode(GL_MODELVIEW);
}

//destructor
App::~App(){
	SDL_Quit();
}

void App::init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
}

void App::reset(){
	score = 0;
	player.alive = true;
	enemies.clear();
	for(size_t i=0; i<enemyBullets.size(); i++){
		delete enemyBullets[i];
	}
	enemyBullets.clear();
	//load textures
	unsigned int spriteSheet = loadTexture("sheet.png");
	player = Entity(spriteSheet, 0.0f, -0.8f, 0.0f, 0.0f, 0.0f, 211.0f/1024.0f, 941.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f);
	bullet = Entity(spriteSheet, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 856.0f/1024.0f, 421.0f/1024.0f, 9.0f/1024.0f, 54.0f/1024.0f);
	bullet.visible = false;
	for(float i=-1.1f; i<0.7f; i+=0.2f){
		enemies.push_back(new Entity(spriteSheet, i, 0.7f, 1.0f, 0.0f, 0.0f, 423.0f/1024.0f, 728.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.f));
		enemies.push_back(new Entity(spriteSheet, i, 0.5f, 1.0f, 0.0f, 0.0f, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.f));
		enemies.push_back(new Entity(spriteSheet, i, 0.3f, 1.0f, 0.0f, 0.0f, 425.0f/1024.0f, 552.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.f));
		enemies.push_back(new Entity(spriteSheet, i, 0.1f, 1.0f, 0.0f, 0.0f, 425.0f/1024.0f, 384.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.f));
	}
	for(size_t i=0; i<enemies.size(); i++){
		enemyBullets.push_back(new Entity(spriteSheet, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 848.0f/1024.0f, 684.0f/1024.0f, 13.0f/1024.0f, 54.0f/1024.0f));
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
void App::renderMainMenu(){
	drawText(font, "SPACE INVADERS", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.7f, 0.7f);
	drawText(font, "ARROW KEYS TO MOVE", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.5f);	
	drawText(font, "SPACE TO SHOOT", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.7f, 0.3f);
	drawText(font, "R FOR GAME OVER", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, 0.1f);
	drawText(font, "Press SPACE to start", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.9f, -0.3f);
}

//renders game level
void App::renderGameLevel(){
	drawText(font, "Score:" + to_string(score), 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.2f, 0.9f);
	if(player.alive){
		player.drawSprite(0.8f);
	}
	if(bullet.visible){
		bullet.drawSprite(0.8f);
	}
	for(size_t i=0; i<enemies.size(); i++){
		if(enemies[i]->alive){
			enemies[i]->drawSprite(0.8f);
		}
	}
	for(size_t i=0; i<enemyBullets.size(); i++){
		if(enemyBullets[i]->visible){
			enemyBullets[i]->drawSprite(0.8f);
		}
	}
}

//renders game over
void App::renderGameOver(){
	drawText(font, "GAME OVER", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
	drawText(font, "Score:" + to_string(score), 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.7f, 0.5f);
	drawText(font, "Press ENTER for main menu", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.2f, 0.3f);
}

void App::update(float elapsed){
	SDL_Event event;

	if(state == STATE_GAME_LEVEL){
		updateGameLevel(elapsed);
	}

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
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

void App::updateGameLevel(float elapsed){
	//lose/win conditions
	if(score == 4000 || !player.alive){
		state = STATE_GAME_OVER;
	}
	for(size_t i=0; i<enemies.size(); i++){
		if(enemies[i]->y < -0.7f){
			state = STATE_GAME_OVER;
		}
	}
	//enemy bullet collision
	for(size_t i=0; i<enemyBullets.size(); i++){
		if(player.collide(enemyBullets[i])){
			player.alive = false;
			state = STATE_GAME_OVER;
		}
	}
	//enemy shoots
	if(enemies.size() != 0){
		for(size_t i=0; i<enemies.size(); i++){
			if(enemies[i]->x > player.x - 0.5f && enemies[i]->x < player.x + 0.5f && enemies[i]->alive == true){
				enemyBullets[i]->visible = true;
				enemyBullets[i]->x = enemies[i]->x;
				enemyBullets[i]->y = enemies[i]->y - enemies[i]->height*0.8f;
			}
		}
	}
	for(size_t i=0; i<enemyBullets.size(); i++){
		if(enemyBullets[i]->visible){
			enemyBullets[i]->y -= 2.0f * elapsed;
		}
	}

 	//move enemies
	for(size_t i=0; i<enemies.size(); i++){
		enemies[i]->x += 0.5f*enemies[i]->xDirection*elapsed;
		if(enemies[i]->x > 1.0f && enemies[i]->xDirection > 0.0f){
			enemies[i]->xDirection = -enemies[i]->xDirection;
			enemies[i]->y = enemies[i]->y - 0.1f;
		}
		else if(enemies[i]->x < -1.0f && enemies[i]->xDirection < 0.0f){
			enemies[i]->xDirection = -enemies[i]->xDirection;
			enemies[i]->y = enemies[i]->y - 0.1f;
		}
	}
	//move player ship left
	if(keys[SDL_SCANCODE_LEFT] && player.x > -1.1f){
		player.x -= 2.0f * elapsed;
	}
	//move player ship right
	else if(keys[SDL_SCANCODE_RIGHT] && player.x < 1.1f){
		player.x += 2.0f * elapsed;
	}
	if(keys[SDL_SCANCODE_SPACE] && state == STATE_GAME_LEVEL){
		bullet.visible = true;
		bullet.x = player.x;
		bullet.y = player.y + player.height*0.8f;
	}
	if(bullet.visible){
		bullet.y += 2.0f*elapsed;
	}
	if(bullet.y > 1.2f){
		bullet.visible = false;
	}

	//bullet collision on enemies
	for(size_t i=0; i<enemies.size(); i++){
		if(enemies[i]->collide(&bullet)){
			if(enemies[i]->alive && bullet.visible == true){
				enemies[i]->alive = false;
				bullet.visible = false;
				delete enemies[i];
				score += 100;
			}
		}
	}
}

bool App::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	render();
	update(elapsed);
	return done;
}