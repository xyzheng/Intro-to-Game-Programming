#include "App.h"

enum GameState{ STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

int state;

GLuint loadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);

	return textureID;
}

//constructor
App::App(){
	done = false;
	lastFrameTicks = 0.0f;
	score = 0;
	init();
}

//destructor
App::~App(){
	SDL_Quit();
}

//initialize
void App::init(){

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	//load textsheets and spritesheet
	textSheet = loadTexture("spritesheet_font.png");
	spriteSheet = loadTexture("sheet.png");

	//initialize player sprite
	Spritesheet user = Spritesheet(spriteSheet, 237.0f/1024.0f, 377.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f);
	player = new Entity(user, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f);

	float initialPosX = -1.0f;
	//row one enemies
	Spritesheet enemy = Spritesheet(spriteSheet, 423.0f/1024.0f, 728.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f);
	for(int i=0; i<10; i++){
		Entity* enemyOne = new Entity(enemy, 0.5f, initialPosX, 1.0f, 0.0f, 0.0f);
		rowOne.push_back(enemyOne);
		initialPosX += 0.1f;
	}

	//row two enemies
	initialPosX = -1.0f;
	enemy = Spritesheet(spriteSheet, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f);
	for(int i=0; i<10; i++){
		Entity* enemyTwo = new Entity(enemy, 0.5f, initialPosX, 0.8f, 0.0f, 0.0f);
		rowTwo.push_back(enemyTwo);
		initialPosX += 0.1f;
	}

	//row three enemies
	initialPosX = -1.0f;
	enemy = Spritesheet(spriteSheet, 425.0f/1024.0f, 552.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f);
	for(int i=0; i<10; i++){
		Entity* enemyThree = new Entity(enemy, 0.5f, initialPosX, 0.6f, 0.0f, 0.0f);
		rowThree.push_back(enemyThree);
		initialPosX += 0.1f;
	}

	//row four enemies
	initialPosX = -1.0f;
	enemy = Spritesheet(spriteSheet, 425.0f/1024.0f, 384.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f);
	for(int i=0; i<10; i++){
		Entity* enemyFour = new Entity(enemy, 0.5f, initialPosX, 0.4f, 0.0f, 0.0f);
		rowFour.push_back(enemyFour);
		initialPosX += 0.1f;
	}

}

//render states
void App::render(){
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
}

//update depending on states
void App::update(float elapsed){
	switch(state){
	case STATE_MAIN_MENU:
		updateMainMenu(elapsed);
	break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
	break;
	case STATE_GAME_OVER:
		updateGameOver(elapsed);
	break;
	}
}

//draw texts
void App::drawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

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

//render main menu with texts
void App::renderMainMenu(){
  glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.4f, 1.0f, 0.0f);
	drawText(textSheet, "SPACE INVADERS", 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);
	glLoadIdentity();
	glTranslatef(-04.f, 0.5f, 0.0f);
	drawText(textSheet, "ARROW KEYS TO MOVE", 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);
	glLoadIdentity();
	glTranslatef(-04.f, 0.0f, 0.0f);
	drawText(textSheet, "SPACE TO SHOOT", 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);
	glLoadIdentity();
	glTranslatef(-04.f, -0.5f, 0.0f);
	drawText(textSheet, "ENTER TO START", 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);
}

//render game level
void App::renderGameLevel(){
	//render enemies
	for(size_t i=0; i<rowOne.size(); i++){
		rowOne[i]->render();
	}
	for(size_t i=0; i<rowTwo.size(); i++){
		rowTwo[i]->render();
	}
	for(size_t i=0; i<rowThree.size(); i++){
		rowThree[i]->render();
	}
	for(size_t i=0; i<rowFour.size(); i++){
		rowFour[i]->render();
	}	
	//render bullets
	for(int i=0; i<4; i++){
		enemyShots[i].render();
	}

	glLoadIdentity();
	glTranslatef(-1.0f, 0.1f, 0.0f);
	drawText(textSheet, "Score: " + to_string(score), 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);

}

//render the game over screen
void App::renderGameOver(){
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.4f, 1.0f, 0.0f);
	drawText(textSheet, "GAME OVER", 0.2f, 0.1f, 4.0f, 3.0f, 3.0f, 2.0f);
	glLoadIdentity();
	glTranslatef(-0.4f, 0.8f, 0.0f);
	drawText(textSheet, "SCORE: " + to_string(score), 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);
	glLoadIdentity();
	glTranslatef(-0.4f, 0.6f, 0.0f);
	drawText(textSheet, "ENTER FOR MAIN MENU", 0.2f, 0.1f, 0.4f, 0.3f, 0.3f, 0.2f);
}

bool App::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	update(elapsed);
	render();
	return done;
}

//update the game over screen so it can restart the game
void App::updateGameOver(float elapsed){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_MAIN_MENU;
			}
		}
	}
}

void App::updateMainMenu(float elapsed){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				state = STATE_GAME_LEVEL;
			}
		}
	}

}

void App::shoot(){
	Spritesheet bullet = Spritesheet(spriteSheet, 143.0f/1024.0f, 377.0f/1024.0f, 43.0f/1024.0f, 31.0f/1024.0f);
	playerShots.sheetSprite = bullet;
	playerShots.visible = true;
	playerShots.scale = 0.3f;
	playerShots.speed = 5.0f;
}

void App::updateGameLevel(float elapsed){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (!playerShots.visible)
					shoot();
			}
		}
	}
	
	if(player->aliveStatus){
		for(int i=0; i<10; i++){
			if(rowOne[i]->x > 1.2f || rowOne[i]->x < -1.2f){
				for(int j=0; j<10; j++){
					rowOne[i]->xDirection = -rowOne[i]->xDirection;
				}
			}
		}

		for(int i=0; i<10; i++){
			if(rowTwo[i]->x > 1.2f || rowTwo[i]->x < -1.2f){
				for(int j=0; j<10; j++){
					rowTwo[i]->xDirection = -rowTwo[i]->xDirection;
				}
			}
		}

		for(int i=0; i<10; i++){
			if(rowThree[i]->x > 1.2f || rowThree[i]->x < -1.2f){
				for(int j=0; j<10; j++){
					rowThree[i]->xDirection = -rowThree[i]->xDirection;
				}
			}
		}

		for(int i=0; i<10; i++){
			if(rowFour[i]->x > 1.2f || rowFour[i]->x < -1.2f){
				for(int j=0; j<10; j++){
					rowFour[i]->xDirection = -rowFour[i]->xDirection;
				}
			}
		}
}
	
